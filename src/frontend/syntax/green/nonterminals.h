#ifndef VELA_NONTERMINALS_H
#define VELA_NONTERMINALS_H

#include <vector>

#include "frontend/diagnostic.h"
#include "node.h"

namespace vela::parse {
    class GreenCompilationUnitNode final : public GreenNodeN<2> {
    public:
        GreenCompilationUnitNode(const GreenNode *declarations, const GreenToken *eof, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(CompilationUnitSyntax, { declarations, eof }, std::move(diagnostics)) { }

        const GreenNode *declarations() const { return getSlot(0); }
        const GreenToken *eof() const { return static_cast<const GreenToken *>(getSlot(1)); }
    };

    class GreenClassDeclarationSyntaxNode final : public GreenNodeN<3> {
        GreenClassDeclarationSyntaxNode(
            const GreenToken *kw, const GreenToken *identifier, const GreenNode* block, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(CompilationUnitSyntax, { kw, identifier, block }, std::move(diagnostics)) { }

        const GreenToken *kw() const { return static_cast<const GreenToken *>(getSlot(0)); }
        const GreenToken *identifier() const { return static_cast<const GreenToken *>(getSlot(1)); }
        const GreenNode *body() const { return getSlot(2); }
    };

    class GreenStructDeclarationSyntaxNode final : public GreenNodeN<3> {
        GreenStructDeclarationSyntaxNode(const GreenToken *kw, const GreenToken *identifier, const GreenToken* block, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(StructDeclarationSyntax, { kw, identifier, block }, std::move(diagnostics)) { }

        const GreenToken *kw() const { return static_cast<const GreenToken *>(getSlot(0)); }
        const GreenToken *identifier() const { return static_cast<const GreenToken *>(getSlot(1)); }
        const GreenNode *body() const { return getSlot(2); }
    };

    class GreenFieldDeclarationSyntax final : public GreenNodeN<2> {
        GreenFieldDeclarationSyntax(const GreenNode *declaration, const GreenToken *semicolon, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(FieldDeclarationSyntax, { declaration, semicolon }, std::move(diagnostics)) { }

        const GreenNode *declaration() const { return getSlot(0); }
        const GreenToken *semicolon() const { return static_cast<const GreenToken *>(getSlot(1)); }
    };

    class GreenMethodDeclarationSyntax final : public GreenNodeN<7> {
        GreenMethodDeclarationSyntax(
            const GreenToken *kw,
            const GreenToken *ret,
            const GreenToken *identifier,
            const GreenToken *lparen,
            const GreenNodeList *parameters,
            const GreenToken *rparen,
            const GreenNode *block,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(MethodDeclarationSyntax, { kw, ret, identifier, lparen, parameters, rparen, block }, std::move(diagnostics)) { }

        const GreenToken *kw() const { return static_cast<const GreenToken *>(getSlot(0)); }
        const GreenToken *ret() const { return static_cast<const GreenToken *>(getSlot(1)); }
        const GreenToken *identifier() const { return static_cast<const GreenToken *>(getSlot(2)); }
        const GreenToken *lparen() const { return static_cast<const GreenToken *>(getSlot(3)); }
        const GreenNodeList *parameters() const { return static_cast<const GreenNodeList *>(getSlot(4)); }
        const GreenToken *rparen() const { return static_cast<const GreenToken *>(getSlot(5)); }
        const GreenNode *block() const { return getSlot(6); }
    };

    class GreenParameterSyntax final : public GreenNodeN<2> {
        GreenParameterSyntax(const GreenToken *type, const GreenToken *identifier, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(ParameterSyntax, { type, identifier }, std::move(diagnostics)) { }

        const GreenToken *type() const { return static_cast<const GreenToken *>(getSlot(0)); }
        const GreenToken *identifier() const { return static_cast<const GreenToken *>(getSlot(1)); }
    };

    class GreenVariableDeclarationSyntax final : public GreenNodeN<4> {
        GreenVariableDeclarationSyntax(
            const GreenToken *type,
            const GreenToken *identifier,
            const GreenToken *assign,
            const GreenNode *initializer,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(VariableDeclarationSyntax, { type, identifier, assign, initializer }, std::move(diagnostics)) { }

        const GreenToken *type() const { return static_cast<const GreenToken *>(getSlot(0)); }
        const GreenToken *identifier() const { return static_cast<const GreenToken *>(getSlot(1)); }
        const GreenToken *assign() const { return static_cast<const GreenToken *>(getSlot(2)); }
        const GreenNode *initializer() const { return getSlot(3); }
    };

