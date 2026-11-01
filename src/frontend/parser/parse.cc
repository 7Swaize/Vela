#include "parse.h"

#include <span>

namespace vela::parse {
    constexpr int kMaxDepth = 256;

    class Parser {
    public:
        Parser(const std::span<const lex::Token> tokens, shared::ArenaAllocator &arena)
            : m_tokens(tokens), m_arena(arena) { }

        ParseResult run() {
            const std::size_t mark = m_scratch.size();
            while (kind() != lex::TokenKind::Eof) {
                m_scratch.push_back(declaration());
            }
            std::span<ast::Decl *const> decls = take<ast::Decl>(mark);
            ast::Program *program = alloc_node<ast::Program>(ast::Span{0, peek().end}, decls);
            return {.program = program, .nodeCount = m_nextId};
        }

    private:
        struct DepthGuard {
            Parser& m_parser;

            explicit DepthGuard(Parser& parser) : m_parser(parser) {
                if (++m_parser.m_depth > kMaxDepth) {
                    --m_parser.m_depth;
                    // fail
                }
            }

            ~DepthGuard() { --m_parser.m_depth; }
            DepthGuard(const DepthGuard &) = delete;
            DepthGuard &operator=(const DepthGuard &) = delete;
        };

        template <typename T>
        std::span<T *const> take(const std::size_t mark) {
            const std::size_t n = m_scratch.size() - mark;
            const std::span<T *> out = m_arena.make_array<T *>(n);
            for (std::size_t i = 0; i < n; ++i) {
                out[i] = static_cast<T *>(m_scratch.back());
                m_scratch.pop_back();
            }

            return out;
        }

        template <ast::HasNodeKind T, typename... Args>
        T *alloc_node(const ast::Span span, Args &&... args) {
            return m_arena.make<T>(
                ast::Node{.kind = T::Kind, .id = m_nextId++, .span = span},
                std::forward<Args>(args)...
            );
        }

        const lex::Token &peek(const std::size_t k = 0) const {
            return m_tokens[std::min(m_pos + k, m_tokens.size() - 1)];
        }

        const lex::TokenKind &kind(const std::size_t k = 0) const {
            return peek(k).kind;
        }

        lex::Token next() {
            const lex::Token t = m_tokens[m_pos];
            if (m_pos + 1 < m_tokens.size()) {
                ++m_pos;
            }

            m_prevEnd = t.end;
            return t;
        }

        lex::Token expect(const lex::TokenKind k) {
            if (kind() != k) {
                // fail
            }
            return next();
        }

        bool accept(const lex::TokenKind k) {
            if (kind() != k) {
                return false;
            }

            next();
            return true;
        }

        static bool is_prim(const lex::TokenKind kind) {
            return kind == lex::TokenKind::KwInt
                || kind == lex::TokenKind::KwBool
                || kind == lex::TokenKind::KwFloat
                || kind == lex::TokenKind::KwString;
        }

        static bool is_lvalue(const ast::Expr &expr) {
            return expr.kind == ast::NodeKind::Identifier
                || expr.kind == ast::NodeKind::Member
                || expr.kind == ast::NodeKind::Index;
        }

        // LL(2) here
        bool starts_decl() const {
            if (is_prim(kind())) {
                return true;
            }

            if (kind() != lex::TokenKind::Identifier) {
                return true;
            }

            return kind(1) == lex::TokenKind::Identifier
                || (kind(1) == lex::TokenKind::LBracket && kind(2) == lex::TokenKind::RBracket);
        }

        ast::Decl *declaration() {
            switch (kind()) {
                case lex::TokenKind::KwStruct:
                case lex::TokenKind::KwClass:
                    return aggregate();
                case lex::TokenKind::KwFn:
                    return func();
                default:
                    return var_decl();
            }
        }

