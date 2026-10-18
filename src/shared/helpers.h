#ifndef VELA_HELPERS_H
#define VELA_HELPERS_H

#include <cstdlib>
#include <cstddef>
#include <memory>
#include <new>

#include "preprocessor.h"

namespace vela::shared {
    // clang-format off
    template<typename T, typename Deleter, typename... Args>
    VELA_FORCE_INLINE std::unique_ptr<T, Deleter> make_unique_with_deleter(Args &&...args) {
        return std::unique_ptr<T, Deleter>(new T(std::forward<Args>(args)...));
    }

    template<typename T, typename Deleter>
    VELA_FORCE_INLINE std::unique_ptr<T, Deleter> adopt_unique_with_deleter(
        typename std::unique_ptr<T, Deleter>::pointer ptr,
        Deleter &&deleter)
    {
        return std::unique_ptr<T, Deleter>(ptr, std::forward<Deleter>(deleter));
    }
    // clang-format on

    template<typename T, std::size_t Alignment>
    struct AlignedAllocator {
        using value_type = T;

        AlignedAllocator() noexcept = default;

        template<class U>
        explicit constexpr AlignedAllocator(const AlignedAllocator<U, Alignment> &) noexcept { }

        // https://stackoverflow.com/questions/66891368/template-parametric-type-allocator-in-c
        template<class U>
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
} // namespace vela::shared

#endif // VELA_HELPERS_H
