#ifndef VELA_GREEN_NONTERMINALS_H
#define VELA_GREEN_NONTERMINALS_H

#include <vector>

#include "green_node.h"
#include "frontend/diagnostic.h"

namespace vela::parse {
    class GreenCompilationUnitSyntax final : public GreenNodeN<2> {
    public:
        GreenCompilationUnitSyntax(const GreenNode *declarations, const GreenToken *eof, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::CompilationUnitSyntax, { declarations, eof }, std::move(diagnostics)) { }

        const GreenNode *declarations() const { return slot(0); }
        const GreenToken *eof() const { return static_cast<const GreenToken *>(slot(1)); }
    };

    class GreenClassDeclarationSyntax final : public GreenNodeN<3> {
    public:
        GreenClassDeclarationSyntax(
            const GreenToken *kw, const GreenToken *identifier, const GreenNode* block, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::ClassDeclarationSyntax, { kw, identifier, block }, std::move(diagnostics)) { }

        const GreenToken *kw() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenToken *identifier() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenNode *block() const { return slot(2); }
    };

    class GreenStructDeclarationSyntax final : public GreenNodeN<3> {
    public:
        GreenStructDeclarationSyntax(const GreenToken *kw, const GreenToken *identifier, const GreenNode* block, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::StructDeclarationSyntax, { kw, identifier, block }, std::move(diagnostics)) { }

        const GreenToken *kw() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenToken *identifier() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenNode *block() const { return slot(2); }
    };

    class GreenFieldDeclarationSyntax final : public GreenNodeN<2> {
    public:
        GreenFieldDeclarationSyntax(const GreenNode *declaration, const GreenToken *semicolon, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::FieldDeclarationSyntax, { declaration, semicolon }, std::move(diagnostics)) { }

        const GreenNode *declaration() const { return slot(0); }
        const GreenToken *semicolon() const { return static_cast<const GreenToken *>(slot(1)); }
    };

    class GreenMethodDeclarationSyntax final : public GreenNodeN<7> {
    public:
        GreenMethodDeclarationSyntax(
            const GreenToken *kw,
            const GreenToken *ret,
            const GreenToken *identifier,
            const GreenToken *lparen,
            const GreenNodeList *parameters,
            const GreenToken *rparen,
            const GreenNode *block,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::MethodDeclarationSyntax, { kw, ret, identifier, lparen, parameters, rparen, block }, std::move(diagnostics)) { }

        const GreenToken *kw() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenToken *ret() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenToken *identifier() const { return static_cast<const GreenToken *>(slot(2)); }
        const GreenToken *lparen() const { return static_cast<const GreenToken *>(slot(3)); }
        const GreenNodeList *parameters() const { return static_cast<const GreenNodeList *>(slot(4)); }
        const GreenToken *rparen() const { return static_cast<const GreenToken *>(slot(5)); }
        const GreenNode *block() const { return slot(6); }
    };

    class GreenParameterSyntax final : public GreenNodeN<2> {
    public:
        GreenParameterSyntax(const GreenToken *type, const GreenToken *identifier, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::ParameterSyntax, { type, identifier }, std::move(diagnostics)) { }

        const GreenToken *type() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenToken *identifier() const { return static_cast<const GreenToken *>(slot(1)); }
    };

    class GreenVariableDeclarationSyntax final : public GreenNodeN<4> {
    public:
        GreenVariableDeclarationSyntax(
            const GreenToken *type,
            const GreenToken *identifier,
            const GreenToken *assign,
            const GreenNode *initializer,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::VariableDeclarationSyntax, { type, identifier, assign, initializer }, std::move(diagnostics)) { }

        const GreenToken *type() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenToken *identifier() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenToken *assign() const { return static_cast<const GreenToken *>(slot(2)); }
        const GreenNode *initializer() const { return slot(3); }
    };

    class GreenPredefinedTypeSyntax final : public GreenNodeN<1> {
    public:
        explicit GreenPredefinedTypeSyntax(const GreenToken *type, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::PredefinedTypeSyntax, { type }, std::move(diagnostics)) { }