        ast::Aggregate *aggregate() {
            const lex::Token kw = next();
            const lex::Token name = expect(lex::TokenKind::Identifier);
            expect(lex::TokenKind::LBrace);
            const std::size_t mark = m_scratch.size();

            while (kind() != lex::TokenKind::RBrace && kind() != lex::TokenKind::Eof) {
                if (kind() == lex::TokenKind::KwFn) {
                    m_scratch.push_back(func());
                } else {
                    m_scratch.push_back(field());
                }
            }

            expect(lex::TokenKind::RBrace);
            std::span<ast::Decl *const> decls = take<ast::Decl>(mark);

            return alloc_node<ast::Aggregate>(
                ast::Span{kw.start, m_prevEnd},
                kw.kind == lex::TokenKind::KwClass,
                ast::Span{name.start, name.end},
                decls
            );
        }

        ast::FieldDecl *field() {
            const lex::LexPos start = peek().start;
            const ast::TypeRef *t = type();
            const lex::Token name = expect(lex::TokenKind::Identifier);
            expect(lex::TokenKind::Semicolon);

            return alloc_node<ast::FieldDecl>(
                ast::Span{start, m_prevEnd},
                t,
                ast::Span{name.start, name.end}
            );
        }

        ast::FunctionDecl *func() {
            const lex::Token kw = expect(lex::TokenKind::KwFn);
            ast::TypeRef *retType = return_type();
            const lex::Token name = expect(lex::TokenKind::Identifier);
            expect(lex::TokenKind::LParen);
            const std::size_t mark = m_scratch.size();

            if (kind() != lex::TokenKind::RParen) {
                do {
                    m_scratch.push_back(param());
                } while (accept(lex::TokenKind::Comma));
            }

            expect(lex::TokenKind::LParen);
            const std::span<ast::Parameter *const> params = take<ast::Parameter>(mark);
            ast::Block *body = block();

            return alloc_node<ast::FunctionDecl>(
                ast::Span{kw.start, m_prevEnd},
                retType,
                ast::Span{name.start, name.end},
                params,
                body
            );
        }

        ast::TypeRef *type() {
            const lex::Token token = peek();
            if (!is_prim(token.kind) && token.kind != lex::TokenKind::Identifier) {
                // fail
            }
            next();

            bool isArray = false;
            if (kind() == lex::TokenKind::LBracket && kind(1) == lex::TokenKind::RBracket) {
                next();
                next();
                isArray = true;
            }

            return alloc_node<ast::TypeRef>(
                ast::Span{token.start, m_prevEnd},
                token.kind,
                isArray,
                ast::Span{token.start, token.end}
            );
        }

        ast::TypeRef *return_type() {
            if (kind() == lex::TokenKind::KwVoid) {
                const lex::Token token = next();
                return alloc_node<ast::TypeRef>(
                    ast::Span{token.start, m_prevEnd},
                    token.kind,
                    false,
                    ast::Span{token.start, token.end}
                );
            }

            return type();
        }

        ast::Parameter *param() {
            const lex::LexPos start = peek().start;
            ast::TypeRef *typeRef = type();
            const lex::Token name = expect(lex::TokenKind::Identifier);

            return alloc_node<ast::Parameter>(
                ast::Span{start, m_prevEnd},
                typeRef,
                ast::Span{name.start, name.end}
            );
        }

        ast::Block *block() {
            const lex::Token open = expect(lex::TokenKind::LBrace);
            const std::size_t mark = m_scratch.size();

            while (kind() != lex::TokenKind::RBrace && kind() != lex::TokenKind::Eof) {
                if (starts_decl()) {
                    m_scratch.push_back(var_decl());
                } else {
                    m_scratch.push_back(statement());
                }
            }

            expect(lex::TokenKind::RBrace);
            const std::span<ast::Node *const> items = take<ast::Node>(mark);

            return alloc_node<ast::Block>(
                ast::Span{open.start, m_prevEnd},
                items
            );
        }

