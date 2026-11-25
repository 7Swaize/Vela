#ifndef VELA_LEX_H
#define VELA_LEX_H

#include <vector>

#include "../source_buffer.h"
#include "token.h"
#include "backend/select.h"

namespace vela::lex {
    class Lexer {
    public:
        explicit Lexer(const shared::FileSourceBuffer<backend::SimdWord> &src) : m_src(src) { }

        std::vector<Token> lex() const;
        std::vector<Token> lex_subset(LexPos start, LexPos end) const;

        const shared::FileSourceBuffer<backend::SimdWord> &src() const noexcept { return m_src; }

    private:
        const shared::FileSourceBuffer<backend::SimdWord>& m_src;
    };
} // namespace vela::lex

#endif // VELA_LEX_H
