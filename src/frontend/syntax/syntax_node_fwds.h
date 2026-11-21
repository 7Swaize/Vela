#ifndef VELA_SYNTAX_NODE_FWDS_H
#define VELA_SYNTAX_NODE_FWDS_H

#include "green/green_node_fwds.h"

namespace vela::parse {
    class SyntaxElement;
    class SyntaxNode;
    class SyntaxToken;
    struct SyntaxTrivia;
    class SyntaxTriviaList;

    template <typename T>
    concept RequiresSyntaxElement = std::derived_from<T, SyntaxElement>;

    template <typename T>
    concept RequiresSyntaxNode = std::derived_from<T, SyntaxNode>;

    class ListSyntax;
    class SeparatedListSyntax;
    template <RequiresSyntaxElement T> class SyntaxList;
    template <RequiresSyntaxElement T> class SeparatedSyntaxList;

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
