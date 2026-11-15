#ifndef VELA_SYNTAX_NODE_CONCRETE_H
#define VELA_SYNTAX_NODE_CONCRETE_H

#include "syntax_node.h"
#include "syntax_node_fwds.h"

namespace vela::parse {
    class StatementListSyntax final : public SyntaxNode {
    public:
        StatementListSyntax(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : SyntaxNode(green, parent, syntaxTree, position) { }

        [[nodiscard]] NodeKind kind() const override { return NodeKind::ListSyntax; }

    private:
        std::vector<const StatementSyntax *> m_statements;
    };

    class SeparatedListSyntax final : public SyntaxNode { };
    class StatementSyntax final : public SyntaxNode { };
    class ExpressionSyntax final : public SyntaxNode { };

    class BlockSyntax final : public SyntaxNode {
    public:
        BlockSyntax(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : SyntaxNode(green, parent, syntaxTree, position) { }

        [[nodiscard]] NodeKind kind() const override { return NodeKind::BlockSyntax; }

        [[nodiscard]] const StatementListSyntax *statement() { return validateRequiredNode<StatementListSyntax>(m_statementList, 0); }

    private:
        const StatementListSyntax *m_statementList = nullptr;
    };

    class CompilationUnitSyntax final : public SyntaxNode {
    public:
        CompilationUnitSyntax(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : SyntaxNode(green, parent, syntaxTree, position) { }

        [[nodiscard]] NodeKind kind() const override { return NodeKind::CompilationUnitSyntax; }

        [[nodiscard]] const StatementListSyntax *statements() { return validateRequiredNode<StatementListSyntax>(m_statementList, 0); }
        [[nodiscard]] const SyntaxToken *eofToken() { return validateRequiredNode<SyntaxToken>(m_eofToken, 1); }

    private:
        const StatementListSyntax *m_statementList = nullptr;
        const SyntaxToken *m_eofToken = nullptr;
    };

    class ClassDeclarationSyntax final : public SyntaxNode {
    public:
        ClassDeclarationSyntax(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : SyntaxNode(green, parent, syntaxTree, position) { }

        [[nodiscard]] NodeKind kind() const override { return NodeKind::ClassDeclarationSyntax; }

        [[nodiscard]] const SyntaxToken *identifier() { return validateRequiredNode<SyntaxToken>(m_identifier, 1); }
        [[nodiscard]] const BlockSyntax *block() { return validateRequiredNode<BlockSyntax>(m_block, 2); }

    private:
        const SyntaxToken *m_identifier = nullptr;
        const BlockSyntax *m_block = nullptr;
    };

    class StructDeclarationSyntax final : public SyntaxNode {
    public:
        StructDeclarationSyntax(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : SyntaxNode(green, parent, syntaxTree, position) { }

        [[nodiscard]] NodeKind kind() const override { return NodeKind::StructDeclarationSyntax; }

        [[nodiscard]] const SyntaxToken *identifier() { return validateRequiredNode<SyntaxToken>(m_identifier, 1); }
        [[nodiscard]] const BlockSyntax *block() { return validateRequiredNode<BlockSyntax>(m_block, 2); }

    private:
        const SyntaxToken *m_identifier = nullptr;
        const BlockSyntax *m_block = nullptr;
    };

    class FieldDeclarationSyntax final : public SyntaxNode {
    public:
        FieldDeclarationSyntax(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : SyntaxNode(green, parent, syntaxTree, position) { }

        [[nodiscard]] NodeKind kind() const override { return NodeKind::FieldDeclarationSyntax; }

        [[nodiscard]] const VariableDeclarationSyntax *declaration() { return validateRequiredNode<VariableDeclarationSyntax>(m_declaration, 0); }

    private:
        const VariableDeclarationSyntax *m_declaration = nullptr;
    };

