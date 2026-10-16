#ifndef VELA_FACTORIES_H
#define VELA_FACTORIES_H

#include <memory>

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
} // namespace vela::shared

#endif // VELA_FACTORIES_H