    class GreenPredefinedTypeSyntax final : public GreenNodeN<1> {
        explicit GreenPredefinedTypeSyntax(const GreenToken *type, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(PredefinedTypeSyntax, { type }, std::move(diagnostics)) { }

        const GreenToken *type() const { return static_cast<const GreenToken *>(getSlot(0)); }
    };

    class GreenIdentifierNameSyntax final : public GreenNodeN<1> {
        explicit GreenIdentifierNameSyntax(const GreenToken *identifier, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(IdentifierNameSyntax, { identifier }, std::move(diagnostics)) { }

        const GreenToken *identifier() const { return static_cast<const GreenToken *>(getSlot(0)); }
    };

    class GreenArrayTypeSyntax final : public GreenNodeN<3> {
        GreenArrayTypeSyntax(
            const GreenToken *type,
            const GreenToken *lbracket,
            const GreenToken *rbracket,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(ArrayTypeSyntax, { type, lbracket, rbracket }, std::move(diagnostics)) { }

        const GreenToken *type() const { return static_cast<const GreenToken *>(getSlot(0)); }
        const GreenToken *lbracket() const { return static_cast<const GreenToken *>(getSlot(1)); }
        const GreenToken *rbracket() const { return static_cast<const GreenToken *>(getSlot(2)); }
    };

    class GreenBlockSyntax final : public GreenNodeN<3> {
        GreenBlockSyntax(
            const GreenToken *lbrace,
            const GreenNodeList *statements,
            const GreenToken *rbrace,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(BlockSyntax, { lbrace, statements, rbrace }, std::move(diagnostics)) { }

        const GreenToken *lbrace() const { return static_cast<const GreenToken *>(getSlot(0)); }
        const GreenNodeList *statements() const { return static_cast<const GreenNodeList *>(getSlot(1)); }
        const GreenToken *rbrace() const { return static_cast<const GreenToken *>(getSlot(2)); }
    };

    class GreenLocalDeclarationStatementSyntax final : public GreenNodeN<2> {
        GreenLocalDeclarationStatementSyntax(const GreenNode *declaration, const GreenToken *semicolon, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(LocalDeclarationStatementSyntax, { declaration, semicolon }, std::move(diagnostics)) { }

        const GreenNode *declaration() const { return getSlot(0); }
        const GreenToken *semicolon() const { return static_cast<const GreenToken *>(getSlot(1)); }
    };

    class GreenExpressionStatementSyntax final : public GreenNodeN<2> {
        GreenExpressionStatementSyntax(const GreenNode *expression, const GreenToken *semicolon, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(ExpressionStatementSyntax, { expression, semicolon }, std::move(diagnostics)) { }

        const GreenNode *expression() const { return getSlot(0); }
        const GreenToken *semicolon() const { return static_cast<const GreenToken *>(getSlot(1)); }
    };

    class GreenIfStatementSyntax final : public GreenNodeN<7> {
        GreenIfStatementSyntax(
            const GreenToken *kwIf,
            const GreenToken *lparen,
            const GreenNode *condition,
            const GreenToken *rparen,
            const GreenNode *statement,
            const GreenToken *kwElse,
            const GreenNode *elseStatement,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(IfStatementSyntax, { kwIf, lparen, condition, rparen, statement, kwElse, elseStatement }, std::move(diagnostics) ) { }

        const GreenToken *kwIf() const { return static_cast<const GreenToken *>(getSlot(0)); }
        const GreenToken *lparen() const { return static_cast<const GreenToken *>(getSlot(1)); }
        const GreenNode *condition() const { return getSlot(2); }
        const GreenToken *rparen() const { return static_cast<const GreenToken *>(getSlot(3)); }
        const GreenNode *statement() const { return getSlot(4); }
        const GreenToken *kwElse() const { return static_cast<const GreenToken *>(getSlot(5)); }
        const GreenNode *elseStatement() const { return getSlot(6); }
    };

    class GreenWhileStatementSyntax final : public GreenNodeN<5> {
        GreenWhileStatementSyntax(
            const GreenToken *kw,
            const GreenToken *lparen,
            const GreenNode *condition,
            const GreenToken *rparen,
            const GreenNode *statement,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(WhileStatementSyntax, { kw, lparen, condition, rparen, statement }, std::move(diagnostics) ) { }

        const GreenToken *kw() const { return static_cast<const GreenToken *>(getSlot(0)); }
        const GreenToken *lparen() const { return static_cast<const GreenToken *>(getSlot(1)); }
        const GreenNode *condition() const { return getSlot(2); }
        const GreenToken *rparen() const { return static_cast<const GreenToken *>(getSlot(3)); }
        const GreenNode *statement() const { return getSlot(4); }
    };

