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

    // Concept written at namespace level and, thus, is evaluated there and cannot access
    // private members of T. Having an access struct moves checking of that private member
    // to the access context of the struct, which is a friend of T (or atleast should be),
    // and is able to access private members and evaluate the 'requires' clause.
    //
    // Or we could have just used ADL.
    struct intrusive_access {
        template <typename T>
        static void add_ref(T &t) noexcept
            requires requires { { t.intrusive_ptr_add_ref() } noexcept; }
        {
            t.intrusive_ptr_add_ref();
        }

        template <typename T>
        static void release_ref(T &t) noexcept
            requires requires { { t.intrusive_ptr_release_ref() } noexcept; }
        {
            t.intrusive_ptr_release_ref();
        }
    };

    template <typename T>
    concept intrusive_refcounted = requires(T &t) {
        intrusive_access::add_ref(t);
        intrusive_access::release_ref(t);
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
        IntrusivePtr(const IntrusivePtr<U> &other) noexcept : m_ptr(other.get()) { retain(m_ptr); }

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
                intrusive_access::add_ref(*ptr);
            }
        }

        static void drop(T *ptr) {
            static_assert(intrusive_refcounted<T>, "T must provide add_ref and release semantics");
            if (ptr != nullptr) {
                intrusive_access::release_ref(*ptr);
            }
        }


        T *m_ptr = nullptr;
    };

    template <typename T, typename... Args>
    [[nodiscard]] IntrusivePtr<T> make_intrusive(Args &&...args) {
        return IntrusivePtr<T>(new T(std::forward<Args>(args)...));
    }

    template <typename T>
    [[nodiscard]] IntrusivePtr<T> adopt_intrusive(T *ptr) {
        return IntrusivePtr(ptr, adopt_ref);
    }
}

#endif //VELA_INTRUSIVE_PTR_H
