#ifndef VELA_GREEN_NONTERMINALS_H
#define VELA_GREEN_NONTERMINALS_H

#include <vector>

#include "green_node_fwds.h"
#include "frontend/diagnostic.h"

namespace vela::parse {
    class GreenCompilationUnitSyntax final : public GreenNodeN<2> {
    public:
        GreenCompilationUnitSyntax(GreenRef<const GreenNode> declarations, GreenRef<const GreenToken> eof, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::CompilationUnitSyntax, { std::move(declarations), std::move(eof) }, std::move(diagnostics)) { }

        const GreenNode *declarations() const { return slot(0); }
        const GreenToken *eof() const { return static_cast<const GreenToken *>(slot(1)); }
    };

    class GreenClassDeclarationSyntax final : public GreenNodeN<3> {
    public:
        GreenClassDeclarationSyntax(
            GreenRef<const GreenToken> kw,
            GreenRef<const GreenToken> identifier,
            GreenRef<const GreenNode> block,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::ClassDeclarationSyntax, { std::move(kw), std::move(identifier), std::move(block) }, std::move(diagnostics)) { }

        const GreenToken *kw() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenToken *identifier() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenNode *block() const { return slot(2); }
    };

    class GreenStructDeclarationSyntax final : public GreenNodeN<3> {
    public:
        GreenStructDeclarationSyntax(
            GreenRef<const GreenToken> kw,
            GreenRef<const GreenToken> identifier,
            GreenRef<const GreenNode> block,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::StructDeclarationSyntax, { std::move(kw), std::move(identifier), std::move(block) }, std::move(diagnostics)) { }

        const GreenToken *kw() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenToken *identifier() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenNode *block() const { return slot(2); }
    };

    class GreenFieldDeclarationSyntax final : public GreenNodeN<2> {
    public:
        GreenFieldDeclarationSyntax(GreenRef<const GreenNode> declaration, GreenRef<const GreenToken> semicolon, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::FieldDeclarationSyntax, { std::move(declaration), std::move(semicolon) }, std::move(diagnostics)) { }

        const GreenNode *declaration() const { return slot(0); }
        const GreenToken *semicolon() const { return static_cast<const GreenToken *>(slot(1)); }
    };

    class GreenMethodDeclarationSyntax final : public GreenNodeN<7> {
    public:
        GreenMethodDeclarationSyntax(
            GreenRef<const GreenToken> kw,
            GreenRef<const GreenToken> ret,
            GreenRef<const GreenToken> identifier,
            GreenRef<const GreenToken> lparen,
            GreenRef<const GreenNodeList> parameters,
            GreenRef<const GreenToken> rparen,
            GreenRef<const GreenNode> block,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::MethodDeclarationSyntax,
                    { std::move(kw), std::move(ret), std::move(identifier), std::move(lparen), std::move(parameters), std::move(rparen), std::move(block) },
                    std::move(diagnostics)) { }

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
        GreenParameterSyntax(GreenRef<const GreenToken> type, GreenRef<const GreenToken> identifier, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::ParameterSyntax, { std::move(type), std::move(identifier) }, std::move(diagnostics)) { }

        const GreenToken *type() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenToken *identifier() const { return static_cast<const GreenToken *>(slot(1)); }
    };

    class GreenVariableDeclarationSyntax final : public GreenNodeN<4> {
    public:
        GreenVariableDeclarationSyntax(
            GreenRef<const GreenToken> type,
            GreenRef<const GreenToken> identifier,
            GreenRef<const GreenToken> assign,
            GreenRef<const GreenNode> initializer,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::VariableDeclarationSyntax, { std::move(type), std::move(identifier), std::move(assign), std::move(initializer) }, std::move(diagnostics)) { }

        const GreenToken *type() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenToken *identifier() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenToken *assign() const { return static_cast<const GreenToken *>(slot(2)); }
        const GreenNode *initializer() const { return slot(3); }
    };

    class GreenPredefinedTypeSyntax final : public GreenNodeN<1> {
    public:
        explicit GreenPredefinedTypeSyntax(GreenRef<const GreenToken> type, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::PredefinedTypeSyntax, { std::move(type) }, std::move(diagnostics)) { }

