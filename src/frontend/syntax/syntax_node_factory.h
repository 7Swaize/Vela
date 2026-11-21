#ifndef VELA_SYNTAX_NODE_FACTORY_H
#define VELA_SYNTAX_NODE_FACTORY_H

#include "syntax_node.h"
#include "syntax_node_concrete.h"
#include "syntax_tree.h"
#include "green/green_node.h"

namespace vela::parse {
    inline const SyntaxElement *create_syntax_element(const SyntaxElementAllocArgs &args) {
        switch (std::move(args).green->kind()) {
#define X(Kind) case NodeKind::Kind:
            VELA_TERMINAL_SYNTAX_NODE_KINDS(X)
#undef X
            return const_cast<SyntaxTree *>(args.tree)->alloc_token(args);

#define X(Kind) case NodeKind::Kind: return const_cast<SyntaxTree *>(args.tree)->alloc_node<Kind>(args);
            VELA_NONTERMINAL_SYNTAX_NODE_KINDS(X)
            VELA_LIST_SYNTAX_NODE_KINDS(X)
#undef X

#define X(Kind) case NodeKind::Kind:
            VELA_TRIVIA_SYNTAX_NODE_KINDS(X);
#undef X
            break;
        }

        return const_cast<SyntaxTree *>(args.tree)->alloc_node<UndefinedSyntax>(args);
    }
}

#endif //VELA_SYNTAX_NODE_FACTORY_H
