#ifndef VELA_SYNTAX_NODE_H
#define VELA_SYNTAX_NODE_H

#include <concepts>
#include <optional>
#include <vector>

#include "syntax_node_factory.h"
#include "syntax_tree.h"
#include "frontend/diagnostic.h"
#include "green/green_node.h"

namespace vela::parse {
    class SyntaxNode;
    class SyntaxToken;

    template <typename T>
    concept RequiresSyntaxNode = std::derived_from<T, SyntaxNode>;

    class SyntaxNode {
    public:
        SyntaxNode(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : m_green(green), m_syntaxTree(syntaxTree), m_position(position), m_parent(parent) { }

        virtual ~SyntaxNode() { }

        [[nodiscard]] virtual NodeKind kind() const = 0;
        [[nodiscard]] virtual NodeKind underlyingKind() const { return NodeKind::UndefinedSyntax; }

        [[nodiscard]] shared::Range<NodePos> fullSpan() const { return {.start = m_position, .end = m_position + m_green->fullWidth()}; }
        [[nodiscard]] std::span<Diagnostic *> diagnostics() const { return m_green->diagnostics(); }
        [[nodiscard]] NodePos position() const { return m_position; }

        template <RequiresSyntaxNode T>
        const T *validateRequiredNode(const T *target, int index) {
            if (target == nullptr) {
                target = static_cast<const T *>(getOrCreateChildren()[index]);
            }

            return target;
        }

    protected:
        std::span<const SyntaxNode *> getOrCreateChildren() {
            if (m_children.has_value()) {
                return m_children.value();
            }

            createChildren();
            return m_children.value();
        }

        const GreenNode *m_green;
        const SyntaxTree *m_syntaxTree;
        NodePos m_position;

    private:
        void createChildren() {
            const uint16_t count = m_green->slotCount();
            if (count == 0) {
                m_children = {};
                return;
            }

            m_children = std::vector<const SyntaxNode*>{};
            m_children->reserve(count);

            NodePos position = fullSpan().start;
            for (int i = 0; i < count; ++i) {
                const GreenNode *greenChild = m_green->getSlot(i);
                if (greenChild == nullptr) {
                    continue;
                }

                auto child = create_syntax_node(m_syntaxTree, this, greenChild, position);
                m_children->push_back(child);
                position += greenChild->fullWidth();
            }
        }

        const SyntaxNode *m_parent;
        std::optional<std::vector<const SyntaxNode*>> m_children = std::nullopt;
    };

    class SyntaxToken final : public SyntaxNode {
    public:
        SyntaxToken(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : SyntaxNode(green, parent, syntaxTree, position) { }

        [[nodiscard]] NodeKind kind() const override { return m_green->kind(); }
        [[nodiscard]] const GreenToken *token() const { return static_cast<const GreenToken *>(m_green); }
    };

    class SyntaxNodeList final : public SyntaxNode {
    public:

    }
}

#endif //VELA_SYNTAX_NODE_H
