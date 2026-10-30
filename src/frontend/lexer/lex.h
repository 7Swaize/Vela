#ifndef VELA_LEX_H
#define VELA_LEX_H

#include <vector>

#include "../source_buffer.h"
#include "token.h"
#include "backend/select.h"

namespace vela::lex {
    std::vector<Token> lex(const shared::FileSourceBuffer<backend::SimdWord>& src);
} // namespace vela::lex

#endif // VELA_LEX_H