    class MethodDeclarationSyntax final : public SyntaxNode {
    public:
        MethodDeclarationSyntax(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : SyntaxNode(green, parent, syntaxTree, position) { }

        [[nodiscard]] NodeKind kind() const override { return NodeKind::MethodDeclarationSyntax; }

        [[nodiscard]] const SyntaxToken *returnType() { return validateRequiredNode<SyntaxToken>(m_returnType, 1); }
        [[nodiscard]] const SyntaxToken *identifier() { return validateRequiredNode<SyntaxToken>(m_identifier, 2); }
        [[nodiscard]] const SeparatedListSyntax *parameters() { return validateRequiredNode<SeparatedListSyntax>(m_parameters, 4); }
        [[nodiscard]] const BlockSyntax *block() { return validateRequiredNode<BlockSyntax>(m_block, 6); }

    private:
        const SyntaxToken *m_returnType = nullptr;
        const SyntaxToken *m_identifier = nullptr;
        const SeparatedListSyntax *m_parameters = nullptr;
        const BlockSyntax *m_block = nullptr;
    };

    class ParameterSyntax final : public SyntaxNode {
    public:
        ParameterSyntax(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : SyntaxNode(green, parent, syntaxTree, position) { }

        [[nodiscard]] NodeKind kind() const override { return NodeKind::ParameterSyntax; }

        [[nodiscard]] const SyntaxToken *type() { return validateRequiredNode<SyntaxToken>(m_type, 0); }
        [[nodiscard]] const SyntaxToken *identifier() { return validateRequiredNode<SyntaxToken>(m_identifier, 1); }

    private:
        const SyntaxToken *m_type = nullptr;
        const SyntaxToken *m_identifier = nullptr;
    };

    class PredefinedTypeSyntax final : public SyntaxNode {
    public:
        PredefinedTypeSyntax(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : SyntaxNode(green, parent, syntaxTree, position) { }

        [[nodiscard]] NodeKind kind() const override { return NodeKind::PredefinedTypeSyntax; }

        [[nodiscard]] const SyntaxToken *type() { return validateRequiredNode<SyntaxToken>(m_type, 0); }

    private:
        const SyntaxToken *m_type = nullptr;
    };

    class IdentifierNameSyntax final : public SyntaxNode {
    public:
        IdentifierNameSyntax(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : SyntaxNode(green, parent, syntaxTree, position) { }

        [[nodiscard]] NodeKind kind() const override { return NodeKind::IdentifierNameSyntax; }

        [[nodiscard]] const SyntaxToken *identifier() { return validateRequiredNode<SyntaxToken>(m_identifier, 0); }

    private:
        const SyntaxToken *m_identifier = nullptr;
    };

    class ArrayTypeSyntax final : public SyntaxNode {
    public:
        ArrayTypeSyntax(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : SyntaxNode(green, parent, syntaxTree, position) { }

        [[nodiscard]] NodeKind kind() const override { return NodeKind::ArrayTypeSyntax; }

        [[nodiscard]] const SyntaxToken *type() { return validateRequiredNode<SyntaxToken>(m_type, 0); }

    private:
        const SyntaxToken *m_type = nullptr;
    };

    class LocalDeclarationStatementSyntax final : public SyntaxNode {
    public:
        LocalDeclarationStatementSyntax(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : SyntaxNode(green, parent, syntaxTree, position) { }

        [[nodiscard]] NodeKind kind() const override { return NodeKind::LocalDeclarationStatementSyntax; }

        [[nodiscard]] const SyntaxToken *type() { return validateRequiredNode<SyntaxToken>(m_type, 0); }

    private:
        const SyntaxToken *m_type = nullptr;
    };

    class ExpressionStatementSyntax final : public SyntaxNode {
    public:
        ExpressionStatementSyntax(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : SyntaxNode(green, parent, syntaxTree, position) { }

        [[nodiscard]] NodeKind kind() const override { return NodeKind::ExpressionStatementSyntax; }

        [[nodiscard]] const ExpressionSyntax *expression() { return validateRequiredNode<ExpressionSyntax>(m_expression, 0); }

    private:
        const SyntaxToken *m_expression = nullptr;
    };

