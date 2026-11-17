#ifndef VELA_SYNTAX_NODE_FACTORY_H
#define VELA_SYNTAX_NODE_FACTORY_H

#include "syntax_node.h"
#include "syntax_node_concrete.h"
#include "syntax_tree.h"
#include "green/green_node.h"

namespace vela::parse {
    inline const SyntaxNode *create_syntax_node(const GreenNode *green, const SyntaxNode *parent, SyntaxTree *tree, const NodePos position) {
        switch (green->kind()) {
#define X(Kind) case NodeKind::Kind:
            VELA_TERMINAL_SYNTAX_NODE_KINDS(X)
#undef X
            return tree->alloc_node<SyntaxToken>(green, parent, tree, position);

#define X(Kind) case NodeKind::Kind: return tree->alloc_node<Kind>(green, parent, tree, position);;
            VELA_NONTERMINAL_SYNTAX_NODE_KINDS(X)
            VELA_LIST_SYNTAX_NODE_KINDS(X)
#undef X

#define X(Kind) case NodeKind::Kind:
            VELA_TRIVIA_SYNTAX_NODE_KINDS(X);
#undef X
            break;
        }

        return tree->alloc_node<UndefinedSyntax>(green, parent, tree, position);
    }
}

#endif //VELA_SYNTAX_NODE_FACTORY_H