        const GreenToken *type() const { return static_cast<const GreenToken *>(slot(0)); }
    };

    class GreenIdentifierNameSyntax final : public GreenNodeN<1> {
    public:
        explicit GreenIdentifierNameSyntax(const GreenToken *identifier, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::IdentifierNameSyntax, { identifier }, std::move(diagnostics)) { }

        const GreenToken *identifier() const { return static_cast<const GreenToken *>(slot(0)); }
    };

    class GreenArrayTypeSyntax final : public GreenNodeN<3> {
    public:
        GreenArrayTypeSyntax(
            const GreenToken *type,
            const GreenToken *lbracket,
            const GreenToken *rbracket,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::ArrayTypeSyntax, { type, lbracket, rbracket }, std::move(diagnostics)) { }

        const GreenToken *elementType() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenToken *lbracket() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenToken *rbracket() const { return static_cast<const GreenToken *>(slot(2)); }
    };

    class GreenBlockSyntax final : public GreenNodeN<4> {
    public:
        GreenBlockSyntax(
            const GreenToken *lbrace,
            const GreenNodeList *statements,
            const GreenNodeList *declarations,
            const GreenToken *rbrace,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::BlockSyntax, { lbrace, statements, declarations, rbrace }, std::move(diagnostics)) { }

        const GreenToken *lbrace() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNodeList *statements() const { return static_cast<const GreenNodeList *>(slot(1)); }
        const GreenNodeList *declarations() const { return static_cast<const GreenNodeList *>(slot(2)); }
        const GreenToken *rbrace() const { return static_cast<const GreenToken *>(slot(3)); }
    };

    class GreenLocalDeclarationStatementSyntax final : public GreenNodeN<2> {
    public:
        GreenLocalDeclarationStatementSyntax(const GreenNode *declaration, const GreenToken *semicolon, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::LocalDeclarationStatementSyntax, { declaration, semicolon }, std::move(diagnostics)) { }

        const GreenNode *declaration() const { return slot(0); }
        const GreenToken *semicolon() const { return static_cast<const GreenToken *>(slot(1)); }
    };

    class GreenExpressionStatementSyntax final : public GreenNodeN<2> {
    public:
        GreenExpressionStatementSyntax(const GreenNode *expression, const GreenToken *semicolon, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::ExpressionStatementSyntax, { expression, semicolon }, std::move(diagnostics)) { }

        const GreenNode *expression() const { return slot(0); }
        const GreenToken *semicolon() const { return static_cast<const GreenToken *>(slot(1)); }
    };

    class GreenIfStatementSyntax final : public GreenNodeN<7> {
    public:
        GreenIfStatementSyntax(
            const GreenToken *kwIf,
            const GreenToken *lparen,
            const GreenNode *condition,
            const GreenToken *rparen,
            const GreenNode *statement,
            const GreenToken *kwElse,
            const GreenNode *elseStatement,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::IfStatementSyntax, { kwIf, lparen, condition, rparen, statement, kwElse, elseStatement }, std::move(diagnostics) ) { }

        const GreenToken *kwIf() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenToken *lparen() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenNode *condition() const { return slot(2); }
        const GreenToken *rparen() const { return static_cast<const GreenToken *>(slot(3)); }
        const GreenNode *statement() const { return slot(4); }
        const GreenToken *kwElse() const { return static_cast<const GreenToken *>(slot(5)); }
        const GreenNode *elseStatement() const { return slot(6); }
    };

    class GreenWhileStatementSyntax final : public GreenNodeN<5> {
    public:
        GreenWhileStatementSyntax(
            const GreenToken *kw,
            const GreenToken *lparen,
            const GreenNode *condition,
            const GreenToken *rparen,
            const GreenNode *statement,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::WhileStatementSyntax, { kw, lparen, condition, rparen, statement }, std::move(diagnostics) ) { }

        const GreenToken *kw() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenToken *lparen() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenNode *condition() const { return slot(2); }
        const GreenToken *rparen() const { return static_cast<const GreenToken *>(slot(3)); }
        const GreenNode *statement() const { return slot(4); }
    };

