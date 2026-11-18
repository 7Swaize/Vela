#ifndef VELA_SYNTAX_NODE_FWDS_H
#define VELA_SYNTAX_NODE_FWDS_H

#include "green/green_node_fwds.h"

namespace vela::parse {
    class SyntaxNode;
    class SyntaxToken;
    class SyntaxTrivia;
    class SyntaxTriviaList;

    class ListSyntax;
    class SeparatedListSyntax;
    template <typename T> class SyntaxList;
    template <typename T> class SeparatedSyntaxList;

    class MemberDeclarationSyntax;
    class TypeDeclarationSyntax;
    class StatementSyntax;
    class ExpressionSyntax;
    class TypeSyntax;

#define X(Name) class Name;
    VELA_NONTERMINAL_SYNTAX_NODE_KINDS(X)
#undef X

    class SyntaxTree;
}

#endif //VELA_SYNTAX_NODE_FWDS_H
