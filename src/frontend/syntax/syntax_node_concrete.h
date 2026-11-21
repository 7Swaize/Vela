#ifndef VELA_SYNTAX_NODE_CONCRETE_H
#define VELA_SYNTAX_NODE_CONCRETE_H

#include "syntax_node.h"
#include "syntax_node_fwds.h"

#define VELA_SYNTAX_NODE_CTOR(Name, Base)                                               \
    Name(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *tree,      \
         const NodePos position, const int index, const SyntaxElement **slots)     \
            : Base(green, parent, tree, position, index, slots) { }

namespace vela::parse {
    class MemberDeclarationSyntax : public SyntaxNode {
    protected:
        VELA_SYNTAX_NODE_CTOR(MemberDeclarationSyntax, SyntaxNode)
    };

    class StatementSyntax : public SyntaxNode {
    protected:
        VELA_SYNTAX_NODE_CTOR(StatementSyntax, SyntaxNode)
    };

    class ExpressionSyntax : public SyntaxNode {
    protected:
        VELA_SYNTAX_NODE_CTOR(ExpressionSyntax, SyntaxNode)
    };

    class TypeSyntax : public ExpressionSyntax {
    protected:
        VELA_SYNTAX_NODE_CTOR(TypeSyntax, ExpressionSyntax);
    };

    class PredefinedTypeSyntax final : public TypeSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(PredefinedTypeSyntax, TypeSyntax);