        const GreenToken *type() const { return static_cast<const GreenToken *>(slot(0)); }
    };

    class GreenIdentifierNameSyntax final : public GreenNodeN<1> {
    public:
        explicit GreenIdentifierNameSyntax(GreenRef<const GreenToken> identifier, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::IdentifierNameSyntax, { std::move(identifier) }, std::move(diagnostics)) { }

        const GreenToken *identifier() const { return static_cast<const GreenToken *>(slot(0)); }
    };

    class GreenArrayTypeSyntax final : public GreenNodeN<3> {
    public:
        GreenArrayTypeSyntax(
            GreenRef<const GreenToken> type,
            GreenRef<const GreenToken> lbracket,
            GreenRef<const GreenToken> rbracket,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::ArrayTypeSyntax, { std::move(type), std::move(lbracket), std::move(rbracket) }, std::move(diagnostics)) { }

        const GreenToken *elementType() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenToken *lbracket() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenToken *rbracket() const { return static_cast<const GreenToken *>(slot(2)); }
    };

    class GreenBlockSyntax final : public GreenNodeN<4> {
    public:
        GreenBlockSyntax(
            GreenRef<const GreenToken> lbrace,
            GreenRef<const GreenNodeList> statements,
            GreenRef<const GreenNodeList> declarations,
            GreenRef<const GreenToken> rbrace,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::BlockSyntax, { std::move(lbrace), std::move(statements), std::move(declarations), std::move(rbrace) }, std::move(diagnostics)) { }

        const GreenToken *lbrace() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNodeList *statements() const { return static_cast<const GreenNodeList *>(slot(1)); }
        const GreenNodeList *declarations() const { return static_cast<const GreenNodeList *>(slot(2)); }
        const GreenToken *rbrace() const { return static_cast<const GreenToken *>(slot(3)); }
    };

    class GreenLocalDeclarationStatementSyntax final : public GreenNodeN<2> {
    public:
        GreenLocalDeclarationStatementSyntax(GreenRef<const GreenNode> declaration, GreenRef<const GreenToken> semicolon, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::LocalDeclarationStatementSyntax, { std::move(declaration), std::move(semicolon) }, std::move(diagnostics)) { }

        const GreenNode *declaration() const { return slot(0); }
        const GreenToken *semicolon() const { return static_cast<const GreenToken *>(slot(1)); }
    };

    class GreenExpressionStatementSyntax final : public GreenNodeN<2> {
    public:
        GreenExpressionStatementSyntax(GreenRef<const GreenNode> expression, GreenRef<const GreenToken> semicolon, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::ExpressionStatementSyntax, { std::move(expression), std::move(semicolon) }, std::move(diagnostics)) { }

        const GreenNode *expression() const { return slot(0); }
        const GreenToken *semicolon() const { return static_cast<const GreenToken *>(slot(1)); }
    };

    class GreenIfStatementSyntax final : public GreenNodeN<7> {
    public:
        GreenIfStatementSyntax(
            GreenRef<const GreenToken> kwIf,
            GreenRef<const GreenToken> lparen,
            GreenRef<const GreenNode> condition,
            GreenRef<const GreenToken> rparen,
            GreenRef<const GreenNode> statement,
            GreenRef<const GreenToken> kwElse,
            GreenRef<const GreenNode> elseStatement,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::IfStatementSyntax,
                    { std::move(kwIf), std::move(lparen), std::move(condition), std::move(rparen), std::move(statement), std::move(kwElse), std::move(elseStatement) },
                    std::move(diagnostics) ) { }

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
            GreenRef<const GreenToken> kw,
            GreenRef<const GreenToken> lparen,
            GreenRef<const GreenNode> condition,
            GreenRef<const GreenToken> rparen,
            GreenRef<const GreenNode> statement,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::WhileStatementSyntax,
                    { std::move(kw), std::move(lparen), std::move(condition), std::move(rparen), std::move(statement) },
                    std::move(diagnostics) ) { }

