#ifndef VELA_GREEN_NODE_FWDS_H
#define VELA_GREEN_NODE_FWDS_H

#include <cstdint>

#include "shared/intrusive_ptr.h"

namespace vela::parse {
#define VELA_TRIVIA_SYNTAX_NODE_KINDS(X)       \
    X(NewlineTrivia)                           \
    X(SpacesTrivia)                            \
    X(TabsTrivia)                              \
    X(CommentTrivia)                           \
    X(MultilineCommentTrivia)                  \
    X(UnexpectedToken)

#define VELA_TERMINAL_SYNTAX_NODE_KINDS(X)     \
    X(EofToken)                                \
    X(IdentifierToken)                         \
    X(IntLiteralToken)                         \
    X(FloatLiteralToken)                       \
    X(StringLiteralToken)                      \
    X(KwComptimeToken)                         \
    X(KwConcept)                               \
    X(KwMatch)                                 \
    X(KwOperator)                              \
    X(KwStructToken)                           \
    X(KwClassToken)                            \
    X(KwFnToken)                               \
    X(KwIfToken)                               \
    X(KwElseToken)                             \
    X(KwWhileToken)                            \
    X(KwForToken)                              \
    X(KwReturnToken)                           \
    X(KwTrueToken)                             \
    X(KwFalseToken)                            \
    X(KwNullToken)                             \
    X(KwThisToken)                             \
    X(KwNewToken)                              \
    X(KwVoidToken)                             \
    X(KwIntToken)                              \
    X(KwFloatToken)                            \
    X(KwBoolToken)                             \
    X(KwStringToken)                           \
    X(LParenToken)                             \
    X(RParenToken)                             \
    X(LBracketToken)                           \
    X(RBracketToken)                           \
    X(LBraceToken)                             \
    X(RBraceToken)                             \
    X(CommaToken)                              \
    X(DotToken)                                \
    X(SemicolonToken)                          \
    X(PlusToken)                               \
    X(MinusToken)                              \
    X(StarToken)                               \
    X(SlashToken)                              \
    X(DollarToken)                             \
    X(PercentToken)                            \
    X(ExclToken)                               \
    X(TildeToken)                              \
    X(AmpToken)                                \
    X(PipeToken)                               \
    X(CaretToken)                              \
    X(LtToken)                                 \
    X(GtToken)                                 \
    X(AssignToken)                             \
    X(AmpAmpToken)                             \
    X(PipePipeToken)                           \
    X(EqEqToken)                               \
    X(ExclEqToken)                             \
    X(LtEqToken)                               \
    X(GtEqToken)                               \
    X(LtLtToken)                               \
    X(GtGtToken)                               \
    X(DotDotToken)                             \
    X(DotDotDotToken)                          \
    X(ArrowToken)                              \
    X(FatArrowToken)                           \
    X(ErrorToken)                              \
    X(MissingToken)                            \

#define VELA_NONTERMINAL_SYNTAX_NODE_KINDS(X)  \
    X(CompilationUnitSyntax)                   \
    X(ClassDeclarationSyntax)                  \
    X(StructDeclarationSyntax)                 \
    X(FieldDeclarationSyntax)                  \
    X(MethodDeclarationSyntax)                 \
    X(ParameterSyntax)                         \
    X(VariableDeclarationSyntax)               \
    X(PredefinedTypeSyntax)                    \
    X(IdentifierNameSyntax)                    \
    X(ArrayTypeSyntax)                         \
    X(BlockSyntax)                             \
    X(LocalDeclarationStatementSyntax)         \
    X(ExpressionStatementSyntax)               \
    X(IfStatementSyntax)                       \
    X(WhileStatementSyntax)                    \
    X(ForStatementSyntax)                      \
    X(ReturnStatementSyntax)                   \
    X(LiteralExpressionSyntax)                 \
    X(ThisExpressionSyntax)                    \
    X(PrefixUnaryExpressionSyntax)             \
    X(PostfixUnaryExpressionSyntax)            \
    X(BinaryExpressionSyntax)                  \
    X(AssignmentExpressionSyntax)              \
    X(InvocationExpressionSyntax)              \
    X(ElementAccessExpressionSyntax)           \
    X(ObjectCreationExpressionSyntax)          \
    X(InitializerExpressionSyntax)             \
    X(ArrayCreationExpressionSyntax)           \
    X(ConceptDeclarationSyntax)                \
    X(TemplateParameterListSyntax)             \
    X(TemplateParameterSyntax)                 \
    X(TemplateArgumentListSyntax)              \
    X(TemplateArgumentSyntax)                  \
    X(ConstraintClauseSyntax)                  \
    X(RequirementBlockSyntax)                  \
    X(RequirementSyntax)                       \
    X(ArrowExpressionClauseSyntax)             \
    X(TemplateNameSyntax)                      \
    X(PackExpansionSyntax)                     \
    X(TypeVariableSyntax)                      \
    X(MemberAccessExpressionSyntax)            \
    X(LeftFoldExpressionSyntax)                \
    X(RightFoldExpressionSyntax)               \
    X(OperatorReferenceExpressionSyntax)       \
    X(ComptimeMatchExpressionSyntax)           \
    X(ComptimeIfStatementSyntax)               \
    X(ComptimeForStatementSyntax)              \
    X(ComptimeBlockStatementSyntax)            \
    X(QualifiedNameSyntax)                     \
    X(PackIndexTypeSyntax)                     \
    X(AssociatedTypeRequirementSyntax)         \
    X(MemberFunctionRequirementSyntax)         \
    X(RangeExpressionSyntax)                   \
    X(PackLiteralExpressionSyntax)             \
    X(ComptimeVariableDeclarationSyntax)       \
    X(ComptimeMatchArmSyntax)                  \
    X(UndefinedSyntax)                         \

#define VELA_LIST_SYNTAX_NODE_KINDS(X)         \
    X(ListSyntax)                              \
    X(SeparatedListSyntax)                     \

#define VELA_NODE_KINDS(X)                     \
    VELA_TRIVIA_SYNTAX_NODE_KINDS(X)           \
    VELA_TERMINAL_SYNTAX_NODE_KINDS(X)         \
    VELA_NONTERMINAL_SYNTAX_NODE_KINDS(X)      \
    VELA_LIST_SYNTAX_NODE_KINDS(X)             \

    enum class NodeKind : uint16_t;
    using NodePos = uint32_t;

    class GreenNode;
    class GreenTrivia;
    class GreenToken;
    class GreenNodeList;

    template <int N>
    class GreenNodeN;

#define X(Kind) class Green##Kind;
    VELA_NONTERMINAL_SYNTAX_NODE_KINDS(X)
#undef X

    template <typename T>
    using GreenRef = shared::IntrusivePtr<T>;
}

#endif //VELA_GREEN_NODE_FWDS_H