    class GreenForStatementSyntax final : public GreenNodeN<10> {
    public:
        GreenForStatementSyntax(
            const GreenToken *kw,
            const GreenToken *lparen,
            const GreenNode *initializer,
            const GreenNodeList *initializers,
            const GreenToken *semi1,
            const GreenNode *condition,
            const GreenToken *semi2,
            const GreenNodeList *incrementors,
            const GreenToken *rparen,
            const GreenNode *statement,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::ForStatementSyntax, { kw, lparen, initializer, initializers, semi1, condition, semi2, incrementors, rparen, statement }, std::move(diagnostics) ) { }

        const GreenToken* kw() const { return static_cast<const GreenToken*>(slot(0)); }
        const GreenToken* lparen() const { return static_cast<const GreenToken*>(slot(1)); }
        const GreenNode* initializer() const { return slot(2); }
        const GreenNodeList* initializers() const { return static_cast<const GreenNodeList*>(slot(3)); }
        const GreenToken* semi1() const { return static_cast<const GreenToken*>(slot(4)); }
        const GreenNode* condition() const { return slot(5); }
        const GreenToken* semi2() const { return static_cast<const GreenToken*>(slot(6)); }
        const GreenNodeList* incrementors() const { return static_cast<const GreenNodeList*>(slot(7)); }
        const GreenToken* rparen() const { return static_cast<const GreenToken*>(slot(8)); }
        const GreenNode* statement() const { return slot(9); }
    };

    class GreenReturnStatementSyntax final : public GreenNodeN<3> {
    public:
        GreenReturnStatementSyntax(
            const GreenToken *kw,
            const GreenNode *expression,
            const GreenToken *semicolon,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::ReturnStatementSyntax, { kw, expression, semicolon }, std::move(diagnostics) ) { }

        const GreenToken *kw() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNode *expression() const { return slot(1); }
        const GreenToken *semicolon() const { return static_cast<const GreenToken *>(slot(2)); }
    };

    class GreenLiteralExpressionSyntax final : public GreenNodeN<1> {
    public:
        explicit GreenLiteralExpressionSyntax(const GreenToken *literal, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::LiteralExpressionSyntax, { literal }, std::move(diagnostics) ) { }

        const GreenToken *literal() const { return static_cast<const GreenToken *>(slot(0)); }
    };

    class GreenThisExpressionSyntax final : public GreenNodeN<1> {
    public:
        explicit GreenThisExpressionSyntax(const GreenToken *kw, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::ThisExpressionSyntax, { kw }, std::move(diagnostics) ) { }

        const GreenToken *kw() const { return static_cast<const GreenToken *>(slot(0)); }
    };

    class GreenPrefixUnaryExpressionSyntax final : public GreenNodeN<2> {
    public:
        GreenPrefixUnaryExpressionSyntax(const GreenToken *op, const GreenNode *operand, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::PrefixUnaryExpressionSyntax, { op, operand }, std::move(diagnostics) ) { }

        const GreenToken *op() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNode *operand() const { return slot(1); }
    };

    class GreenPostfixUnaryExpressionSyntax final : public GreenNodeN<2> {
    public:
        GreenPostfixUnaryExpressionSyntax(const GreenToken *op, const GreenNode *operand, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::PostfixUnaryExpressionSyntax, { op, operand }, std::move(diagnostics) ) { }

        const GreenToken *op() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNode *operand() const { return slot(1); }
    };

    class GreenBinaryExpressionSyntax final : public GreenNodeN<3> {
    public:
        GreenBinaryExpressionSyntax(
            const GreenNode *lhs,
            const GreenToken *op,
            const GreenNode *rhs,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::BinaryExpressionSyntax, { lhs, op, rhs }, std::move(diagnostics) ) { }

        const GreenNode *lhs() const { return slot(0); }
        const GreenToken *op() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenNode *rhs() const { return slot(2); }
    };