        const SyntaxToken *type() const { return getRequiredNode<SyntaxToken>(0); }
    };

    class IdentifierNameSyntax final : public TypeSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(IdentifierNameSyntax, TypeSyntax);

        const SyntaxToken *identifier() const { return getRequiredNode<SyntaxToken>(0); }
    };

    class ArrayTypeSyntax final : public TypeSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(ArrayTypeSyntax, TypeSyntax);

        const TypeSyntax *elementType() const { return getRequiredNode<TypeSyntax>(0); }
        const SyntaxToken *lbracket() const { return getRequiredNode<SyntaxToken>(1); }
        const SyntaxToken *rbracket() const { return getRequiredNode<SyntaxToken>(2); }
    };

    class LiteralExpressionSyntax final : public ExpressionSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(LiteralExpressionSyntax, ExpressionSyntax);

        const SyntaxToken *literal() const { return getRequiredNode<SyntaxToken>(0); }
    };

    class ThisExpressionSyntax final : public ExpressionSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(ThisExpressionSyntax, ExpressionSyntax);

        const SyntaxToken *kwThis() const { return getRequiredNode<SyntaxToken>(0); }
    };

    class PrefixUnaryExpressionSyntax final : public ExpressionSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(PrefixUnaryExpressionSyntax, ExpressionSyntax);

        const SyntaxToken *op() const { return getRequiredNode<SyntaxToken>(0); }
        const ExpressionSyntax *operand() const { return getRequiredNode<ExpressionSyntax>(1); }
    };

    class PostfixUnaryExpressionSyntax final : public ExpressionSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(PostfixUnaryExpressionSyntax, ExpressionSyntax);

        const SyntaxToken *op() const { return getRequiredNode<SyntaxToken>(0); }
        const ExpressionSyntax *operand() const { return getRequiredNode<ExpressionSyntax>(1); }
    };

    class BinaryExpressionSyntax final : public ExpressionSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(BinaryExpressionSyntax, ExpressionSyntax);

        const ExpressionSyntax *lhs() const { return getRequiredNode<ExpressionSyntax>(0); }
        const SyntaxToken *op() const { return getRequiredNode<SyntaxToken>(1); }
        const ExpressionSyntax *rhs() const { return getRequiredNode<ExpressionSyntax>(2); }
    };

    class AssignmentExpressionSyntax final : public ExpressionSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(AssignmentExpressionSyntax, ExpressionSyntax);

        const ExpressionSyntax *lhs() const { return getRequiredNode<ExpressionSyntax>(0); }
        const SyntaxToken *op() const { return getRequiredNode<SyntaxToken>(1); }
        const ExpressionSyntax *rhs() const { return getRequiredNode<ExpressionSyntax>(2); }
    };

    class InvocationExpressionSyntax final : public ExpressionSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(InvocationExpressionSyntax, ExpressionSyntax);

        const ExpressionSyntax *callee() const { return getRequiredNode<ExpressionSyntax>(0); }
        const SyntaxToken *lparen() const { return getRequiredNode<SyntaxToken>(1); }
        SeparatedSyntaxList<ExpressionSyntax> arguments() const {
            return SeparatedSyntaxList<ExpressionSyntax>(getOptionalNode<SeparatedListSyntax>(2));
        }
        const SyntaxToken *rparen() const { return getRequiredNode<SyntaxToken>(3); }
    };

    class ElementAccessExpressionSyntax final : public ExpressionSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(ElementAccessExpressionSyntax, ExpressionSyntax);

        const ExpressionSyntax *target() const { return getRequiredNode<ExpressionSyntax>(0); }
        const SyntaxToken *lbracket() const { return getRequiredNode<SyntaxToken>(1); }
        const ExpressionSyntax *index() const { return getRequiredNode<ExpressionSyntax>(2); }
        const SyntaxToken *rbracket() const { return getRequiredNode<SyntaxToken>(3); }
    };

    class InitializerExpressionSyntax final : public ExpressionSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(InitializerExpressionSyntax, ExpressionSyntax);

        const SyntaxToken *lbrace() const { return getRequiredNode<SyntaxToken>(0); }
        SeparatedSyntaxList<ExpressionSyntax> expressions() const {
            return SeparatedSyntaxList<ExpressionSyntax>(getOptionalNode<SeparatedListSyntax>(1));
        }
        const SyntaxToken *rbrace() const { return getRequiredNode<SyntaxToken>(2); }
    };

    class ObjectCreationExpressionSyntax final : public ExpressionSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(ObjectCreationExpressionSyntax, ExpressionSyntax);

        const SyntaxToken *kwNew() const { return getRequiredNode<SyntaxToken>(0); }
        const TypeSyntax *targetType() const { return getRequiredNode<TypeSyntax>(1); }
        const SyntaxToken *lparen() const { return getRequiredNode<SyntaxToken>(2); }
        SeparatedSyntaxList<ExpressionSyntax> arguments() const {
            return SeparatedSyntaxList<ExpressionSyntax>(getOptionalNode<SeparatedListSyntax>(3));
        }
        const SyntaxToken *rparen() const { return getRequiredNode<SyntaxToken>(4); }
    };

    class ArrayCreationExpressionSyntax final : public ExpressionSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(ArrayCreationExpressionSyntax, ExpressionSyntax);

        const SyntaxToken *kwNew() const { return getRequiredNode<SyntaxToken>(0); }
        const TypeSyntax *targetType() const { return getRequiredNode<TypeSyntax>(1); }
        const SyntaxToken *lbracket() const { return getRequiredNode<SyntaxToken>(2); }
        const ExpressionSyntax *size() const { return getRequiredNode<ExpressionSyntax>(3); }
        const SyntaxToken *rbracket() const { return getRequiredNode<SyntaxToken>(4); }
        const InitializerExpressionSyntax *initializer() const { return getOptionalNode<InitializerExpressionSyntax>(5); }
    };

    class VariableDeclarationSyntax final : public SyntaxNode {
    public:
        VELA_SYNTAX_NODE_CTOR(VariableDeclarationSyntax, SyntaxNode);

        const TypeSyntax *type() const { return getRequiredNode<TypeSyntax>(0); }
        const SyntaxToken *identifier() const { return getRequiredNode<SyntaxToken>(1); }
        const SyntaxToken *assignmentToken() const { return getOptionalNode<SyntaxToken>(2); }
        const ExpressionSyntax *initializer() const { return getOptionalNode<ExpressionSyntax>(3); }
    };

    class BlockSyntax final : public StatementSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(BlockSyntax, StatementSyntax);

        const SyntaxToken *lbrace() const { return getRequiredNode<SyntaxToken>(0); }
        SyntaxList<StatementSyntax> statements() const {
            return SyntaxList<StatementSyntax>(getOptionalNode<ListSyntax>(1));
        }
        SyntaxList<MemberDeclarationSyntax> declarations() const {
            return SyntaxList<MemberDeclarationSyntax>(getOptionalNode<ListSyntax>(2));
        }
        const SyntaxToken *rparen() const { return getRequiredNode<SyntaxToken>(3); }
    };

    class LocalDeclarationStatementSyntax final : public StatementSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(LocalDeclarationStatementSyntax, StatementSyntax);

        const VariableDeclarationSyntax *declaration() const { return getRequiredNode<VariableDeclarationSyntax>(0); }
        const SyntaxToken *semicolon() const { return getRequiredNode<SyntaxToken>(1); }
    };

    class ExpressionStatementSyntax final : public StatementSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(ExpressionStatementSyntax, StatementSyntax);

        const ExpressionSyntax *expression() const { return getRequiredNode<ExpressionSyntax>(0); }
        const SyntaxToken *semicolon() const { return getRequiredNode<SyntaxToken>(1); }
    };

    class IfStatementSyntax final : public StatementSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(IfStatementSyntax, StatementSyntax);

        const SyntaxToken *kwIf() const { return getRequiredNode<SyntaxToken>(0); }
        const SyntaxToken *lparen() const { return getRequiredNode<SyntaxToken>(1); }
        const ExpressionSyntax *condition() const { return getRequiredNode<ExpressionSyntax>(2); }
        const SyntaxToken *rparen() const { return getRequiredNode<SyntaxToken>(3); }
        const StatementSyntax *statementIf() const { return getRequiredNode<StatementSyntax>(4); }
        const SyntaxToken *kwElse() const { return getOptionalNode<SyntaxToken>(5); }
        const StatementSyntax *statementElse() const { return getOptionalNode<StatementSyntax>(6); }
    };

    class WhileStatementSyntax final : public StatementSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(WhileStatementSyntax, StatementSyntax);

        const SyntaxToken *kwWhile() const { return getRequiredNode<SyntaxToken>(0); }
        const SyntaxToken *lparen() const { return getRequiredNode<SyntaxToken>(1); }
        const ExpressionSyntax *condition() const { return getRequiredNode<ExpressionSyntax>(2); }
        const SyntaxToken *rparen() const { return getRequiredNode<SyntaxToken>(3); }
        const StatementSyntax *statement() const { return getRequiredNode<StatementSyntax>(4); }
    };

    class ForStatementSyntax final : public StatementSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(ForStatementSyntax, StatementSyntax);

        const SyntaxToken *kwFor() const { return getRequiredNode<SyntaxToken>(0); }
        const SyntaxToken *lparen() const { return getRequiredNode<SyntaxToken>(1); }
        const VariableDeclarationSyntax *initializer() const { return getOptionalNode<VariableDeclarationSyntax>(2); }
        SeparatedSyntaxList<VariableDeclarationSyntax> initializers() const {
            return SeparatedSyntaxList<VariableDeclarationSyntax>(getOptionalNode<SeparatedListSyntax>(3));
        }
        const SyntaxToken *semicolon1() const { return getRequiredNode<SyntaxToken>(4); }
        const ExpressionSyntax *condition() const { return getOptionalNode<ExpressionSyntax>(5); }
        const SyntaxToken *semicolon2() const { return getRequiredNode<SyntaxToken>(6); }
        SeparatedSyntaxList<ExpressionSyntax> incrementors() const {
            return SeparatedSyntaxList<ExpressionSyntax>(getOptionalNode<SeparatedListSyntax>(7));
        }
        const SyntaxToken *rparen() const { return getRequiredNode<SyntaxToken>(8); }
        const StatementSyntax *statement() const { return getRequiredNode<StatementSyntax>(9); }
    };

    class ReturnStatementSyntax final : public StatementSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(ReturnStatementSyntax, StatementSyntax);

        const SyntaxToken *kwReturn() const { return getRequiredNode<SyntaxToken>(0); }
        const ExpressionSyntax *returnValue() const { return getOptionalNode<ExpressionSyntax>(1); }
        const SyntaxToken *semicolon() const { return getRequiredNode<SyntaxToken>(2); }
    };

    class ParameterSyntax final : public SyntaxNode {
    public:
        VELA_SYNTAX_NODE_CTOR(ParameterSyntax, SyntaxNode);

        const TypeSyntax *type() const { return getRequiredNode<TypeSyntax>(0); }
        const SyntaxToken *identifier() const { return getRequiredNode<SyntaxToken>(1); }
    };

    class FieldDeclarationSyntax final : public MemberDeclarationSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(FieldDeclarationSyntax, MemberDeclarationSyntax);

        const VariableDeclarationSyntax *declaration() const { return getRequiredNode<VariableDeclarationSyntax>(0); }
        const SyntaxToken *semicolon() const { return getRequiredNode<SyntaxToken>(1); }
    };

    class MethodDeclarationSyntax final : public MemberDeclarationSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(MethodDeclarationSyntax, MemberDeclarationSyntax);

        const SyntaxToken *kwFn() const { return getRequiredNode<SyntaxToken>(0); }
        const TypeSyntax *returnType() const { return getRequiredNode<TypeSyntax>(1); }
        const SyntaxToken *identifier() const { return getRequiredNode<SyntaxToken>(2); }
        const SyntaxToken *lparen() const { return getRequiredNode<SyntaxToken>(3); }
        SeparatedSyntaxList<ParameterSyntax> parameters() const {
            return SeparatedSyntaxList<ParameterSyntax>(getOptionalNode<SeparatedListSyntax>(4));
        }
        const SyntaxToken *rparen() const { return getRequiredNode<SyntaxToken>(5); }
        const BlockSyntax *block() const { return getRequiredNode<BlockSyntax>(6); }
    };

    class TypeDeclarationSyntax : public MemberDeclarationSyntax {
    public:
        const SyntaxToken *kw() const { return getRequiredNode<SyntaxToken>(0); }
        const SyntaxToken *identifier() const { return getRequiredNode<SyntaxToken>(1); }
        const BlockSyntax *block() const { return getRequiredNode<BlockSyntax>(2); }

    protected:
        VELA_SYNTAX_NODE_CTOR(TypeDeclarationSyntax, MemberDeclarationSyntax);
    };

    class ClassDeclarationSyntax final : public TypeDeclarationSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(ClassDeclarationSyntax, TypeDeclarationSyntax);
    };

    class StructDeclarationSyntax final : public TypeDeclarationSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(StructDeclarationSyntax, TypeDeclarationSyntax);
    };

    class CompilationUnitSyntax final : public SyntaxNode {
    public:
        VELA_SYNTAX_NODE_CTOR(CompilationUnitSyntax, SyntaxNode);

        SyntaxList<MemberDeclarationSyntax> declarations() const {
            return SyntaxList<MemberDeclarationSyntax>(getOptionalNode<ListSyntax>(0));
        }
        const SyntaxToken *eof() const { return getRequiredNode<SyntaxToken>(1); }
    };

    class UndefinedSyntax final : public SyntaxNode {
    public:
        VELA_SYNTAX_NODE_CTOR(UndefinedSyntax, SyntaxNode);
    };
}

#undef VELA_SYNTAX_NODE_CTOR

#endif //VELA_SYNTAX_NODE_CONCRETE_H