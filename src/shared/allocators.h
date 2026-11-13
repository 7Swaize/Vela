#ifndef VELA_ALLOCATORS_H
#define VELA_ALLOCATORS_H

#include <cstddef>
#include <memory>
#include <new>
#include <span>
#include <type_traits>
#include <vector>

    template <typename T, std::size_t Alignment>
    struct AlignedAllocator {
        using value_type = T;

        AlignedAllocator() noexcept = default;

        template <class U>
        explicit constexpr AlignedAllocator(const AlignedAllocator<U, Alignment> &) noexcept { }

        // https://stackoverflow.com/questions/66891368/template-parametric-type-allocator-in-c
        template <class U>
        struct rebind {
            using other = AlignedAllocator<U, Alignment>;
        };

        T *allocate(const std::size_t n) {
            void *ptr = nullptr;

#if defined(_MSC_VER) || defined(_WIN32)
            ptr = _aligned_malloc(n * sizeof(T), Alignment);
            if (!ptr) {
                throw std::bad_alloc();
            }
#else
            if (posix_memalign(&ptr, Alignment, n * sizeof(T)) != 0) {
                throw std::bad_alloc();
            }
#endif
            return static_cast<T *>(ptr);
        }

        void deallocate(T *p, std::size_t) noexcept {
#if defined(_MSC_VER) || defined(_WIN32)
            _aligned_free(p);
#else
            free(p);
#endif
        }
    };

    class ArenaAllocator {
    public:
        explicit ArenaAllocator(const std::size_t chunkSize = 64 * 1024) noexcept : m_chunkSize(chunkSize) {}

        ArenaAllocator(const ArenaAllocator &) = delete;
        ArenaAllocator &operator=(const ArenaAllocator &) = delete;
        ArenaAllocator(ArenaAllocator &&) noexcept = default;
        ArenaAllocator &operator=(ArenaAllocator &&) noexcept = default;

        template <typename T, typename... Args> requires std::is_trivially_destructible_v<T>
        [[nodiscard]] T *make(Args &&...args) {
            return ::new (alloc(sizeof(T), alignof(T))) T{std::forward<Args>(args)...};
        }

        template <typename T, typename Func, typename... Args>
            requires std::is_trivially_destructible_v<T> && std::is_invocable_r_v<T*, Func, void*, Args...>
        [[nodiscard]] T *make(Func &&func, Args &&...args) {
            void* memory = alloc(sizeof(T), alignof(T));
            return std::forward<Func>(func)(memory, std::forward<Args>(args)...);
        }

        template <typename T > requires std::is_trivially_destructible_v<T>
        [[nodiscard]] std::span<T> make_array(const std::size_t n) {
            if (n == 0) {
                return {};
            }

            T *p = static_cast<T *>(alloc(n * sizeof(T), alignof(T)));
            std::uninitialized_value_construct_n(p, n);
            return {p, n};
        }

    private:
        [[nodiscard]] void *alloc(const std::size_t size, const std::size_t alignment) {
            if (void *p = try_bump(size, alignment)) {
                return p;
            }

            return alloc_slow(size, alignment);
        }

        [[nodiscard]] void *try_bump(const std::size_t size, const std::size_t alignment) {
            const std::size_t pad = (0 - reinterpret_cast<std::uintptr_t>(m_cur)) & (alignment - 1);
            if (size + pad > static_cast<std::size_t>(m_end - m_cur)) {
                return nullptr;
            }

            std::byte *ret = m_cur;
            m_cur += pad + size;
            return ret;
        }

        [[nodiscard]] void *alloc_slow(const std::size_t size, const std::size_t alignment) {
            std::byte *base = new_chunk(size);
            m_cur = base;
            m_end = base + m_chunkSize;
            return try_bump(size, alignment);
        }

        std::byte *new_chunk(const std::size_t bytes) {
            m_chunks.emplace_back(new std::byte[bytes]);
            return m_chunks.back().get();
        }


        size_t m_chunkSize;
        std::byte *m_cur = nullptr;
        std::byte *m_end = nullptr;
        std::vector<std::unique_ptr<std::byte>> m_chunks;
    };

#endif //VELA_ALLOCATORS_H
