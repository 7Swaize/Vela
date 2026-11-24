#ifndef VELA_SYNTAX_TREE_H
#define VELA_SYNTAX_TREE_H

#include <span>

#include "diagnostic.h"
#include "syntax_node_fwds.h"
#include "green/green_node_fwds.h"
#include "shared/allocators.h"

namespace vela::parse {
    struct SyntaxElementAllocArgs {
        const GreenNode *green;
        const SyntaxNode *parent;
        const SyntaxTree *tree;
        NodePos position;
        int index;
        int slots;
    };

    class SyntaxTree {
    public:
        // Only the SyntaxTree needs to store a GreenRef<T> to a GreenNode. Since red nodes cannot outlive
        // the syntax tree (and thus cannot outlive the compilation), we are guaranteed that holding just
        // the root will keep everything we need alive. Avoiding a GreenRef<T> on every red node also allows
        // for trivial destruction within the memory arena (i.e. a GreenRef<T> member prevents trival destruction).
        explicit SyntaxTree(GreenRef<GreenCompilationUnitSyntax> greenRoot);

        SyntaxTree(const SyntaxTree &) = delete;
        SyntaxTree &operator=(const SyntaxTree &) = delete;
        SyntaxTree(SyntaxTree &&) = delete;
        SyntaxTree &operator=(SyntaxTree &&) = delete;

        template <RequiresSyntaxNode T>
        const T *alloc_node(const SyntaxElementAllocArgs &args) {
            // This won't have any effect on locality, since in most cases these will be allocated side-by-side in memory.
            // Worst case they are in different internal chunks in the alloctor. But that's still fine.
            const std::span<const SyntaxElement *> slots =
                m_arena.make_array_fill<const SyntaxElement *>(static_cast<std::size_t>(args.green->slotCount()), nullptr);

            return m_arena.make<T>(args.green, args.parent, args.tree, args.position, args.index, slots.data());
        }

        const SyntaxToken *alloc_token(const SyntaxElementAllocArgs &args) {
            return m_arena.make<SyntaxToken>(args.green, args.parent, args.position, args.index);
        }

        const GreenCompilationUnitSyntax *greenRoot() const { return m_greenRoot.get(); }
        const CompilationUnitSyntax *root();

        std::span<Diagnostic *> diagnostics() const;

    private:
        GreenRef<GreenCompilationUnitSyntax> m_greenRoot;
        const CompilationUnitSyntax *m_root = nullptr;
        shared::ArenaAllocator m_arena;
    };
}

#endif //VELA_SYNTAX_TREE_H
