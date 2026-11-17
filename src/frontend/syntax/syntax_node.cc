#include "syntax_node.h"
#include "syntax_node_factory.h"

namespace vela::parse {
    void SyntaxNode::createChildren() {
        const uint16_t count = m_green->slotCount();
        if (count == 0) {
            m_children.emplace();
            return;
        }

        m_children = std::vector<const SyntaxNode*>{};
        m_children->reserve(count);

        NodePos position = fullSpan().start;
        for (int i = 0; i < count; ++i) {
            const GreenNode *greenChild = m_green->getSlot(i);
            if (greenChild == nullptr) {
                m_children->push_back(nullptr);
                continue;
            }

            const SyntaxNode *child = create_syntax_node(greenChild, this, m_syntaxTree, position);
            m_children->push_back(child);
            position += greenChild->fullWidth();
        }
    }
} // namespace vela::parse