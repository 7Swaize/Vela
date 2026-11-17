#ifndef VELA_SYNTAX_TREE_H
#define VELA_SYNTAX_TREE_H

#include "diagnostic.h"
#include "syntax_node_fwds.h"
#include "green/green_node_fwds.h"
#include "shared/allocators.h"

namespace vela::parse {
    class SyntaxTree {
    public:
        explicit SyntaxTree(const GreenCompilationUnitSyntax *greenRoot) : m_greenRoot(greenRoot) { }

        template <typename T>
        SyntaxNode *alloc_node(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *tree, const NodePos position) {
            SyntaxNode *node = m_arena.make<T>(green, parent, tree, position);
            return node;
        }

        const GreenCompilationUnitSyntax *greenRoot() const { return m_greenRoot; }
        const CompilationUnitSyntax *root() {
            if (m_root == nullptr) {
                m_root = alloc_node<CompilationUnitSyntax>(m_greenRoot, nullptr, this, 0);
            }

            return m_root;
        }

        std::span<Diagnostic *> diagnostics() const { return m_greenRoot->diagnostics(); }

    private:
        const CompilationUnitSyntax *m_root = nullptr;
        const GreenCompilationUnitSyntax *m_greenRoot;
        shared::ArenaAllocator m_arena;
    };
}

#endif //VELA_SYNTAX_TREE_H
