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

    // '<' templateParam ( ',' templateParam )* '>'
    class GreenTemplateParameterListSyntax final : public GreenNodeN<3> {
    public:
        GreenTemplateParameterListSyntax(
            GreenRef<const GreenToken> lt,
            GreenRef<const GreenNodeList> parameters,
            GreenRef<const GreenToken> gt,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::TemplateParameterListSyntax, { std::move(lt), std::move(parameters), std::move(gt) }, std::move(diagnostics)) { }

        const GreenToken *lt() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNodeList *parameters() const { return static_cast<const GreenNodeList *>(slot(1)); }
        const GreenToken *gt() const { return static_cast<const GreenToken *>(slot(2)); }
    };

    // typeVariable '...'? ( '=' templateArg )?       — $T, Number $T, $Ts..., $F<$>
    // type '...'? IDENTIFIER ( '=' templateArg )?    — int N, int... Ns
    class GreenTemplateParameterSyntax final : public GreenNodeN<5> {
    public:
        GreenTemplateParameterSyntax(
            GreenRef<const GreenNode> type,
            GreenRef<const GreenToken> ellipsis,
            GreenRef<const GreenToken> identifier,
            GreenRef<const GreenToken> assign,
            GreenRef<const GreenNode> defaultArgument,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::TemplateParameterSyntax,
                    { std::move(type), std::move(ellipsis), std::move(identifier), std::move(assign), std::move(defaultArgument) },
                    std::move(diagnostics)) { }

        const GreenNode *type() const { return slot(0); }
        const GreenToken *ellipsis() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenToken *identifier() const { return static_cast<const GreenToken *>(slot(2)); }
        const GreenToken *assign() const { return static_cast<const GreenToken *>(slot(3)); }
        const GreenNode *defaultArgument() const { return slot(4); }
    };

    // '<' templateArg ( ',' templateArg )* '>'
    class GreenTemplateArgumentListSyntax final : public GreenNodeN<3> {
    public:
        GreenTemplateArgumentListSyntax(
            GreenRef<const GreenToken> lt,
            GreenRef<const GreenNodeList> arguments,
            GreenRef<const GreenToken> gt,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::TemplateArgumentListSyntax, { std::move(lt), std::move(arguments), std::move(gt) }, std::move(diagnostics)) { }

        const GreenToken *lt() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNodeList *arguments() const { return static_cast<const GreenNodeList *>(slot(1)); }
        const GreenToken *gt() const { return static_cast<const GreenToken *>(slot(2)); }
    };

    // ( type | bitwise_or ) — represented via PackExpansionSyntax
    class GreenTemplateArgumentSyntax final : public GreenNodeN<1> {
    public:
        explicit GreenTemplateArgumentSyntax(GreenRef<const GreenNode> value, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::TemplateArgumentSyntax, { std::move(value) }, std::move(diagnostics)) { }

        const GreenNode *value() const { return slot(0); }
    };

    // IDENTIFIER templateArgs
    class GreenTemplateNameSyntax final : public GreenNodeN<2> {
    public:
        GreenTemplateNameSyntax(GreenRef<const GreenToken> identifier, GreenRef<const GreenNode> argumentList, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::TemplateNameSyntax, { std::move(identifier), std::move(argumentList) }, std::move(diagnostics)) { }

        const GreenToken *identifier() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNode *argumentList() const { return slot(1); }
    };

    // 'where' '(' expression ')'
    class GreenConstraintClauseSyntax final : public GreenNodeN<4> {
    public:
        GreenConstraintClauseSyntax(
            GreenRef<const GreenToken> kwWhere,
            GreenRef<const GreenToken> lparen,
            GreenRef<const GreenNode> condition,
            GreenRef<const GreenToken> rparen,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::ConstraintClauseSyntax, { std::move(kwWhere), std::move(lparen), std::move(condition), std::move(rparen) }, std::move(diagnostics)) { }

        const GreenToken *kwWhere() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenToken *lparen() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenNode *condition() const { return slot(2); }
        const GreenToken *rparen() const { return static_cast<const GreenToken *>(slot(3)); }
    };

    // typeName? '$' IDENTIFIER? templateArgs? — e.g. $T, Number $T, $F<$, int>
    // The identifier is null for an unnamed slot ('$') inside another type variable's argument list.
    class GreenTypeVariableSyntax final : public GreenNodeN<4> {
    public:
        GreenTypeVariableSyntax(
            GreenRef<const GreenNode> constraint,
            GreenRef<const GreenToken> dollar,
            GreenRef<const GreenToken> identifier,
            GreenRef<const GreenNode> argumentList,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::TypeVariableSyntax,
                    { std::move(constraint), std::move(dollar), std::move(identifier), std::move(argumentList) },
                    std::move(diagnostics)) { }

        const GreenNode *constraint() const { return slot(0); }
        const GreenToken *dollar() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenToken *identifier() const { return static_cast<const GreenToken *>(slot(2)); }
        const GreenNode *argumentList() const { return slot(3); }
    };

    // type '.' IDENTIFIER
    class GreenQualifiedNameSyntax final : public GreenNodeN<3> {
    public:
        GreenQualifiedNameSyntax(
            GreenRef<const GreenNode> left,
            GreenRef<const GreenToken> dot,
            GreenRef<const GreenNode> right,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::QualifiedNameSyntax, { std::move(left), std::move(dot), std::move(right) }, std::move(diagnostics)) { }

        const GreenNode *left() const { return slot(0); }
        const GreenToken *dot() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenNode *right() const { return slot(2); }
    };

    // type '[' ( expression | range ) ']'
    class GreenPackIndexTypeSyntax final : public GreenNodeN<4> {
    public:
        GreenPackIndexTypeSyntax(
            GreenRef<const GreenNode> pack,
            GreenRef<const GreenToken> lbracket,
            GreenRef<const GreenNode> index,
            GreenRef<const GreenToken> rbracket,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::PackIndexTypeSyntax, { std::move(pack), std::move(lbracket), std::move(index), std::move(rbracket) }, std::move(diagnostics)) { }

        const GreenNode *pack() const { return slot(0); }
        const GreenToken *lbracket() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenNode *index() const { return slot(2); }
        const GreenToken *rbracket() const { return static_cast<const GreenToken *>(slot(3)); }
    };

    // 'concept' IDENTIFIER templateParams ( ':' conceptRef ( ',' conceptRef )* )? ( '=>' expression ';' | requirementBlock )
    class GreenConceptDeclarationSyntax final : public GreenNodeN<7> {
    public:
        GreenConceptDeclarationSyntax(
            GreenRef<const GreenToken> kw,
            GreenRef<const GreenToken> identifier,
            GreenRef<const GreenNode> templateParameters,
            GreenRef<const GreenToken> colon,
            GreenRef<const GreenNodeList> refinements,
            GreenRef<const GreenNode> body,
            GreenRef<const GreenToken> semicolon,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::ConceptDeclarationSyntax,
                    { std::move(kw), std::move(identifier), std::move(templateParameters), std::move(colon),
                      std::move(refinements), std::move(body), std::move(semicolon) },
                    std::move(diagnostics)) { }

        const GreenToken *kw() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenToken *identifier() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenNode *templateParameters() const { return slot(2); }
        const GreenToken *colon() const { return static_cast<const GreenToken *>(slot(3)); }
        const GreenNodeList *refinements() const { return static_cast<const GreenNodeList *>(slot(4)); }
        const GreenNode *body() const { return slot(5); }
        const GreenToken *semicolon() const { return static_cast<const GreenToken *>(slot(6)); }
    };

    // '=>' expression
    class GreenArrowExpressionClauseSyntax final : public GreenNodeN<2> {
    public:
        GreenArrowExpressionClauseSyntax(GreenRef<const GreenToken> arrow, GreenRef<const GreenNode> expression, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::ArrowExpressionClauseSyntax, { std::move(arrow), std::move(expression) }, std::move(diagnostics)) { }

        const GreenToken *arrow() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNode *expression() const { return slot(1); }
    };

    // '{' requirement* '}'
    class GreenRequirementBlockSyntax final : public GreenNodeN<3> {
    public:
        GreenRequirementBlockSyntax(
            GreenRef<const GreenToken> lbrace,
            GreenRef<const GreenNodeList> requirements,
            GreenRef<const GreenToken> rbrace,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::RequirementBlockSyntax, { std::move(lbrace), std::move(requirements), std::move(rbrace) }, std::move(diagnostics)) { }

        const GreenToken *lbrace() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNodeList *requirements() const { return static_cast<const GreenNodeList *>(slot(1)); }
        const GreenToken *rbrace() const { return static_cast<const GreenToken *>(slot(2)); }
    };

    // requirementBody ( '->' type )? ';'
    class GreenRequirementSyntax final : public GreenNodeN<5> {
    public:
        GreenRequirementSyntax(
            GreenRef<const GreenToken> colon,
            GreenRef<const GreenNode> body,
            GreenRef<const GreenToken> arrow,
            GreenRef<const GreenNode> resultType,
            GreenRef<const GreenToken> semicolon,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::RequirementSyntax,
                    { std::move(colon), std::move(body), std::move(arrow), std::move(resultType), std::move(semicolon) },
                    std::move(diagnostics)) { }

        const GreenToken *colon() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNode *body() const { return slot(1); }
        const GreenToken *arrow() const { return static_cast<const GreenToken *>(slot(2)); }
        const GreenNode *resultType() const { return slot(3); }
        const GreenToken *semicolon() const { return static_cast<const GreenToken *>(slot(4)); }
    };

    // '$' IDENTIFIER '.' IDENTIFIER
    class GreenAssociatedTypeRequirementSyntax final : public GreenNodeN<2> {
    public:
        GreenAssociatedTypeRequirementSyntax(GreenRef<const GreenToken> dollar, GreenRef<const GreenNode> name, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::AssociatedTypeRequirementSyntax, { std::move(dollar), std::move(name) }, std::move(diagnostics)) { }

        const GreenToken *dollar() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNode *name() const { return slot(1); }
    };

    // 'fn' returnType IDENTIFIER '.' IDENTIFIER '(' typeList? ')'
    class GreenMemberFunctionRequirementSyntax final : public GreenNodeN<6> {
    public:
        GreenMemberFunctionRequirementSyntax(
            GreenRef<const GreenToken> kwFn,
            GreenRef<const GreenNode> returnType,
            GreenRef<const GreenNode> name,
            GreenRef<const GreenToken> lparen,
            GreenRef<const GreenNodeList> parameterTypes,
            GreenRef<const GreenToken> rparen,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::MemberFunctionRequirementSyntax,
                    { std::move(kwFn), std::move(returnType), std::move(name), std::move(lparen), std::move(parameterTypes), std::move(rparen) },
                    std::move(diagnostics)) { }

        const GreenToken *kwFn() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNode *returnType() const { return slot(1); }
        const GreenNode *name() const { return slot(2); }
        const GreenToken *lparen() const { return static_cast<const GreenToken *>(slot(3)); }
        const GreenNodeList *parameterTypes() const { return static_cast<const GreenNodeList *>(slot(4)); }
        const GreenToken *rparen() const { return static_cast<const GreenToken *>(slot(5)); }
    };

    // expression '.' ( IDENTIFIER templateArgs? )
    class GreenMemberAccessExpressionSyntax final : public GreenNodeN<3> {
    public:
        GreenMemberAccessExpressionSyntax(
            GreenRef<const GreenNode> expression,
            GreenRef<const GreenToken> dot,
            GreenRef<const GreenNode> name,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::MemberAccessExpressionSyntax, { std::move(expression), std::move(dot), std::move(name) }, std::move(diagnostics)) { }

        const GreenNode *expression() const { return slot(0); }
        const GreenToken *dot() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenNode *name() const { return slot(2); }
    };

    // operand '...'
    class GreenPackExpansionSyntax final : public GreenNodeN<2> {
    public:
        GreenPackExpansionSyntax(GreenRef<const GreenNode> operand, GreenRef<const GreenToken> ellipsis, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::PackExpansionSyntax, { std::move(operand), std::move(ellipsis) }, std::move(diagnostics)) { }

        const GreenNode *operand() const { return slot(0); }
        const GreenToken *ellipsis() const { return static_cast<const GreenToken *>(slot(1)); }
    };

    // '(' '...' binaryOp unary ')'
    class GreenLeftFoldExpressionSyntax final : public GreenNodeN<5> {
    public:
        GreenLeftFoldExpressionSyntax(
            GreenRef<const GreenToken> lparen,
            GreenRef<const GreenToken> ellipsis,
            GreenRef<const GreenToken> op,
            GreenRef<const GreenNode> operand,
            GreenRef<const GreenToken> rparen,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::LeftFoldExpressionSyntax,
                    { std::move(lparen), std::move(ellipsis), std::move(op), std::move(operand), std::move(rparen) },
                    std::move(diagnostics)) { }

        const GreenToken *lparen() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenToken *ellipsis() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenToken *op() const { return static_cast<const GreenToken *>(slot(2)); }
        const GreenNode *operand() const { return slot(3); }
        const GreenToken *rparen() const { return static_cast<const GreenToken *>(slot(4)); }
    };

    // '(' unary binaryOp '...' ( binaryOp unary )? ')'
    class GreenRightFoldExpressionSyntax final : public GreenNodeN<7> {
    public:
        GreenRightFoldExpressionSyntax(
            GreenRef<const GreenToken> lparen,
            GreenRef<const GreenNode> operand,
            GreenRef<const GreenToken> op,
            GreenRef<const GreenToken> ellipsis,
            GreenRef<const GreenToken> secondOp,
            GreenRef<const GreenNode> secondOperand,
            GreenRef<const GreenToken> rparen,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::RightFoldExpressionSyntax,
                    { std::move(lparen), std::move(operand), std::move(op), std::move(ellipsis),
                      std::move(secondOp), std::move(secondOperand), std::move(rparen) },
                    std::move(diagnostics)) { }

        const GreenToken *lparen() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNode *operand() const { return slot(1); }
        const GreenToken *op() const { return static_cast<const GreenToken *>(slot(2)); }
        const GreenToken *ellipsis() const { return static_cast<const GreenToken *>(slot(3)); }
        const GreenToken *secondOp() const { return static_cast<const GreenToken *>(slot(4)); }
        const GreenNode *secondOperand() const { return slot(5); }
        const GreenToken *rparen() const { return static_cast<const GreenToken *>(slot(6)); }
    };

    // '(' ( binaryOp | '!' | '~' ) ')'
    class GreenOperatorReferenceExpressionSyntax final : public GreenNodeN<3> {
    public:
        GreenOperatorReferenceExpressionSyntax(
            GreenRef<const GreenToken> lparen,
            GreenRef<const GreenToken> op,
            GreenRef<const GreenToken> rparen,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::OperatorReferenceExpressionSyntax, { std::move(lparen), std::move(op), std::move(rparen) }, std::move(diagnostics)) { }

        const GreenToken *lparen() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenToken *op() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenToken *rparen() const { return static_cast<const GreenToken *>(slot(2)); }
    };

    // logic_or? '..' logic_or?
    class GreenRangeExpressionSyntax final : public GreenNodeN<3> {
    public:
        GreenRangeExpressionSyntax(
            GreenRef<const GreenNode> lower,
            GreenRef<const GreenToken> dotDot,
            GreenRef<const GreenNode> upper,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::RangeExpressionSyntax, { std::move(lower), std::move(dotDot), std::move(upper) }, std::move(diagnostics)) { }

        const GreenNode *lower() const { return slot(0); }
        const GreenToken *dotDot() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenNode *upper() const { return slot(2); }
    };

    // '(' ')' | '(' templateArg ',' ( templateArg ( ',' templateArg )* ','? )? ')'
    class GreenPackLiteralExpressionSyntax final : public GreenNodeN<3> {
    public:
        GreenPackLiteralExpressionSyntax(
            GreenRef<const GreenToken> lparen,
            GreenRef<const GreenNodeList> elements,
            GreenRef<const GreenToken> rparen,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::PackLiteralExpressionSyntax, { std::move(lparen), std::move(elements), std::move(rparen) }, std::move(diagnostics)) { }

        const GreenToken *lparen() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNodeList *elements() const { return static_cast<const GreenNodeList *>(slot(1)); }
        const GreenToken *rparen() const { return static_cast<const GreenToken *>(slot(2)); }
    };

    // 'comptime' ( typeVariable '...'? | type '...'? IDENTIFIER ) '=' expression ';'
    // A type binder's name lives in its TypeVariableSyntax, so 'identifier' is null for it.
    class GreenComptimeVariableDeclarationSyntax final : public GreenNodeN<7> {
    public:
        GreenComptimeVariableDeclarationSyntax(
            GreenRef<const GreenToken> kwComptime,
            GreenRef<const GreenNode> type,
            GreenRef<const GreenToken> ellipsis,
            GreenRef<const GreenToken> identifier,
            GreenRef<const GreenToken> assign,
            GreenRef<const GreenNode> initializer,
            GreenRef<const GreenToken> semicolon,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::ComptimeVariableDeclarationSyntax,
                    { std::move(kwComptime), std::move(type), std::move(ellipsis), std::move(identifier),
                      std::move(assign), std::move(initializer), std::move(semicolon) },
                    std::move(diagnostics)) { }

        const GreenToken *kwComptime() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNode *type() const { return slot(1); }
        const GreenToken *ellipsis() const { return static_cast<const GreenToken *>(slot(2)); }
        const GreenToken *identifier() const { return static_cast<const GreenToken *>(slot(3)); }
        const GreenToken *assign() const { return static_cast<const GreenToken *>(slot(4)); }
        const GreenNode *initializer() const { return slot(5); }
        const GreenToken *semicolon() const { return static_cast<const GreenToken *>(slot(6)); }
    };

    // 'comptime' 'if' '(' expression ')' statement ( 'else' statement )?
    class GreenComptimeIfStatementSyntax final : public GreenNodeN<8> {
    public:
        GreenComptimeIfStatementSyntax(
            GreenRef<const GreenToken> kwComptime,
            GreenRef<const GreenToken> kwIf,
            GreenRef<const GreenToken> lparen,
            GreenRef<const GreenNode> condition,
            GreenRef<const GreenToken> rparen,
            GreenRef<const GreenNode> statement,
            GreenRef<const GreenToken> kwElse,
            GreenRef<const GreenNode> elseStatement,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::ComptimeIfStatementSyntax,
                    { std::move(kwComptime), std::move(kwIf), std::move(lparen), std::move(condition),
                      std::move(rparen), std::move(statement), std::move(kwElse), std::move(elseStatement) },
                    std::move(diagnostics)) { }

        const GreenToken *kwComptime() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenToken *kwIf() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenToken *lparen() const { return static_cast<const GreenToken *>(slot(2)); }
        const GreenNode *condition() const { return slot(3); }
        const GreenToken *rparen() const { return static_cast<const GreenToken *>(slot(4)); }
        const GreenNode *statement() const { return slot(5); }
        const GreenToken *kwElse() const { return static_cast<const GreenToken *>(slot(6)); }
        const GreenNode *elseStatement() const { return slot(7); }
    };

    // 'comptime' 'for' '(' ( typeVariable | type IDENTIFIER ) 'in' ( range | expression ) ')' statement
    // A type binder's name lives in its TypeVariableSyntax, so 'identifier' is null for it.
    class GreenComptimeForStatementSyntax final : public GreenNodeN<9> {
    public:
        GreenComptimeForStatementSyntax(
            GreenRef<const GreenToken> kwComptime,
            GreenRef<const GreenToken> kwFor,
            GreenRef<const GreenToken> lparen,
            GreenRef<const GreenNode> type,
            GreenRef<const GreenToken> identifier,
            GreenRef<const GreenToken> kwIn,
            GreenRef<const GreenNode> source,
            GreenRef<const GreenToken> rparen,
            GreenRef<const GreenNode> statement,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::ComptimeForStatementSyntax,
                    { std::move(kwComptime), std::move(kwFor), std::move(lparen), std::move(type), std::move(identifier),
                      std::move(kwIn), std::move(source), std::move(rparen), std::move(statement) },
                    std::move(diagnostics)) { }

        const GreenToken *kwComptime() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenToken *kwFor() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenToken *lparen() const { return static_cast<const GreenToken *>(slot(2)); }
        const GreenNode *type() const { return slot(3); }
        const GreenToken *identifier() const { return static_cast<const GreenToken *>(slot(4)); }
        const GreenToken *kwIn() const { return static_cast<const GreenToken *>(slot(5)); }
        const GreenNode *source() const { return slot(6); }
        const GreenToken *rparen() const { return static_cast<const GreenToken *>(slot(7)); }
        const GreenNode *statement() const { return slot(8); }
    };

    // 'comptime' block
    class GreenComptimeBlockStatementSyntax final : public GreenNodeN<2> {
    public:
        GreenComptimeBlockStatementSyntax(GreenRef<const GreenToken> kwComptime, GreenRef<const GreenNode> block, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNodeN(NodeKind::ComptimeBlockStatementSyntax, { std::move(kwComptime), std::move(block) }, std::move(diagnostics)) { }

        const GreenToken *kwComptime() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenNode *block() const { return slot(1); }
    };

    // 'comptime' 'match' '(' ( type | expression ) ')' '{' matchArm ( ',' matchArm )* ','? '}'
    class GreenComptimeMatchExpressionSyntax final : public GreenNodeN<8> {
    public:
        GreenComptimeMatchExpressionSyntax(
            GreenRef<const GreenToken> kwComptime,
            GreenRef<const GreenToken> kwMatch,
            GreenRef<const GreenToken> lparen,
            GreenRef<const GreenNode> subject,
            GreenRef<const GreenToken> rparen,
            GreenRef<const GreenToken> lbrace,
            GreenRef<const GreenNodeList> arms,
            GreenRef<const GreenToken> rbrace,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::ComptimeMatchExpressionSyntax,
                    { std::move(kwComptime), std::move(kwMatch), std::move(lparen), std::move(subject),
                      std::move(rparen), std::move(lbrace), std::move(arms), std::move(rbrace) },
                    std::move(diagnostics)) { }

        const GreenToken *kwComptime() const { return static_cast<const GreenToken *>(slot(0)); }
        const GreenToken *kwMatch() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenToken *lparen() const { return static_cast<const GreenToken *>(slot(2)); }
        const GreenNode *subject() const { return slot(3); }
        const GreenToken *rparen() const { return static_cast<const GreenToken *>(slot(4)); }
        const GreenToken *lbrace() const { return static_cast<const GreenToken *>(slot(5)); }
        const GreenNodeList *arms() const { return static_cast<const GreenNodeList *>(slot(6)); }
        const GreenToken *rbrace() const { return static_cast<const GreenToken *>(slot(7)); }
    };

    // pattern '=>' expression
    class GreenComptimeMatchArmSyntax final : public GreenNodeN<3> {
    public:
        GreenComptimeMatchArmSyntax(
            GreenRef<const GreenNode> pattern,
            GreenRef<const GreenToken> arrow,
            GreenRef<const GreenNode> expression,
            std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
                : GreenNodeN(NodeKind::ComptimeMatchArmSyntax, { std::move(pattern), std::move(arrow), std::move(expression) }, std::move(diagnostics)) { }

        const GreenNode *pattern() const { return slot(0); }
        const GreenToken *arrow() const { return static_cast<const GreenToken *>(slot(1)); }
        const GreenNode *expression() const { return slot(2); }
    };
}

#endif //VELA_GREEN_NONTERMINALS_H