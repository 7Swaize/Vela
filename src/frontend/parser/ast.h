// ReSharper disable CppMemberFunctionMayBeStatic
#ifndef VELA_AST_H
#define VELA_AST_H

#include <cassert>
#include <concepts>
#include <cstdint>
#include <span>
#include <string_view>
#include <type_traits>
#include <utility>

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

    using NodeId = lex::LexPos;

    struct Node {
        NodeKind kind;
        NodeId id;
        Span span;
    };

    struct Expr : Node { };
    struct Stmt : Node { };
    struct Decl : Node { };

    struct TypeReference : Node {
        static constexpr NodeKind Kind = NodeKind::TypeReference;
        lex::TokenKind base;
        bool isArray;
        Span name;  // excluding trailing '[]'
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

    struct FieldDeclaration : Decl {
        static constexpr NodeKind Kind = NodeKind::FieldDeclaration;
        TypeReference *type;
        Span name;
    };

    struct Parameter : Node {
        static constexpr NodeKind Kind = NodeKind::Parameter;
        TypeReference *type;
        Span name;
    };

    struct Block;
    struct FunctionDeclaration : Decl {
        static constexpr NodeKind Kind = NodeKind::FunctionDeclaration;
        TypeReference *ret;
        Span name;
        std::span<Parameter *const> params;
        Block *body;
    };

    struct VariableDeclaration : Decl {
        static constexpr NodeKind Kind = NodeKind::VariableDeclaration;
        TypeReference *type;
        Span name;
        Expr *init; // nullable
    };

    struct Block : Stmt {
        static constexpr NodeKind Kind = NodeKind::Block;
        std::span<Node *const> items;
    };

    struct ExpressionStatement : Stmt {
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

    struct IntLiteral : Expr { static constexpr NodeKind Kind = NodeKind::IntLiteral; };
    struct FloatLiteral : Expr { static constexpr NodeKind Kind = NodeKind::FloatLiteral; };
    struct StringLiteral : Expr { static constexpr NodeKind Kind = NodeKind::StringLiteral; };
    struct NullLiteral : Expr { static constexpr NodeKind Kind = NodeKind::NullLiteral; };
    struct This : Expr { static constexpr NodeKind Kind = NodeKind::This; };
    struct Identifier : Expr { static constexpr NodeKind Kind = NodeKind::Identifier; };

    struct BoolLiteral : Expr {
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
    struct StructLiteral : Expr {
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
        TypeReference *elem;
        Expr *size;
    };

    template <typename T>
    concept HasNodeKind = requires {
        requires std::same_as<std::remove_cvref_t<decltype(T::Kind)>, NodeKind>;
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

    template <typename V>
    concept AstVisitor = true
#define X(Name) && std::invocable<V, Name *>
        VELA_AST_NODE_KINDS(X);
#undef X

    template <AstVisitor V>
    decltype(auto) visit(Node *n, V &&v) {
        switch (n->kind) {
#define X(Name) case NodeKind::Name: return std::forward<V>(v)(static_cast<Name *>(n));
            VELA_AST_NODE_KINDS(X)
#undef X
        }
        std::unreachable();
    }

    template <typename Derived>
    struct AstWalker {
        void walk(Node *n) {
            visit(n, [this](auto *node) {
                if (node == nullptr) {
                    return;
                }

                if constexpr (requires { static_cast<Derived *>(this)->visit(node); }) {
                    static_cast<Derived *>(this)->visit(node);
                } else {
                    walkChildren(node);
                }
            });
        }

        void walkChildren(const Program *node) {
            for (const auto &n : node->decls) {
                walk(n);
            }
        }

        void walkChildren(const Aggregate *node) {
            for (const auto &n : node->members) {
                walk(n);
            }
        }

        void walkChildren(const FieldDeclaration *node) {
            walk(node->type);
        }

        void walkChildren(const Parameter *node) {
            walk(node->type);
        }

        void walkChildren(const FunctionDeclaration *node) {
            walk(node->ret);

            for (const auto &n : node->params) {
                walk(n);
            }

            walk(node->body);
        }

        void walkChildren(const VariableDeclaration *node) {
            walk(node->type);
            walk(node->init);
        }

        void walkChildren(const Block *node) {
            for (const auto &n : node->items) {
                walk(n);
            }
        }

        void walkChildren(const ExpressionStatement *node) {
            walk(node->expr);
        }

        void walkChildren(const If *node) {
            walk(node->condition);
            walk(node->thenBranch);
            walk(node->elseBranch);
        }

        void walkChildren(const While *node) {
            walk(node->condition);
            walk(node->body);
        }

        void walkChildren(const For *node) {
            walk(node->init);
            walk(node->condition);
            walk(node->step);
            walk(node->body);
        }

        void walkChildren(const Return *node) {
            walk(node->value);
        }

        void walkChildren(const Unary *node) {
            walk(node->operand);
        }

        void walkChildren(const Binary *node) {
            walk(node->lhs);
            walk(node->rhs);
        }

        void walkChildren(const Assign *node) {
            walk(node->target);
            walk(node->value);
        }

        void walkChildren(const Call *node) {
            walk(node->callee);

            for (const auto &n : node->args) {
                walk(n);
            }
        }

        void walkChildren(const Member *node) {
            walk(node->object);
        }

        void walkChildren(const Index *node) {
            walk(node->object);
            walk(node->index);
        }

        void walkChildren(const FieldInit *node) {
            walk(node->value);
        }

        void walkChildren(const StructLiteral *node) {
            for (const auto &n : node->inits) {
                walk(n);
            }
        }

        void walkChildren(const New *node) {
            for (const auto &n : node->inits) {
                walk(n);
            }
        }

        void walkChildren(const NewArray *node) {
            walk(node->elem);
            walk(node->size);
        }

        void walkChildren(const TypeReference *) {}
        void walkChildren(const IntLiteral *) {}
        void walkChildren(const FloatLiteral *) {}
        void walkChildren(const StringLiteral *) {}
        void walkChildren(const BoolLiteral *) {}
        void walkChildren(const NullLiteral *) {}
        void walkChildren(const This *) {}
        void walkChildren(const Identifier *) {}
    };
} // namespace vela::parse::ast

#endif //VELA_AST_H