    class GreenAssignmentExpressionSyntax final : public GreenNodeN<3> {
    public:
        GreenAssignmentExpressionSyntax(
            const GreenNode *lhs,
            const GreenToken *assignment,
            const GreenNode *rhs,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::AssignmentExpressionSyntax, { lhs, assignment, rhs }, std::move(diagnostics) ) { }

        const GreenNode *lhs() const { return slot(0); }
        const GreenToken *assignment() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenNode *rhs() const { return slot(2); }
    };

    class GreenInvocationExpressionSyntax final : public GreenNodeN<4> {
    public:
        GreenInvocationExpressionSyntax(
            const GreenNode *expression,
            const GreenToken *lparen,
            const GreenNodeList *arguments,
            const GreenToken *rparen,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::InvocationExpressionSyntax, { expression, lparen, arguments, rparen }, std::move(diagnostics) ) { }

        const GreenNode *expression() const { return slot(0); }
        const GreenToken *lparen() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenNodeList *arguments() const { return static_cast<const GreenNodeList *>(slot(2)); }
        const GreenToken *rparen() const { return static_cast<const GreenToken *>(slot(3)); }
    };

    class GreenElementAccessExpressionSyntax final : public GreenNodeN<4> {
    public:
        GreenElementAccessExpressionSyntax(
            const GreenNode *expression,
            const GreenToken *lbracket,
            const GreenNode *index,
            const GreenToken *rbracket,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::ElementAccessExpressionSyntax, { expression, lbracket, index, rbracket }, std::move(diagnostics) ) { }

        const GreenNode *expression() const { return slot(0); }
        const GreenToken *lbracket() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenNode *index() const { return slot(2); }
        const GreenToken *rbracket() const { return static_cast<const GreenToken *>(slot(3)); }
    };

    class GreenObjectCreationExpressionSyntax final : public GreenNodeN<5> {
    public:
        GreenObjectCreationExpressionSyntax(
            const GreenToken *kw,
            const GreenNode *type,
            const GreenToken *lparen,
            const GreenNodeList *arguments,
            const GreenToken *rparen,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::ObjectCreationExpressionSyntax, { kw, type, lparen, arguments, rparen }, std::move(diagnostics) ) { }

        const GreenToken *kw() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNode *type() const { return slot(1); }
        const GreenToken *lparen() const { return static_cast<const GreenToken *>(slot(2)); }
        const GreenNodeList *arguments() const { return static_cast<const GreenNodeList *>(slot(3)); }
        const GreenToken *rparen() const { return static_cast<const GreenToken *>(slot(4)); }
    };

    class GreenInitializerExpressionSyntax final : public GreenNodeN<3> {
    public:
        GreenInitializerExpressionSyntax(
            const GreenToken *lbrace,
            const GreenNodeList *expressions,
            const GreenToken *rbrace,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::InitializerExpressionSyntax, { lbrace, expressions, rbrace }, std::move(diagnostics) ) { }

        const GreenToken *lbrace() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNodeList *expressions() const { return static_cast<const GreenNodeList *>(slot(1)); }
        const GreenToken *rbrace() const { return static_cast<const GreenToken *>(slot(2)); }
    };

    class GreenArrayCreationExpressionSyntax final : public GreenNodeN<6> {
    public:
        GreenArrayCreationExpressionSyntax(
            const GreenToken *kw,
            const GreenNode *type,
            const GreenToken *lbracket,
            const GreenNode *size,
            const GreenToken *rbracket,
            const GreenNode *initializer,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::ArrayCreationExpressionSyntax, { kw, type, lbracket, size, rbracket, initializer }, std::move(diagnostics) ) { }

        const GreenToken *kw() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNode *type() const { return slot(1); }
        const GreenToken *lbracket() const { return static_cast<const GreenToken *>(slot(2)); }
        const GreenNode *size() const { return slot(3); }
        const GreenToken *rbracket() const { return static_cast<const GreenToken *>(slot(4)); }
        const GreenNode *initializer() const { return slot(5); }
    };
}

#endif //VELA_GREEN_NONTERMINALS_H
