#ifndef VELA_TYPING_H
#define VELA_TYPING_H

#include <concepts>
#include <cstdint>

// clang-format off

template <typename T, typename... Ts>
concept OneOf = (std::same_as<T, Ts> || ...);

template <typename T>
concept FixedUnsignedInteger =
    OneOf<std::remove_cvref_t<T>, uint8_t, uint16_t, uint32_t, uint64_t>;

// clang-format on

#endif // VELA_TYPING_H