    class IfStatementSyntax final : public SyntaxNode {
    public:
        IfStatementSyntax(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : SyntaxNode(green, parent, syntaxTree, position) { }

        [[nodiscard]] NodeKind kind() const override { return NodeKind::IfStatementSyntax; }

        [[nodiscard]] const ExpressionSyntax *condition() { return validateRequiredNode<ExpressionSyntax>(m_condition, 2); }
        [[nodiscard]] const StatementSyntax *statement() { return validateRequiredNode<StatementSyntax>(m_statement, 4); }
        [[nodiscard]] const StatementSyntax *elseStatement() { return validateRequiredNode<StatementSyntax>(m_elseStatement, 6); }

    private:
        const ExpressionSyntax *m_condition = nullptr;
        const StatementSyntax *m_statement = nullptr;
        const StatementSyntax *m_elseStatement = nullptr;
    };

    class WhileStatementSyntax final : public SyntaxNode {
    public:
        WhileStatementSyntax(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : SyntaxNode(green, parent, syntaxTree, position) { }

        [[nodiscard]] NodeKind kind() const override { return NodeKind::WhileStatementSyntax; }

        [[nodiscard]] const ExpressionSyntax *expression() { return validateRequiredNode<ExpressionSyntax>(m_condition, 2); }
        [[nodiscard]] const StatementSyntax *statement() { return validateRequiredNode<StatementSyntax>(m_statement, 4); }

    private:
        const ExpressionSyntax *m_condition = nullptr;
        const StatementSyntax *m_statement = nullptr;
    };

    class ForStatementSyntax final : public SyntaxNode {
    public:
        ForStatementSyntax(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : SyntaxNode(green, parent, syntaxTree, position) { }

        [[nodiscard]] NodeKind kind() const override { return NodeKind::ForStatementSyntax; }

        [[nodiscard]] const StatementSyntax *initializer() { return validateRequiredNode<StatementSyntax>(m_initializer, 2); }
        [[nodiscard]] const ExpressionSyntax *condition() { return validateRequiredNode<ExpressionSyntax>(m_condition, 4); }
        [[nodiscard]] const StatementSyntax *increment() { return validateRequiredNode<StatementSyntax>(m_increment, 6); }
        [[nodiscard]] const StatementSyntax *statement() { return validateRequiredNode<StatementSyntax>(m_statement, 8); }

    private:
        const StatementSyntax *m_initializer = nullptr;
        const ExpressionSyntax *m_condition = nullptr;
        const StatementSyntax *m_increment = nullptr;
        const StatementSyntax *m_statement = nullptr;
    };

    class ReturnStatementSyntax final : public SyntaxNode {
    public:
        ReturnStatementSyntax(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : SyntaxNode(green, parent, syntaxTree, position) { }

        [[nodiscard]] NodeKind kind() const override { return NodeKind::ReturnStatementSyntax; }

        [[nodiscard]] const ExpressionSyntax *expression() { return validateRequiredNode<ExpressionSyntax>(m_expression, 1); }

    private:
        const ExpressionSyntax *m_expression = nullptr;
    };

    class LiteralExpressionSyntax final : public SyntaxNode {
    public:
        LiteralExpressionSyntax(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : SyntaxNode(green, parent, syntaxTree, position) { }

        [[nodiscard]] NodeKind kind() const override { return NodeKind::LiteralExpressionSyntax; }

        [[nodiscard]] const ExpressionSyntax *expression() { return validateRequiredNode<ExpressionSyntax>(m_expression, 1); }

    private:
        const ExpressionSyntax *m_expression = nullptr;
    };

    class ThisExpressionSyntax final : public SyntaxNode {
    public:
        ThisExpressionSyntax(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : SyntaxNode(green, parent, syntaxTree, position) { }

        [[nodiscard]] NodeKind kind() const override { return NodeKind::ThisExpressionSyntax; }
    };
}

#endif //VELA_SYNTAX_NODE_CONCRETE_H