    class GreenForStatementSyntax final : public GreenNodeN<9> {
        GreenForStatementSyntax(
            const GreenToken *kw,
            const GreenToken *lparen,
            const GreenNode *initializer,
            const GreenToken *semi1,
            const GreenNode *condition,
            const GreenToken *semi2,
            const GreenNode *increment,
            const GreenToken *rparen,
            const GreenNode *statement,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(ForStatementSyntax, { kw, lparen, initializer, semi1, condition, semi2, increment, rparen, statement }, std::move(diagnostics) ) { }

        const GreenToken *kw() const { return static_cast<const GreenToken *>(getSlot(0)); }
        const GreenToken *lparen() const { return static_cast<const GreenToken *>(getSlot(1)); }
        const GreenNode *initializer() const { return getSlot(2); }
        const GreenToken *semi1() const { return static_cast<const GreenToken *>(getSlot(3)); }
        const GreenNode *condition() const { return getSlot(4); }
        const GreenToken *semi2() const { return static_cast<const GreenToken *>(getSlot(5)); }
        const GreenNode *increment() const { return getSlot(6); }
        const GreenToken *rparen() const { return static_cast<const GreenToken *>(getSlot(7)); }
        const GreenNode *statement() const { return getSlot(8); }
    };

    class GreenReturnStatementSyntax final : public GreenNodeN<3> {
        GreenReturnStatementSyntax(
            const GreenToken *kw,
            const GreenNode *expression,
            const GreenToken *semicolon,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(ReturnStatementSyntax, { kw, expression, semicolon }, std::move(diagnostics) ) { }

        const GreenToken *kw() const { return static_cast<const GreenToken *>(getSlot(0)); }
        const GreenNode *expression() const { return getSlot(1); }
        const GreenToken *semicolon() const { return static_cast<const GreenToken *>(getSlot(2)); }
    };

    class GreenLiteralExpressionSyntax final : public GreenNodeN<1> {
        explicit GreenLiteralExpressionSyntax(const GreenToken *literal, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(LiteralExpressionSyntax, { literal }, std::move(diagnostics) ) { }

        const GreenToken *literal() const { return static_cast<const GreenToken *>(getSlot(0)); }
    };

    class GreenThisExpressionSyntax final : public GreenNodeN<1> {
        explicit GreenThisExpressionSyntax(const GreenToken *kw, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(ThisExpressionSyntax, { kw }, std::move(diagnostics) ) { }

        const GreenToken *kw() const { return static_cast<const GreenToken *>(getSlot(0)); }
    };

    class GreenPrefixUnaryExpressionSyntax final : public GreenNodeN<2> {
        GreenPrefixUnaryExpressionSyntax(const GreenToken *op, const GreenNode *operand, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(PrefixUnaryExpressionSyntax, { op, operand }, std::move(diagnostics) ) { }

        const GreenToken *op() const { return static_cast<const GreenToken *>(getSlot(0)); }
        const GreenNode *operand() const { return getSlot(1); }
    };

    class GreenPostfixUnaryExpressionSyntax final : public GreenNodeN<2> {
        GreenPostfixUnaryExpressionSyntax(const GreenToken *op, const GreenNode *operand, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(PostfixUnaryExpressionSyntax, { op, operand }, std::move(diagnostics) ) { }

        const GreenToken *op() const { return static_cast<const GreenToken *>(getSlot(0)); }
        const GreenNode *operand() const { return getSlot(1); }
    };

    class GreenBinaryExpressionSyntax final : public GreenNodeN<3> {
        GreenBinaryExpressionSyntax(
            const GreenNode *lhs,
            const GreenToken *op,
            const GreenNode *rhs,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(BinaryExpressionSyntax, { lhs, op, rhs }, std::move(diagnostics) ) { }

        const GreenNode *lhs() const { return getSlot(0); }
        const GreenToken *op() const { return static_cast<const GreenToken *>(getSlot(1)); }
        const GreenNode *rhs() const { return getSlot(2); }
    };

