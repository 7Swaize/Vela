#include "syntax_node_concrete.h"
#include "syntax_tree.h"
#include "green/green_nonterminals.h"

namespace vela::parse {
    const CompilationUnitSyntax *SyntaxTree::root() {
        if (m_root == nullptr) {
            m_root = alloc_node<CompilationUnitSyntax>(m_greenRoot, nullptr, this, 0);
        }

        return m_root;
    }

    std::span <Diagnostic *> SyntaxTree::diagnostics() const { return m_greenRoot->diagnostics(); }
} // namespace vela::parse