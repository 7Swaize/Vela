#ifndef VELA_LEX_H
#define VELA_LEX_H

#include <vector>

#include "../../shared/source_buffer.h"
#include "token.h"

namespace vela::lex {
    std::vector<Token> lex(const PaddedSourceBufferView& src);
} // namespace vela::lex

#endif // VELA_LEX_H