        ast::VariableDecl *var_decl() {
            const lex::LexPos start = peek().start;
            ast::TypeRef *typeRef = type();
            const lex::Token name = expect(lex::TokenKind::Identifier);
            ast::Expr *init = nullptr;

            if (accept(lex::TokenKind::Assign)) {
                init = expression();
            }

            expect(lex::TokenKind::Semicolon);

            return alloc_node<ast::VariableDecl>(
                ast::Span{start, m_prevEnd},
                typeRef,
                ast::Span{name.start, name.end},
                init
            );
        }

        ast::Stmt *statement() {
            const DepthGuard guard(*this);
            switch (kind()) {
                case lex::TokenKind::KwIf:
                    return if_stmt();
                case lex::TokenKind::KwFor:
                    return for_stmt();
                case lex::TokenKind::KwWhile:
                    return while_stmt();
                case lex::TokenKind::KwReturn:
                    return return_stmt();
                case lex::TokenKind::LBrace:
                    return block();
                default:
                    return expr_stmt();
            }
        }

        ast::If *if_stmt() {
            const lex::Token kw = expect(lex::TokenKind::KwIf);
            expect(lex::TokenKind::LParen);
            ast::Expr *cond = expression();
            expect(lex::TokenKind::RParen);

            ast::Stmt *thenBranch = statement();
            ast::Stmt *elseBranch = nullptr;

            if (accept(lex::TokenKind::KwElse)) {
                elseBranch = statement();
            }

            return alloc_node<ast::If>(
                ast::Span{kw.start, m_prevEnd},
                cond,
                thenBranch,
                elseBranch
            );
        }

        ast::For *for_stmt() {
            const lex::Token kw = expect(lex::TokenKind::KwFor);
            expect(lex::TokenKind::LParen);

            ast::Node *init = nullptr;
            if (!accept(lex::TokenKind::Semicolon)) {
                if (starts_decl()) {
                    init = var_decl();
                } else {
                    init = expr_stmt();
                }
            }

            ast::Expr *cond = nullptr;
            if (!accept(lex::TokenKind::Semicolon)) {
                cond = expression();
            }
            expect(lex::TokenKind::Semicolon);

            ast::Expr *step = nullptr;
            if (kind() != lex::TokenKind::RParen) {
                step = expression();
            }
            expect(lex::TokenKind::RParen);

            ast::Stmt *body = statement();
            return alloc_node<ast::For>(
                ast::Span{kw.start, m_prevEnd},
                init,
                cond,
                step,
                body
            );
        }

        ast::While *while_stmt() {
            const lex::Token kw = expect(lex::TokenKind::KwWhile);
            expect(lex::TokenKind::LParen);
            ast::Expr *cond = expression();
            expect(lex::TokenKind::RParen);
            ast::Stmt *body = statement();

            return alloc_node<ast::While>(
                ast::Span{kw.start, m_prevEnd},
                cond,
                body
            );
        }

        ast::Return *return_stmt() {
            const lex::Token kw = expect(lex::TokenKind::KwReturn);

            ast::Expr *expr = nullptr;
            if (kind() != lex::TokenKind::Semicolon) {
                expr = expression();
            }
            expect(lex::TokenKind::Semicolon);

            return alloc_node<ast::Return>(
                ast::Span{kw.start, m_prevEnd},
                expr
            );
        }

        ast::ExprStmt *expr_stmt() {
            const lex::LexPos start = peek().start;
            ast::Expr *expr = expression();
            expect(lex::TokenKind::Semicolon);

            return alloc_node<ast::ExprStmt>(
                ast::Span{start, m_prevEnd},
                expr
            );
        }

        ast::Expr *expression() {
            return assignment();
        }

