#ifndef VELA_SYNTAX_TREE_H
#define VELA_SYNTAX_TREE_H

#include <span>

#include "diagnostic.h"
#include "syntax_node_fwds.h"
#include "green/green_node_fwds.h"
#include "shared/allocators.h"

namespace vela::parse {
    class SyntaxTree {
    public:
        explicit SyntaxTree(const GreenCompilationUnitSyntax *greenRoot) : m_greenRoot(greenRoot) { }

        template <typename T>
        T *alloc_node(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *tree, const NodePos position) {
            T *node = m_arena.make<T>(green, parent, tree, position);
            return node;
        }

        const GreenCompilationUnitSyntax *greenRoot() const { return m_greenRoot; }
        const CompilationUnitSyntax *root();

        std::span<Diagnostic *> diagnostics() const;

    private:
        const CompilationUnitSyntax *m_root = nullptr;
        const GreenCompilationUnitSyntax *m_greenRoot;
        shared::ArenaAllocator m_arena;
    };
}

#endif //VELA_SYNTAX_TREE_H
