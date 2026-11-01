#ifndef VELA_AST_H
#define VELA_AST_H

#include <cassert>

#include "frontend/lexer/lex.h"

namespace vela::parse::ast {
    struct Span {
        lex::LexPos start = 0, end = 0;
    };

#define VELA_AST_NODE_KINDS(X) \
    X(Program)                 \
    X(Aggregate)               \
    X(FieldDeclaration)        \
    X(FunctionDeclaration)     \
    X(Parameter)               \
    X(VariableDeclaration)     \
    X(TypeReference)           \
    X(Block)                   \
    X(ExpressionStatement)     \
    X(If)                      \
    X(While)                   \
    X(For)                     \
    X(Return)                  \
    X(IntLiteral)              \
    X(FloatLiteral)            \
    X(StringLiteral)           \
    X(BoolLiteral)             \
    X(NullLiteral)             \
    X(This)                    \
    X(Identifier)              \
    X(Unary)                   \
    X(Binary)                  \
    X(Assign)                  \
    X(Call)                    \
    X(Member)                  \
    X(Index)                   \
    X(FieldInit)               \
    X(StructLiteral)           \
    X(New)                     \
    X(NewArray)                \

    enum class NodeKind : uint8_t {
#define X(Name) Name,
        VELA_AST_NODE_KINDS(X)
#undef X
    };

    constexpr std::string_view to_string(const NodeKind kind) {
        switch (kind) {
#define X(Name) \
        case NodeKind::Name: \
            return #Name;
                VELA_AST_NODE_KINDS(X)
#undef X
        }

        return "?";
    }

    struct Node {
        NodeKind kind;
        uint32_t id;
        Span span;
    };

    struct Expr : Node { };
    struct Stmt : Node { };
    struct Decl : Node { };

    struct TypeRef : Node {
        static constexpr NodeKind Kind = NodeKind::TypeReference;
        lex::TokenKind base;
        bool isArray;
        Span name;  // including trailing '[]'
    };

    struct Program : Node {
        static constexpr NodeKind Kind = NodeKind::Program;
        std::span<Decl *const> decls;
    };

    // Struct or class
    struct Aggregate : Decl {
        static constexpr NodeKind Kind = NodeKind::Aggregate;
        bool isClass;
        Span name;
        std::span<Decl *const> members;
    };

    struct FieldDecl : Decl {
        static constexpr NodeKind Kind = NodeKind::FieldDeclaration;
        TypeRef *type;
        Span name;
    };

    struct Parameter : Node {
        static constexpr NodeKind Kind = NodeKind::Parameter;
        TypeRef *type;
        Span name;
    };

    struct Block;
    struct FunctionDecl : Decl {
        static constexpr NodeKind Kind = NodeKind::FunctionDeclaration;
        TypeRef *ret;
        Span name;
        std::span<Parameter *const> params;
        Block *body;
    };

    struct VariableDecl : Decl {
        static constexpr NodeKind Kind = NodeKind::VariableDeclaration;
        TypeRef *type;
        Span name;
        Expr *init; // nullable
    };

    struct Block : Stmt {
        static constexpr NodeKind Kind = NodeKind::Block;
        std::span<Node *const> items;
    };

    struct ExprStmt : Stmt {
        static constexpr NodeKind Kind = NodeKind::ExpressionStatement;
        Expr *expr;
    };

    struct If : Stmt {
        static constexpr NodeKind Kind = NodeKind::If;
        Expr *condition;
        Stmt *thenBranch;
        Stmt *elseBranch;
    };

    struct While : Stmt {
        static constexpr NodeKind Kind = NodeKind::While;
        Expr *condition;
        Stmt *body;
    };

    struct For : Stmt {
        static constexpr NodeKind Kind = NodeKind::For;
        Node *init;
        Expr *condition;
        Expr *step;
        Stmt *body;
    };

    struct Return : Stmt {
        static constexpr NodeKind Kind = NodeKind::Return;
        Expr *value;
    };

    struct IntLit : Expr { static constexpr NodeKind Kind = NodeKind::IntLiteral; };
    struct FloatLit : Expr { static constexpr NodeKind Kind = NodeKind::FloatLiteral; };
    struct StringLit : Expr { static constexpr NodeKind Kind = NodeKind::StringLiteral; };
    struct NullLit : Expr { static constexpr NodeKind Kind = NodeKind::NullLiteral; };
    struct This : Expr { static constexpr NodeKind Kind = NodeKind::This; };
    struct Ident : Expr { static constexpr NodeKind Kind = NodeKind::Identifier; };

    struct BoolLit : Expr {
        static constexpr NodeKind Kind = NodeKind::BoolLiteral;
        bool value;
    };

    struct Unary : Expr {
        static constexpr NodeKind Kind = NodeKind::Unary;
        lex::TokenKind op;
        Expr *operand;
    };

    struct Binary : Expr {
        static constexpr NodeKind Kind = NodeKind::Binary;
        lex::TokenKind op;
        Expr *lhs, *rhs;
    };

    struct Assign : Expr {
        static constexpr NodeKind Kind = NodeKind::Assign;
        Expr *target;
        Expr *value;
    };

    struct Call : Expr {
        static constexpr NodeKind Kind = NodeKind::Call;
        Expr *callee;
        std::span<Expr *const> args;
    };

    struct Member : Expr {
        static constexpr NodeKind Kind = NodeKind::Member;
        Expr *object;
        Span name;
    };

    struct Index : Expr {
        static constexpr NodeKind Kind = NodeKind::Index;
        Expr *object;
        Expr *index;
    };

    struct FieldInit : Node {
        static constexpr NodeKind Kind = NodeKind::FieldInit;
        Span name;
        Expr *value;
    };

    // Point { x = 1, y = 2 }
    struct StructLit : Expr {
        static constexpr NodeKind Kind = NodeKind::StructLiteral;
        Span typeName;
        std::span<FieldInit *const> inits;
    };

    // new Point { x = 1 }
    struct New : Expr {
        static constexpr NodeKind Kind = NodeKind::New;
        Span typeName;
        std::span<FieldInit *const> inits;
    };

    struct NewArray : Expr {
        static constexpr NodeKind Kind = NodeKind::NewArray;
        TypeRef *elem;
        Expr *size;
    };

    template <typename T>
    concept HasNodeKind = requires {
        { T::Kind } -> std::convertible_to<NodeKind>;
        requires T::Kind == T::Kind;
    };

    template <HasNodeKind T>
    [[nodiscard]] bool is(const Node *n) {
        return n != nullptr && n->kind == T::Kind;
    }

    template <HasNodeKind T>
    [[nodiscard]] T *as(Node *n) {
        assert(is<T>(n));
        return static_cast<T *>(n);
    }

    template <HasNodeKind T>
    [[nodiscard]] T *dyn_cast(Node *n) {
        return is<T>(n) ? static_cast<T *>(n) : nullptr;
    }


} // namespace vela::parse:ast

#endif //VELA_AST_H