        // := logic_or ( "=" assignment )?
        ast::Expr *assignment() {
            const DepthGuard guard(*this);
            ast::Expr *lhs = binary(1);

            if (kind() == lex::TokenKind::Assign) {
                const lex::Token eq = next();
                if (!is_lvalue(*lhs)) {
                    // fail
                }
                ast::Expr *rhs = assignment();

                return alloc_node<ast::Assign>(
                    ast::Span{lhs->span.start, rhs->span.end},
                    lhs,
                    rhs
                );
            }

            return lhs;
        }

        // Operator precedence - higher number binds tighter
        static constexpr int prec(const lex::TokenKind kind) {
            switch (kind) {
                case lex::TokenKind::PipePipe: return 1;
                case lex::TokenKind::AmpAmp: return 2;
                case lex::TokenKind::EqEq:
                case lex::TokenKind::ExclEq: return 3;
                case lex::TokenKind::Lt:
                case lex::TokenKind::Gt:
                case lex::TokenKind::LtEq:
                case lex::TokenKind::GtEq: return 4;
                case lex::TokenKind::Pipe: return 5;
                case lex::TokenKind::Caret: return 6;
                case lex::TokenKind::Amp: return 7;
                case lex::TokenKind::LtLt:
                case lex::TokenKind::GtGt: return 8;
                case lex::TokenKind::Plus:
                case lex::TokenKind::Minus: return 9;
                case lex::TokenKind::Star:
                case lex::TokenKind::Slash:
                case lex::TokenKind::Percent: return 10;
                default: return 0;
            }
        }

        ast::Expr *binary(const int minPrec) {
            ast::Expr *left = unary();

            while (true) {
                if (const int p = prec(kind()); p == 0 || p < minPrec) {
                    return left;
                }

                const lex::Token op = next();
                ast::Expr *right = binary(minPrec + 1);

                left = alloc_node<ast::Binary>(
                    ast::Span{left->span.start, right->span.end},
                    op.kind,
                    left,
                    right
                );
            }
        }

        ast::Expr *unary() {
            if (kind() == lex::TokenKind::Excl || kind() == lex::TokenKind::Tilde || kind() == lex::TokenKind::Minus) {
                const DepthGuard guard(*this);
                const lex::Token op = next();
                ast::Expr *operand = unary();

                return alloc_node<ast::Unary>(
                    ast::Span{op.start, operand->span.end},
                    op.kind,
                    operand
                );
            }

            return call();
        }

        ast::Expr *call() {
            ast::Expr *expr = primary();

            while (true) {
                if (kind() == lex::TokenKind::LParen) {
                    next();
                    const std::size_t mark = m_scratch.size();

                    if (kind() != lex::TokenKind::RParen) {
                        do {
                            m_scratch.push_back(expression());
                        } while (accept(lex::TokenKind::Comma));
                        expect(lex::TokenKind::RParen);
                        const std::span<ast::Expr *const> args = take<ast::Expr>(mark);

                        expr = alloc_node<ast::Call>(
                            ast::Span{expr->span.start, m_prevEnd},
                            expr,
                            args
                        );
                    }
                } else if (kind() == lex::TokenKind::Dot) {
                    next();
                    const lex::Token name = expect(lex::TokenKind::Identifier);
                    expr = alloc_node<ast::Index>(
                        ast::Span{expr->span.start, name.end},
                        expr,
                        ast::Span{name.start, name.end}
                    );
                } else if (kind() == lex::TokenKind::LBracket) {
                    next();
                    ast::Expr *idx = expression();
                    const lex::Token close = expect(lex::TokenKind::RBracket);
                    expr = alloc_node<ast::Index>(
                        ast::Span{expr->span.start, close.end},
                        expr,
                        idx
                    );
                } else {
                    return expr;
                }
            }
        }

