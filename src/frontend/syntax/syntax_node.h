#ifndef VELA_SYNTAX_NODE_H
#define VELA_SYNTAX_NODE_H

#include <vector>

#include "syntax_tree.h"
#include "frontend/diagnostic.h"
#include "green/green_node.h"

namespace vela::parse {
    class SyntaxNode {
    public:
        SyntaxNode(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : m_green(green), m_syntaxTree(syntaxTree), m_position(position), m_parent(parent) { }

        virtual ~SyntaxNode() { }

        [[nodiscard]] virtual NodeKind kind() const = 0;

        [[nodiscard]] const std::vector<Diagnostic *> &diagnostics() const {
            auto diagnostics = m_green->diagnostics();
            return diagnostics.has_value() ? diagnostics.value() : std::vector<Diagnostic *>();
        }

    protected:
        const GreenNode *m_green;
        const SyntaxTree *m_syntaxTree;
        NodePos m_position;

    private:
         const SyntaxNode *m_parent;
    };
}

#endif //VELA_SYNTAX_NODE_H
