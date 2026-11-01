#ifndef VELA_DIAGNOSTIC_H
#define VELA_DIAGNOSTIC_H

#include <algorithm>
#include <ranges>
#include <vector>

#include "frontend/lexer/token.h"

namespace vela::parse {
    struct Note {
        lex::LexPos offset;
        lex::LexPos length;
        std::string message;
    };

    struct Diagnostic {
        lex::LexPos offset;
        lex::LexPos length;
        std::string message;
        std::optional<Note> note;
    };

    struct Location {
        lex::LexPos line;
        lex::LexPos column;
    };

    class LineIndex {
    public:
        explicit LineIndex(const std::string_view text) : m_text(text) {
            m_starts.push_back(0);
            for (std::size_t i = 0; i < text.size(); ++i) {
                if (text[i] == '\n') {
                    m_starts.push_back(static_cast<lex::LexPos>(i + 1));
                }
            }
        }

        Location locate(lex::LexPos offset) const {
            offset = std::min(offset, static_cast<lex::LexPos>(m_starts.size()));
            const auto it = std::ranges::upper_bound(m_starts, offset);
            const auto line = static_cast<lex::LexPos>(it - m_starts.begin());
            return {
                .line = line,
                .column = offset - m_starts[line - 1]
            };
        }

    private:
        std::string_view m_text;
        std::vector<lex::LexPos> m_starts;
    };

    inline std::string render(const Diagnostic &diagnostic, const std::string_view file) {

    }
} // namespace vela::parse

#endif //VELA_DIAGNOSTIC_H