        const GreenToken *kw() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenToken *lparen() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenNode *condition() const { return slot(2); }
        const GreenToken *rparen() const { return static_cast<const GreenToken *>(slot(3)); }
        const GreenNode *statement() const { return slot(4); }
    };

    class GreenForStatementSyntax final : public GreenNodeN<10> {
    public:
        GreenForStatementSyntax(
            GreenRef<const GreenToken> kw,
            GreenRef<const GreenToken> lparen,
            GreenRef<const GreenNode> initializer,
            GreenRef<const GreenNodeList> initializers,
            GreenRef<const GreenToken> semi1,
            GreenRef<const GreenNode> condition,
            GreenRef<const GreenToken> semi2,
            GreenRef<const GreenNodeList> incrementors,
            GreenRef<const GreenToken> rparen,
            GreenRef<const GreenNode> statement,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::ForStatementSyntax,
                    { std::move(kw), std::move(lparen), std::move(initializer), std::move(initializers), std::move(semi1),
                      std::move(condition), std::move(semi2), std::move(incrementors), std::move(rparen), std::move(statement) },
                    std::move(diagnostics) ) { }

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
            GreenRef<const GreenToken> kw,
            GreenRef<const GreenNode> expression,
            GreenRef<const GreenToken> semicolon,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::ReturnStatementSyntax, { std::move(kw), std::move(expression), std::move(semicolon) }, std::move(diagnostics) ) { }

        const GreenToken *kw() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNode *expression() const { return slot(1); }
        const GreenToken *semicolon() const { return static_cast<const GreenToken *>(slot(2)); }
    };

    class GreenLiteralExpressionSyntax final : public GreenNodeN<1> {
    public:
        explicit GreenLiteralExpressionSyntax(GreenRef<const GreenToken> literal, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::LiteralExpressionSyntax, { std::move(literal) }, std::move(diagnostics) ) { }

        const GreenToken *literal() const { return static_cast<const GreenToken *>(slot(0)); }
    };

    class GreenThisExpressionSyntax final : public GreenNodeN<1> {
    public:
        explicit GreenThisExpressionSyntax(GreenRef<const GreenToken> kw, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::ThisExpressionSyntax, { std::move(kw) }, std::move(diagnostics) ) { }

        const GreenToken *kw() const { return static_cast<const GreenToken *>(slot(0)); }
    };

    class GreenPrefixUnaryExpressionSyntax final : public GreenNodeN<2> {
    public:
        GreenPrefixUnaryExpressionSyntax(GreenRef<const GreenToken> op, GreenRef<const GreenNode> operand, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::PrefixUnaryExpressionSyntax, { std::move(op), std::move(operand) }, std::move(diagnostics) ) { }

        const GreenToken *op() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNode *operand() const { return slot(1); }
    };

    class GreenPostfixUnaryExpressionSyntax final : public GreenNodeN<2> {
    public:
        GreenPostfixUnaryExpressionSyntax(GreenRef<const GreenToken> op, GreenRef<const GreenNode> operand, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::PostfixUnaryExpressionSyntax, { std::move(op), std::move(operand) }, std::move(diagnostics) ) { }

        const GreenToken *op() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNode *operand() const { return slot(1); }
    };

    class GreenBinaryExpressionSyntax final : public GreenNodeN<3> {
    public:
        GreenBinaryExpressionSyntax(
            GreenRef<const GreenNode> lhs,
            GreenRef<const GreenToken> op,
            GreenRef<const GreenNode> rhs,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::BinaryExpressionSyntax, { std::move(lhs), std::move(op), std::move(rhs) }, std::move(diagnostics) ) { }

        const GreenNode *lhs() const { return slot(0); }
        const GreenToken *op() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenNode *rhs() const { return slot(2); }
    };

    class GreenAssignmentExpressionSyntax final : public GreenNodeN<3> {
    public:
        GreenAssignmentExpressionSyntax(
            GreenRef<const GreenNode> lhs,
            GreenRef<const GreenToken> assignment,
            GreenRef<const GreenNode> rhs,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::AssignmentExpressionSyntax, { std::move(lhs), std::move(assignment), std::move(rhs) }, std::move(diagnostics) ) { }

        const GreenNode *lhs() const { return slot(0); }
        const GreenToken *assignment() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenNode *rhs() const { return slot(2); }
    };

    class GreenInvocationExpressionSyntax final : public GreenNodeN<4> {
    public:
        GreenInvocationExpressionSyntax(
            GreenRef<const GreenNode> expression,
            GreenRef<const GreenToken> lparen,
            GreenRef<const GreenNodeList> arguments,
            GreenRef<const GreenToken> rparen,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::InvocationExpressionSyntax, { std::move(expression), std::move(lparen), std::move(arguments), std::move(rparen) }, std::move(diagnostics) ) { }

        const GreenNode *expression() const { return slot(0); }
        const GreenToken *lparen() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenNodeList *arguments() const { return static_cast<const GreenNodeList *>(slot(2)); }
        const GreenToken *rparen() const { return static_cast<const GreenToken *>(slot(3)); }
    };

    class GreenElementAccessExpressionSyntax final : public GreenNodeN<4> {
    public:
        GreenElementAccessExpressionSyntax(
            GreenRef<const GreenNode> expression,
            GreenRef<const GreenToken> lbracket,
            GreenRef<const GreenNode> index,
            GreenRef<const GreenToken> rbracket,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::ElementAccessExpressionSyntax, { std::move(expression), std::move(lbracket), std::move(index), std::move(rbracket) }, std::move(diagnostics) ) { }

        const GreenNode *expression() const { return slot(0); }
        const GreenToken *lbracket() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenNode *index() const { return slot(2); }
        const GreenToken *rbracket() const { return static_cast<const GreenToken *>(slot(3)); }
    };

    class GreenObjectCreationExpressionSyntax final : public GreenNodeN<5> {
    public:
        GreenObjectCreationExpressionSyntax(
            GreenRef<const GreenToken> kw,
            GreenRef<const GreenNode> type,
            GreenRef<const GreenToken> lparen,
            GreenRef<const GreenNodeList> arguments,
            GreenRef<const GreenToken> rparen,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::ObjectCreationExpressionSyntax, { std::move(kw), std::move(type), std::move(lparen), std::move(arguments), std::move(rparen) }, std::move(diagnostics) ) { }

        const GreenToken *kw() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNode *type() const { return slot(1); }
        const GreenToken *lparen() const { return static_cast<const GreenToken *>(slot(2)); }
        const GreenNodeList *arguments() const { return static_cast<const GreenNodeList *>(slot(3)); }
        const GreenToken *rparen() const { return static_cast<const GreenToken *>(slot(4)); }
    };

    class GreenInitializerExpressionSyntax final : public GreenNodeN<3> {
    public:
        GreenInitializerExpressionSyntax(
            GreenRef<const GreenToken> lbrace,
            GreenRef<const GreenNodeList> expressions,
            GreenRef<const GreenToken> rbrace,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::InitializerExpressionSyntax, { std::move(lbrace), std::move(expressions), std::move(rbrace) }, std::move(diagnostics) ) { }

        const GreenToken *lbrace() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNodeList *expressions() const { return static_cast<const GreenNodeList *>(slot(1)); }
        const GreenToken *rbrace() const { return static_cast<const GreenToken *>(slot(2)); }
    };

    class GreenArrayCreationExpressionSyntax final : public GreenNodeN<6> {
    public:
        GreenArrayCreationExpressionSyntax(
            GreenRef<const GreenToken> kw,
            GreenRef<const GreenNode> type,
            GreenRef<const GreenToken> lbracket,
            GreenRef<const GreenNode> size,
            GreenRef<const GreenToken> rbracket,
            GreenRef<const GreenNode> initializer,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::ArrayCreationExpressionSyntax,
                    { std::move(kw), std::move(type), std::move(lbracket), std::move(size), std::move(rbracket), std::move(initializer) },
                    std::move(diagnostics) ) { }

        const GreenToken *kw() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNode *type() const { return slot(1); }
        const GreenToken *lbracket() const { return static_cast<const GreenToken *>(slot(2)); }
        const GreenNode *size() const { return slot(3); }
        const GreenToken *rbracket() const { return static_cast<const GreenToken *>(slot(4)); }
        const GreenNode *initializer() const { return slot(5); }
    };
}

#endif //VELA_GREEN_NONTERMINALS_H