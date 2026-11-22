#include "syntax_node_concrete.h"
#include "syntax_tree.h"
#include "green/green_nonterminals.h"

namespace vela::parse {
    // ReSharper disable once CppPassValueParameterByConstReference
    SyntaxTree::SyntaxTree(GreenRef<GreenCompilationUnitSyntax> greenRoot)
        : m_greenRoot(std::move(greenRoot)) { }

    std::span<Diagnostic *> SyntaxTree::diagnostics() const { return m_greenRoot->diagnostics(); }

    const CompilationUnitSyntax *SyntaxTree::root() {
        if (m_root == nullptr) {
            m_root = alloc_node<CompilationUnitSyntax>(SyntaxElementAllocArgs {
                .green = m_greenRoot.get(),
                .parent = nullptr,
                .tree = this,
                .position = 0,
                .index = 0,
                .slots = m_greenRoot->slotCount()
            });
        }

        return m_root;
    }
} // namespace vela::parse