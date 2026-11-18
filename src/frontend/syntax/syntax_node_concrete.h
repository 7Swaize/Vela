#ifndef VELA_SYNTAX_NODE_CONCRETE_H
#define VELA_SYNTAX_NODE_CONCRETE_H

#include "syntax_node.h"
#include "syntax_node_fwds.h"

#define VELA_SYNTAX_NODE_CTOR(Name, Base)                                                                   \
    Name(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *tree, const NodePos position)  \
        : Base(green, parent, tree, position) { }

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

        const SyntaxToken *type() { return getRequiredNode<SyntaxToken>(m_type, 0); }

    private:
        const SyntaxToken *m_type = nullptr;
    };

    class IdentifierNameSyntax final : public TypeSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(IdentifierNameSyntax, TypeSyntax);

        const SyntaxToken *identifier() { return getRequiredNode<SyntaxToken>(m_ident, 0); }

    private:
        const SyntaxToken *m_ident = nullptr;
    };

    class ArrayTypeSyntax final : public TypeSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(ArrayTypeSyntax, TypeSyntax);

        const TypeSyntax *elementType() { return getRequiredNode<TypeSyntax>(m_elementType, 0); }
        const SyntaxToken *lbracket() { return getRequiredNode<SyntaxToken>(m_lbracket, 1); }
        const SyntaxToken *rbracket() { return getRequiredNode<SyntaxToken>(m_rbracket, 2); }

    private:
        const TypeSyntax *m_elementType = nullptr;
        const SyntaxToken *m_lbracket = nullptr;
        const SyntaxToken *m_rbracket = nullptr;
    };

    class LiteralExpressionSyntax final : public ExpressionSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(LiteralExpressionSyntax, ExpressionSyntax);

        const SyntaxToken *literal() { return getRequiredNode<SyntaxToken>(m_literal, 0); }

    private:
        const SyntaxToken *m_literal = nullptr;
    };

    class ThisExpressionSyntax final : public ExpressionSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(ThisExpressionSyntax, ExpressionSyntax);

        const SyntaxToken *kwThis() { return getRequiredNode<SyntaxToken>(m_kwThis, 0); }

    private:
        const SyntaxToken *m_kwThis = nullptr;
    };

    class PrefixUnaryExpressionSyntax final : public ExpressionSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(PrefixUnaryExpressionSyntax, ExpressionSyntax);

        const SyntaxToken *op() { return getRequiredNode<SyntaxToken>(m_op, 0); }
        const ExpressionSyntax *operand() { return getRequiredNode<ExpressionSyntax>(m_operand, 1); }

    private:
        const SyntaxToken *m_op = nullptr;
        const ExpressionSyntax *m_operand = nullptr;
    };

    class PostfixUnaryExpressionSyntax final : public ExpressionSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(PostfixUnaryExpressionSyntax, ExpressionSyntax);

        const SyntaxToken *op() { return getRequiredNode<SyntaxToken>(m_op, 0); }
        const ExpressionSyntax *operand() { return getRequiredNode<ExpressionSyntax>(m_operand, 1); }

    private:
        const SyntaxToken *m_op = nullptr;
        const ExpressionSyntax *m_operand = nullptr;
    };

    class BinaryExpressionSyntax final : public ExpressionSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(BinaryExpressionSyntax, ExpressionSyntax);

        const ExpressionSyntax *lhs() { return getRequiredNode<ExpressionSyntax>(m_lhs, 0); }
        const SyntaxToken *op() { return getRequiredNode<SyntaxToken>(m_op, 1); }
        const ExpressionSyntax *rhs() { return getRequiredNode<ExpressionSyntax>(m_rhs, 2); }

    private:
        const ExpressionSyntax *m_lhs = nullptr;
        const SyntaxToken *m_op = nullptr;
        const ExpressionSyntax *m_rhs = nullptr;
    };

    class AssignmentExpressionSyntax final : public ExpressionSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(AssignmentExpressionSyntax, ExpressionSyntax);

        const ExpressionSyntax *lhs() { return getRequiredNode<ExpressionSyntax>(m_lhs, 0); }
        const SyntaxToken *op() { return getRequiredNode<SyntaxToken>(m_op, 1); }
        const ExpressionSyntax *rhs() { return getRequiredNode<ExpressionSyntax>(m_rhs, 2); }

    private:
        const ExpressionSyntax *m_lhs = nullptr;
        const SyntaxToken *m_op = nullptr;
        const ExpressionSyntax *m_rhs = nullptr;
    };

    class InvocationExpressionSyntax final : public ExpressionSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(InvocationExpressionSyntax, ExpressionSyntax);

        const ExpressionSyntax *callee() { return getRequiredNode<ExpressionSyntax>(m_callee, 0); }
        const SyntaxToken *lparen() { return getRequiredNode<SyntaxToken>(m_lparen, 1); }
        SeparatedSyntaxList<ExpressionSyntax> arguments() {
            return SeparatedSyntaxList<ExpressionSyntax>(getOptionalNode<SeparatedListSyntax>(m_arguments, 2));
        }
        const SyntaxToken *rparen() { return getRequiredNode<SyntaxToken>(m_rparen, 3); }

    private:
        const ExpressionSyntax *m_callee = nullptr;
        const SyntaxToken *m_lparen = nullptr;
        const SeparatedListSyntax *m_arguments = nullptr;
        const SyntaxToken *m_rparen = nullptr;
    };

    class ElementAccessExpressionSyntax final : public ExpressionSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(ElementAccessExpressionSyntax, ExpressionSyntax);

        const ExpressionSyntax *target() { return getRequiredNode<ExpressionSyntax>(m_target, 0); }
        const SyntaxToken *lbracket() { return getRequiredNode<SyntaxToken>(m_lbracket, 1); }
        const ExpressionSyntax *index() { return getRequiredNode<ExpressionSyntax>(m_index, 2); }
        const SyntaxToken *rbracket() { return getRequiredNode<SyntaxToken>(m_rbracket, 3); }

    private:
        const ExpressionSyntax *m_target = nullptr;
        const SyntaxToken *m_lbracket = nullptr;
        const ExpressionSyntax *m_index = nullptr;
        const SyntaxToken *m_rbracket = nullptr;
    };

    class InitializerExpressionSyntax final : public ExpressionSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(InitializerExpressionSyntax, ExpressionSyntax);

        const SyntaxToken *lbrace() { return getRequiredNode<SyntaxToken>(m_lbrace, 0); }
        SeparatedSyntaxList<ExpressionSyntax> expressions() {
            return SeparatedSyntaxList<ExpressionSyntax>(getOptionalNode<SeparatedListSyntax>(m_expressions, 1));
        }
        const SyntaxToken *rbrace() { return getRequiredNode<SyntaxToken>(m_rbrace, 2); }

    private:
        const SyntaxToken *m_lbrace = nullptr;
        const SeparatedListSyntax *m_expressions = nullptr;
        const SyntaxToken *m_rbrace = nullptr;
    };

    class ObjectCreationExpressionSyntax final : public ExpressionSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(ObjectCreationExpressionSyntax, ExpressionSyntax);

        const SyntaxToken *kwNew() { return getRequiredNode<SyntaxToken>(m_kwNew, 0); }
        const TypeSyntax *targetType() { return getRequiredNode<TypeSyntax>(m_targetType, 1); }
        const SyntaxToken *lparen() { return getRequiredNode<SyntaxToken>(m_lbrace, 2); }
        SeparatedSyntaxList<ExpressionSyntax> arguments() {
            return SeparatedSyntaxList<ExpressionSyntax>(getOptionalNode<SeparatedListSyntax>(m_arguments, 3));
        }
        const SyntaxToken *rparen() { return getRequiredNode<SyntaxToken>(m_rparen, 4); }

    private:
        const SyntaxToken *m_kwNew = nullptr;
        const TypeSyntax *m_targetType = nullptr;
        const SyntaxToken *m_lbrace = nullptr;
        const SeparatedListSyntax *m_arguments = nullptr;
        const SyntaxToken *m_rparen = nullptr;
    };

    class ArrayCreationExpressionSyntax final : public ExpressionSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(ArrayCreationExpressionSyntax, ExpressionSyntax);

        const SyntaxToken *kwNew() { return getRequiredNode<SyntaxToken>(m_kwNew, 0); }
        const TypeSyntax *targetType() { return getRequiredNode<TypeSyntax>(m_targetType, 1); }
        const SyntaxToken *lbracket() { return getRequiredNode<SyntaxToken>(m_lbracket, 2); }
        const ExpressionSyntax *size() { return getRequiredNode<ExpressionSyntax>(m_size, 3); }
        const SyntaxToken *rbracket() { return getRequiredNode<SyntaxToken>(m_rbracket, 4); }
        const InitializerExpressionSyntax *initializer() { return getOptionalNode<InitializerExpressionSyntax>(m_initializer, 5); }

    private:
        const SyntaxToken *m_kwNew = nullptr;
        const TypeSyntax *m_targetType = nullptr;
        const SyntaxToken *m_lbracket = nullptr;
        const ExpressionSyntax *m_size = nullptr;
        const SyntaxToken *m_rbracket = nullptr;
        const InitializerExpressionSyntax *m_initializer = nullptr;
    };

    class VariableDeclarationSyntax final : public SyntaxNode {
    public:
        VELA_SYNTAX_NODE_CTOR(VariableDeclarationSyntax, SyntaxNode);

        const TypeSyntax *type() { return getRequiredNode<TypeSyntax>(m_type, 0); }
        const SyntaxToken *identifier() { return getRequiredNode<SyntaxToken>(m_identifier, 1); }
        const SyntaxToken *assignmentToken() { return getOptionalNode<SyntaxToken>(m_assignmentToken, 2); }
        const ExpressionSyntax *initializer() { return getOptionalNode<ExpressionSyntax>(m_initializer, 3); }

    private:
        const TypeSyntax *m_type = nullptr;
        const SyntaxToken *m_identifier = nullptr;
        const SyntaxToken *m_assignmentToken = nullptr;
        const ExpressionSyntax *m_initializer = nullptr;
    };

    class BlockSyntax final : public StatementSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(BlockSyntax, StatementSyntax);

        const SyntaxToken *lbrace() { return getRequiredNode<SyntaxToken>(m_lbrace, 0); }
        SyntaxList<StatementSyntax> statements() {
            return SyntaxList<StatementSyntax>(getOptionalNode<ListSyntax>(m_statements, 1));
        }
        SyntaxList<MemberDeclarationSyntax> declarations() {
            return SyntaxList<MemberDeclarationSyntax>(getOptionalNode<ListSyntax>(m_declarations, 2));
        }
        const SyntaxToken *rparen() { return getRequiredNode<SyntaxToken>(m_rparen, 3); }

    private:
        const SyntaxToken *m_lbrace = nullptr;
        const ListSyntax *m_statements = nullptr;
        const ListSyntax *m_declarations = nullptr;
        const SyntaxToken *m_rparen = nullptr;
    };

    class LocalDeclarationStatementSyntax final : public StatementSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(LocalDeclarationStatementSyntax, StatementSyntax);

        const VariableDeclarationSyntax *declaration() { return getRequiredNode<VariableDeclarationSyntax>(m_declaration, 0); }
        const SyntaxToken *semicolon() { return getRequiredNode<SyntaxToken>(m_semicolon, 1); }

    private:
        const VariableDeclarationSyntax *m_declaration = nullptr;
        const SyntaxToken *m_semicolon = nullptr;
    };

    class ExpressionStatementSyntax final : public StatementSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(ExpressionStatementSyntax, StatementSyntax);

        const ExpressionSyntax *expression() { return getRequiredNode<ExpressionSyntax>(m_expression, 0); }
        const SyntaxToken *semicolon() { return getRequiredNode<SyntaxToken>(m_semicolon, 1); }

    private:
        const ExpressionSyntax *m_expression = nullptr;
        const SyntaxToken *m_semicolon = nullptr;
    };

    class IfStatementSyntax final : public StatementSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(IfStatementSyntax, StatementSyntax);

        const SyntaxToken *kwIf() { return getRequiredNode<SyntaxToken>(m_kwIf, 0); }
        const SyntaxToken *lparen() { return getRequiredNode<SyntaxToken>(m_lparen, 1); }
        const ExpressionSyntax *condition() { return getRequiredNode<ExpressionSyntax>(m_condition, 2); }
        const SyntaxToken *rparen() { return getRequiredNode<SyntaxToken>(m_rparen, 3); }
        const StatementSyntax *statementIf() { return getRequiredNode<StatementSyntax>(m_statementIf, 4); }
        const SyntaxToken *kwElse() { return getOptionalNode<SyntaxToken>(m_kwElse, 5); }
        const StatementSyntax *statementElse() { return getOptionalNode<StatementSyntax>(m_statementElse, 6); }

    private:
        const SyntaxToken *m_kwIf = nullptr;
        const SyntaxToken *m_lparen = nullptr;
        const ExpressionSyntax *m_condition = nullptr;
        const SyntaxToken *m_rparen = nullptr;
        const StatementSyntax *m_statementIf = nullptr;
        const SyntaxToken *m_kwElse = nullptr;
        const StatementSyntax *m_statementElse = nullptr;
    };

    class WhileStatementSyntax final : public StatementSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(WhileStatementSyntax, StatementSyntax);

        const SyntaxToken *kwWhile() { return getRequiredNode<SyntaxToken>(m_kwWhile, 0); }
        const SyntaxToken *lparen() { return getRequiredNode<SyntaxToken>(m_lparen, 1); }
        const ExpressionSyntax *condition() { return getRequiredNode<ExpressionSyntax>(m_condition, 2); }
        const SyntaxToken *rparen() { return getRequiredNode<SyntaxToken>(m_rparen, 3); }
        const StatementSyntax *statement() { return getRequiredNode<StatementSyntax>(m_statement, 4); }

    private:
        const SyntaxToken *m_kwWhile = nullptr;
        const SyntaxToken *m_lparen = nullptr;
        const ExpressionSyntax *m_condition = nullptr;
        const SyntaxToken *m_rparen = nullptr;
        const StatementSyntax *m_statement = nullptr;
    };

    class ForStatementSyntax final : public StatementSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(ForStatementSyntax, StatementSyntax);

        const SyntaxToken *kwFor() { return getRequiredNode<SyntaxToken>(m_kwFor, 0); }
        const SyntaxToken *lparen() { return getRequiredNode<SyntaxToken>(m_lparen, 1); }
        const VariableDeclarationSyntax *initializer() { return getOptionalNode<VariableDeclarationSyntax>(m_initializer, 2); }
        SeparatedSyntaxList<VariableDeclarationSyntax> initializers() {
            return SeparatedSyntaxList<VariableDeclarationSyntax>(getOptionalNode<SeparatedListSyntax>(m_initializers, 3));
        }
        const SyntaxToken *semicolon1() { return getRequiredNode<SyntaxToken>(m_semicolon1, 4); }
        const ExpressionSyntax *condition() { return getOptionalNode<ExpressionSyntax>(m_condition, 5); }
        const SyntaxToken *semicolon2() { return getRequiredNode<SyntaxToken>(m_semicolon2, 6); }
        SeparatedSyntaxList<ExpressionSyntax> incrementors() {
            return SeparatedSyntaxList<ExpressionSyntax>(getOptionalNode<SeparatedListSyntax>(m_incrementors, 7));
        }
        const SyntaxToken *rparen() { return getRequiredNode<SyntaxToken>(m_rparen, 8); }
        const StatementSyntax *statement() { return getRequiredNode<StatementSyntax>(m_statement, 9); }

    private:
        const SyntaxToken *m_kwFor = nullptr;
        const SyntaxToken *m_lparen = nullptr;
        const VariableDeclarationSyntax *m_initializer = nullptr;
        const SeparatedListSyntax *m_initializers = nullptr;
        const SyntaxToken *m_semicolon1 = nullptr;
        const ExpressionSyntax *m_condition = nullptr;
        const SyntaxToken *m_semicolon2 = nullptr;
        const SeparatedListSyntax *m_incrementors = nullptr;
        const SyntaxToken *m_rparen = nullptr;
        const StatementSyntax *m_statement = nullptr;
    };

    class ReturnStatementSyntax final : public StatementSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(ReturnStatementSyntax, StatementSyntax);

        const SyntaxToken *kwReturn() { return getRequiredNode<SyntaxToken>(m_kwReturn, 0); }
        const ExpressionSyntax *returnValue() { return getOptionalNode<ExpressionSyntax>(m_returnValue, 1); }
        const SyntaxToken *semicolon() { return getRequiredNode<SyntaxToken>(m_semicolon, 2); }

    private:
        const SyntaxToken *m_kwReturn = nullptr;
        const ExpressionSyntax *m_returnValue = nullptr;
        const SyntaxToken *m_semicolon = nullptr;
    };

    class ParameterSyntax final : public SyntaxNode {
    public:
        VELA_SYNTAX_NODE_CTOR(ParameterSyntax, SyntaxNode);

        const TypeSyntax *type() { return getRequiredNode<TypeSyntax>(m_type, 0); }
        const SyntaxToken *identifier() { return getRequiredNode<SyntaxToken>(m_identifier, 1); }

    private:
        const TypeSyntax *m_type = nullptr;
        const SyntaxToken *m_identifier = nullptr;
    };

    class FieldDeclarationSyntax final : public MemberDeclarationSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(FieldDeclarationSyntax, MemberDeclarationSyntax);

        const VariableDeclarationSyntax *declaration() { return getRequiredNode<VariableDeclarationSyntax>(m_declaration, 0); }
        const SyntaxToken *semicolon() { return getRequiredNode<SyntaxToken>(m_semicolon, 1); }

    private:
        const VariableDeclarationSyntax *m_declaration = nullptr;
        const SyntaxToken *m_semicolon = nullptr;
    };

    class MethodDeclarationSyntax final : public MemberDeclarationSyntax {
    public:
        VELA_SYNTAX_NODE_CTOR(MethodDeclarationSyntax, MemberDeclarationSyntax);

        const SyntaxToken *kwFn() { return getRequiredNode<SyntaxToken>(m_kwFn, 0); }
        const TypeSyntax *returnType() { return getRequiredNode<TypeSyntax>(m_returnType, 1); }
        const SyntaxToken *identifier() { return getRequiredNode<SyntaxToken>(m_identifier, 2); }
        const SyntaxToken *lparen() { return getRequiredNode<SyntaxToken>(m_lparen, 3); }
        SeparatedSyntaxList<ParameterSyntax> parameters() {
            return SeparatedSyntaxList<ParameterSyntax>(getOptionalNode<SeparatedListSyntax>(m_parameters, 4));
        }
        const SyntaxToken *rparen() { return getRequiredNode<SyntaxToken>(m_rparen, 5); }
        const BlockSyntax *block() { return getRequiredNode<BlockSyntax>(m_block, 6); }

    private:
        const SyntaxToken *m_kwFn = nullptr;
        const TypeSyntax *m_returnType = nullptr;
        const SyntaxToken *m_identifier = nullptr;
        const SyntaxToken *m_lparen = nullptr;
        const SeparatedListSyntax *m_parameters = nullptr;
        const SyntaxToken *m_rparen = nullptr;
        const BlockSyntax *m_block = nullptr;
    };

    class TypeDeclarationSyntax : public MemberDeclarationSyntax {
    public:
        const SyntaxToken *kw() { return getRequiredNode<SyntaxToken>(m_kw, 0); }
        const SyntaxToken *identifier() { return getRequiredNode<SyntaxToken>(m_identifier, 1); }
        const BlockSyntax *block() { return getRequiredNode<BlockSyntax>(m_block, 2); }

    protected:
        VELA_SYNTAX_NODE_CTOR(TypeDeclarationSyntax, MemberDeclarationSyntax);

    private:
        const SyntaxToken *m_kw = nullptr;
        const SyntaxToken *m_identifier = nullptr;
        const BlockSyntax *m_block = nullptr;
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

        SyntaxList<MemberDeclarationSyntax> declarations() {
            return SyntaxList<MemberDeclarationSyntax>(getOptionalNode<ListSyntax>(m_declarations, 0));
        }
        const SyntaxToken *eof() { return getRequiredNode<SyntaxToken>(m_eofToken, 1); }

    private:
        const ListSyntax *m_declarations = nullptr;
        const SyntaxToken *m_eofToken = nullptr;
    };

    class UndefinedSyntax final : public SyntaxNode {
    public:
        VELA_SYNTAX_NODE_CTOR(UndefinedSyntax, SyntaxNode);
    };
}

#undef VELA_SYNTAX_NODE_CTOR

#endif //VELA_SYNTAX_NODE_CONCRETE_H
