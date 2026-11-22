// ReSharper disable CppNonExplicitConvertingConstructor
#ifndef VELA_INTRUSIVE_PTR_H
#define VELA_INTRUSIVE_PTR_H

#include <cassert>
#include <compare>
#include <concepts>
#include <cstddef>
#include <utility>

namespace vela::shared {
    struct adopt_ref_t { explicit adopt_ref_t() = default; };
    inline constexpr adopt_ref_t adopt_ref{};

    template <typename T>
    concept intrusive_refcounted = requires(T &t) {
        { t.intrusive_ptr_add_ref() } noexcept;
        { t.intrusive_ptr_release_ref() } noexcept;
    };

    template <typename T>
    class IntrusivePtr {
    public:
        constexpr IntrusivePtr() noexcept = default;
        constexpr IntrusivePtr(std::nullptr_t) noexcept { }

        // Shares: increments ref count
        explicit IntrusivePtr(T *ptr) noexcept : m_ptr(ptr) { retain(m_ptr);}

        // Adopts: takes over existing without incrementing ref count
        IntrusivePtr(T *ptr, adopt_ref_t) noexcept : m_ptr(ptr) { }

        IntrusivePtr(const IntrusivePtr &other) noexcept : m_ptr(other.m_ptr) { retain(m_ptr); }
        IntrusivePtr(IntrusivePtr &&other) noexcept : m_ptr(other.detach()) { }

        template <typename U> requires std::convertible_to<U *, T *>
        IntrusivePtr(const IntrusivePtr<U> &other) noexcept : m_ptr(other.m_ptr) { retain(m_ptr); }

        template <typename U> requires std::convertible_to<U *, T *>
        IntrusivePtr(IntrusivePtr<U> &&other) noexcept :m_ptr(other.detach()) { }

        ~IntrusivePtr() { drop(m_ptr); }

        IntrusivePtr &operator=(IntrusivePtr other) noexcept {
            swap(other);
            return *this;
        }

        T *get() const noexcept { return m_ptr; }
        T *detach() noexcept { return std::exchange(m_ptr, nullptr); }

        void swap(IntrusivePtr &other) noexcept { std::swap(m_ptr, other.m_ptr); }

        T &operator*() const noexcept {
            assert(m_ptr != nullptr);
            return *m_ptr;
        }

        T *operator->() const noexcept {
            assert(m_ptr != nullptr);
            return m_ptr;
        }

        friend void swap(IntrusivePtr &a, IntrusivePtr &b) noexcept { a.swap(b); }
        friend bool operator==(const IntrusivePtr &a, const IntrusivePtr &b) noexcept { return a.m_ptr == b.m_ptr; }
        friend bool operator==(const IntrusivePtr &a, std::nullptr_t) noexcept { return a.m_ptr == nullptr; }
        friend auto operator<=>(const IntrusivePtr &a, const IntrusivePtr &b) noexcept {
            return std::compare_three_way{}(a.m_ptr, b.m_ptr);
        }

    private:
        static void retain(T *ptr) {
            static_assert(intrusive_refcounted<T>, "T must provide add_ref and release semantics");
            if (ptr != nullptr) {
                ptr->intrusive_ptr_add_ref();
            }
        }

        static void drop(T *ptr) {
            static_assert(intrusive_refcounted<T>, "T must provide add_ref and release semantics");
            if (ptr != nullptr) {
                ptr->intrusive_ptr_release_ref();
            }
        }

        T *m_ptr = nullptr;
    };

    template <typename T, typename... Args>
    [[nodiscard]] IntrusivePtr<T> make_intrusive(Args &&...args) {
        return IntrusivePtr<T>(new T(std::forward<Args>(args)...));
    }
}

#endif //VELA_INTRUSIVE_PTR_H