        ast::Expr *primary() {
            const lex::Token token = peek();
            switch (token.kind) {
                case lex::TokenKind::KwTrue:
                case lex::TokenKind::KwFalse:
                    next();
                    return alloc_node<ast::BoolLit>(
                        ast::Span{token.start, token.end},
                        token.kind == lex::TokenKind::KwTrue
                    );
                case lex::TokenKind::KwNull:
                    next();
                    return alloc_node<ast::NullLit>(ast::Span{token.start, token.end});
                case lex::TokenKind::KwThis:
                    next();
                    return alloc_node<ast::This>(ast::Span{token.start, token.end});
                case lex::TokenKind::IntLiteral:
                    next();
                    return alloc_node<ast::IntLit>(ast::Span{token.start, token.end});
                case lex::TokenKind::FloatLiteral:
                    next();
                    return alloc_node<ast::FloatLit>(ast::Span{token.start, token.end});
                case lex::TokenKind::StringLiteral:
                    next();
                    return alloc_node<ast::StringLit>(ast::Span{token.start, token.end});
                case lex::TokenKind::Identifier: {
                    if (kind(1) == lex::TokenKind::LBrace) {
                        next();
                        const std::span<ast::FieldInit *const> inits = field_inits();
                        return alloc_node<ast::StructLit>(
                            ast::Span{token.start, m_prevEnd},
                            ast::Span{token.start, token.end},
                            inits
                        );
                    }
                    next();
                    return alloc_node<ast::Ident>(ast::Span{token.start, token.end});
                }
                case lex::TokenKind::LParen: {
                    next();
                    ast::Expr *expr = expression();
                    expect(lex::TokenKind::RParen);
                    return expr;
                }
                case lex::TokenKind::KwNew:
                    return new_expr();
                default:
                    // fail
            }
        }

        ast::Expr *new_expr() {
            const lex::Token kw = next();
            const lex::Token ty = peek();

            if (!is_prim(ty.kind) && ty.kind != lex::TokenKind::Identifier) {
                // fail
            }

            next();
            if (kind() == lex::TokenKind::LBracket) {
                ast::TypeRef *elem = alloc_node<ast::TypeRef>(
                    ast::Span{ty.start, ty.end},
                    ty.kind,
                    false,
                    ast::Span{ty.start, ty.end}
                );
                next();
                ast::Expr *size = expression();
                const lex::Token close = expect(lex::TokenKind::RBracket);

                return alloc_node<ast::NewArray>(
                    ast::Span{kw.start, close.end},
                    elem,
                    size
                );
            }
            if (ty.kind == lex::TokenKind::Identifier && kind() == lex::TokenKind::LBrace) {
                const std::span<ast::FieldInit *const> inits = field_inits();
                return alloc_node<ast::New>(
                    ast::Span{kw.start, m_prevEnd},
                    ast::Span{ty.start, ty.end},
                    inits
                );
            }

            // fail
        }

        std::span<ast::FieldInit *const> field_inits() {
            expect(lex::TokenKind::LBrace);
            const std::size_t mark = m_scratch.size();

            while (kind() != lex::TokenKind::RBrace && kind() != lex::TokenKind::Eof) {
                const lex::Token name = expect(lex::TokenKind::Identifier);
                expect(lex::TokenKind::Assign);
                ast::Expr *value = expression();

                m_scratch.push_back(alloc_node<ast::FieldInit>(
                    ast::Span{name.start, value->span.end},
                    ast::Span{name.start, name.end},
                    value
                ));

                if (!accept(lex::TokenKind::Comma)) {
                    break;
                }
            }

            expect(lex::TokenKind::RBrace);
            return take<ast::FieldInit>(mark);
        }

        std::span<const lex::Token> m_tokens;
        shared::ArenaAllocator &m_arena;

        std::vector<ast::Node *> m_scratch;
        lex::LexPos m_pos = 0;
        lex::LexPos m_prevEnd = 0;
        lex::LexPos m_nextId = 0;
        int m_depth = 0;
    };

    ParseResult parse(std::span<const lex::Token> tokens, shared::ArenaAllocator &arena) {
        return Parser(tokens, arena).run();
    }
} // namespace vela::parse