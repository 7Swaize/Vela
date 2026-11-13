#ifndef VELA_HELPERS_H
#define VELA_HELPERS_H

#include <format>
#include <memory>
#include <ranges>

#include "preprocessor.h"

namespace vela::shared {
    namespace detail {
        template <typename T>
        std::string stringify(T&& t) {
            using std::to_string;
            return to_string(std::forward<T>(t));
        }
    } // namespace detail

    // clang-format off
    template <typename T, typename Deleter, typename... Args>
    VELA_FORCE_INLINE std::unique_ptr<T, Deleter> make_unique_with_deleter(Args &&...args) {
        return std::unique_ptr<T, Deleter>(new T(std::forward<Args>(args)...));
    }

    template <typename T, typename Deleter>
    VELA_FORCE_INLINE std::unique_ptr<T, Deleter> adopt_unique_with_deleter(
        typename std::unique_ptr<T, Deleter>::pointer ptr,
        Deleter &&deleter)
    {
        return std::unique_ptr<T, Deleter>(ptr, std::forward<Deleter>(deleter));
    }

    template <std::ranges::input_range T>
    std::string collection_to_string(T&& collection) {
        auto strings = std::forward<T>(collection)
            | std::ranges::views::transform([](const auto& a) {
                return detail::stringify(a);
            })
            | std::ranges::views::join_with(std::string(", "));

        return "[ " + std::ranges::to<std::string>(strings) + " ]";
    }

    template <typename... Args>
    std::string runtime_format(const std::string_view dynamic_fmt, Args &&...args) {
        return std::vformat(dynamic_fmt, std::make_format_args(args...));
    }

    template <typename T>
    std::size_t collection_size_or_zero(const std::optional<T> &value) {
        return value.has_value() ? value->size() : 0;
    }
    // clang-format on
} // namespace vela::shared

#endif // VELA_HELPERS_H
