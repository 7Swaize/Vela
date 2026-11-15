#ifndef VELA_SYNTAX_NODE_FACTORY_H
#define VELA_SYNTAX_NODE_FACTORY_H

#include <utility>

#include "syntax_node.h"
#include "syntax_tree.h"

namespace vela::parse {
    SyntaxNode *create_syntax_node(const SyntaxTree *tree, const SyntaxNode* parent, const GreenNode *green, int position) {
        if (green->isToken()) {
            // create token
        }

        // switch on kinds with x macro
        std::unreachable();
    }
}

#endif //VELA_SYNTAX_NODE_FACTORY_H
