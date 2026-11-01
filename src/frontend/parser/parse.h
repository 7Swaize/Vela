#ifndef VELA_PARSE_H
#define VELA_PARSE_H

#include "ast.h"
#include "diagnostic.h"
#include "shared/helpers.h"

namespace vela::parse {
    struct ParseResult {
        ast::Program *program;
        lex::LexPos nodeCount;
        std::vector<Diagnostic> diagnostics;

        bool ok() const {
            return diagnostics.empty();
        }
    };

    ParseResult parse(std::span<const lex::Token> tokens, shared::ArenaAllocator &arena);
} // namespace vela::parse

#endif //VELA_PARSE_H
