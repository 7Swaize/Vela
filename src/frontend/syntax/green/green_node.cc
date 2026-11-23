#include "green_node.h"

namespace vela::parse {
    // We have to use an iterative approach here because a recursive chain could possibly overflow.
    // (Though its not very likely).
    void GreenNode::destroy(const GreenNode *root) {
        std::vector worklist{root};

        auto drop = [&](const GreenNode *child) {
            if (child != nullptr && --child->m_refCount == 0) {
                worklist.push_back(child);
            }
        };

        while (!worklist.empty()) {
            const GreenNode *node = worklist.back();
            worklist.pop_back();

            if (node->isToken()) {
                const GreenToken *token = static_cast<const GreenToken *>(node);
                drop(token->leading());
                drop(token->trailing());
            } else {
                for (int i = 0; i < node->slotCount(); ++i) {
                    drop(node->slot(i));
                }
            }

            if (node->hasFlag(NodeFlags::HasDiagnosticsDirectly)) {
                m_diagnosticsTable.erase(node);
            }

            delete node;
        }
    }
}