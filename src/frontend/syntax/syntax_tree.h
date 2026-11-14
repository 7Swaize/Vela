#ifndef VELA_SYNTAX_TREE_H
#define VELA_SYNTAX_TREE_H

#include "syntax_node.h"

namespace vela::parse {
    class SyntaxTree {
    public:
        SyntaxTree(const CompilationUnitSyntax *)

    private:
        const SyntaxNode *_root;
    };
}

#endif //VELA_SYNTAX_TREE_H
