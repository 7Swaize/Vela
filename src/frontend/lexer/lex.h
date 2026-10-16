#ifndef VELA_LEX_H
#define VELA_LEX_H

#include <vector>

#include "lex_source_buffer.h"
#include "token.h"

namespace vela::lex {
    std::vector<Token> lex(const LexSourceBufferView& src);
} // namespace vela::lex

#endif // VELA_LEX_H
