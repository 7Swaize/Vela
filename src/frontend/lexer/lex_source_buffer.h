#ifndef VELA_LEX_SOURCE_BUFFER_H
#define VELA_LEX_SOURCE_BUFFER_H

#include <memory>
#include <optional>
#include "token.h"

namespace vela::lex {
    class LexSourceBufferView {
    public:
        static constexpr LexPos kSimdPadToLargestBoundary = 32;

        static std::optional<LexSourceBufferView> load(const char *path);

        template<typename T>
            requires std::is_const_v<T>
        std::weak_ptr<T> data() const {
            return std::weak_ptr<T>(m_data);
        }

        [[nodiscard]] LexPos size() const {
            return m_size;
        }

    private:
        struct Free {
            void operator()(uint8_t *data) const {
                operator delete[](data, static_cast<std::align_val_t>(kSimdPadToLargestBoundary));
            }
        };

        std::unique_ptr<uint8_t[], Free> m_data;
        LexPos m_size = 0;
    };
} // namespace vela::lex

#endif // VELA_LEX_SOURCE_BUFFER_H
