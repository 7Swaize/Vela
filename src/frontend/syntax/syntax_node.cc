#include "syntax_node.h"
#include "syntax_node_factory.h"

namespace vela::parse {
    const SyntaxElement *SyntaxNode::at(const int index) const {
        assert(index >= 0 && index < slotCount());

        if (const SyntaxElement *cached = m_slots[index]; cached != nullptr) {
            return cached;
        }

        const GreenNode *green = m_green->slot(index);
        if (green == nullptr) {
            return nullptr;
        }

        m_slots[index] = create_syntax_element(SyntaxElementAllocArgs {
            .green = green,
            .parent = this,
            .tree = m_syntaxTree,
            .position = m_position + static_cast<NodePos>(m_green->getSlotOffset(index)),
            .index = index,
            .slots = m_green->slotCount()
        });
        return m_slots[index];
    }

    const SyntaxToken *SyntaxNode::findTokenH(const SyntaxElement *curr, const NodePos start) {
        if (curr->isToken()) {
            return static_cast<const SyntaxToken *>(curr);
        }

        const SyntaxNode *node = static_cast<const SyntaxNode *>(curr);
        for (int i = 0; i < node->slotCount(); ++i) {
            if (const SyntaxElement *child = node->at(i); child != nullptr && start < child->end()) {
                return findTokenH(child, start);
            }
        }

        assert(false && "start outside node's span");
        return nullptr;
    }
} // namespace vela::parse