    class GreenAssignmentExpressionSyntax final : public GreenNodeN<3> {
        GreenAssignmentExpressionSyntax(
            const GreenNode *lhs,
            const GreenToken *assignment,
            const GreenNode *rhs,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(AssignmentExpressionSyntax, { lhs, assignment, rhs }, std::move(diagnostics) ) { }

        const GreenNode *lhs() const { return getSlot(0); }
        const GreenToken *assignment() const { return static_cast<const GreenToken *>(getSlot(1)); }
        const GreenNode *rhs() const { return getSlot(2); }
    };

    class GreenInvocationExpressionSyntax final : public GreenNodeN<4> {
        GreenInvocationExpressionSyntax(
            const GreenNode *expression,
            const GreenToken *lparen,
            const GreenNodeList *arguments,
            const GreenToken *rparen,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(InvocationExpressionSyntax, { expression, lparen, arguments, rparen }, std::move(diagnostics) ) { }

        const GreenNode *expression() const { return getSlot(0); }
        const GreenToken *lparen() const { return static_cast<const GreenToken *>(getSlot(1)); }
        const GreenNodeList *arguments() const { return static_cast<const GreenNodeList *>(getSlot(2)); }
        const GreenToken *rparen() const { return static_cast<const GreenToken *>(getSlot(3)); }
    };

    class GreenElementAccessExpressionSyntax final : public GreenNodeN<4> {
        GreenElementAccessExpressionSyntax(
            const GreenNode *expression,
            const GreenToken *lbracket,
            const GreenNode *index,
            const GreenToken *rbracket,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(ElementAccessExpressionSyntax, { expression, lbracket, index, rbracket }, std::move(diagnostics) ) { }

        const GreenNode *expression() const { return getSlot(0); }
        const GreenToken *lbracket() const { return static_cast<const GreenToken *>(getSlot(1)); }
        const GreenNode *index() const { return getSlot(2); }
        const GreenToken *rbracket() const { return static_cast<const GreenToken *>(getSlot(3)); }
    };

    class GreenObjectCreationExpressionSyntax final : public GreenNodeN<5> {
        GreenObjectCreationExpressionSyntax(
            const GreenToken *kw,
            const GreenNode *type,
            const GreenToken *lparen,
            const GreenNodeList *arguments,
            const GreenToken *rparen,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(ObjectCreationExpressionSyntax, { kw, type, lparen, arguments, rparen }, std::move(diagnostics) ) { }

        const GreenToken *kw() const { return static_cast<const GreenToken *>(getSlot(1)); }
        const GreenNode *type() const { return getSlot(2); }
        const GreenToken *lparen() const { return static_cast<const GreenToken *>(getSlot(3)); }
        const GreenNodeList *arguments() const { return static_cast<const GreenNodeList *>(getSlot(4)); }
        const GreenToken *rparen() const { return static_cast<const GreenToken *>(getSlot(5)); }
    };

    class GreenInitializerExpressionSyntax final : public GreenNodeN<3> {
        GreenInitializerExpressionSyntax(
            const GreenToken *lbrace,
            const GreenNodeList *expressions,
            const GreenToken *rbrace,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(InitializerExpressionSyntax, { lbrace, expressions, rbrace }, std::move(diagnostics) ) { }

        const GreenToken *lbrace() const { return static_cast<const GreenToken *>(getSlot(1)); }
        const GreenNodeList *expressions() const { return static_cast<const GreenNodeList *>(getSlot(2)); }
        const GreenToken *rbrace() const { return static_cast<const GreenToken *>(getSlot(3)); }
    };

    class GreenArrayCreationExpressionSyntax final : public GreenNodeN<6> {
        GreenArrayCreationExpressionSyntax(
            const GreenToken *kw,
            const GreenNode *type,
            const GreenToken *lbracket,
            const GreenNode *size,
            const GreenToken *rbracket,
            const GreenNode *initializer,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(ArrayCreationExpressionSyntax, { kw, type, lbracket, size, rbracket, initializer }, std::move(diagnostics) ) { }

        const GreenToken *kw() const { return static_cast<const GreenToken *>(getSlot(1)); }
        const GreenNode *type() const { return getSlot(2); }
        const GreenToken *lbracket() const { return static_cast<const GreenToken *>(getSlot(3)); }
        const GreenNode *size() const { return getSlot(4); }
        const GreenToken *rbracket() const { return static_cast<const GreenToken *>(getSlot(5)); }
        const GreenNode *initializer() const { return getSlot(6); }
    };
}

#endif //VELA_NONTERMINALS_H
