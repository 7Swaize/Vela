#ifndef VELA_BLENDER_H
#define VELA_BLENDER_H

#include <algorithm>
#include <cstddef>
#include <memory>
#include <optional>
#include <stack>
#include <vector>

#include "frontend/syntax/green/green_node_fwds.h"
#include "lexer/lex.h"
#include "shared/allocators.h"
#include "shared/helpers.h"
#include "syntax/syntax_node.h"

namespace vela::parse {
    struct BlendedNode;

    class TextChangeRange {
    public:
        using Range = shared::Range<NodePos>;

        constexpr TextChangeRange() noexcept = default;
        constexpr TextChangeRange(Range range, NodePos newLength) noexcept
            : m_range(range), m_newLength(newLength) { }

        constexpr const Range& range() const noexcept { return m_range; }
        constexpr NodePos newLength() const noexcept { return m_newLength; }

        constexpr std::ptrdiff_t delta() const noexcept {
            return static_cast<std::ptrdiff_t>(m_newLength) - static_cast<std::ptrdiff_t>(m_range.length());
        }

        constexpr bool touches(const TextChangeRange& other) const noexcept {
            return m_range.start <= other.m_range.end && other.m_range.start <= m_range.end;
        }

        static constexpr TextChangeRange merge(const TextChangeRange& a, const TextChangeRange& b) {
            const Range merged {
                .start = std::min(a.m_range.start, b.m_range.start),
                .end = std::max(a.m_range.end, b.m_range.end)
            };

            // Merge changed length cross changes.
            // For example, [10, 12)->3 and [12, 15)->0 touch, so so they merge to [10->15)->3.
            // The deltars are 1 and -3 respectively, and 5 + (1 - 3) = 3.
            const auto len = static_cast<std::ptrdiff_t>(merged.length()) + a.delta() + b.delta();
            return TextChangeRange(merged, static_cast<NodePos>(len));
        }

    private:
        shared::Range<NodePos> m_range;
        NodePos m_newLength;
    };

    class NormalizedChangeCollection {
    public:
        NormalizedChangeCollection() = default;

        explicit NormalizedChangeCollection(std::span<const TextChangeRange> changes)
            : m_changes(changes.begin(), changes.end())
        {
            normalize();
        }

        explicit NormalizedChangeCollection(std::vector<TextChangeRange> changes)
            : m_changes(std::move(changes))
        {
            normalize();
        }

        template <std::invocable<const TextChangeRange&> F>
        NormalizedChangeCollection transform(F &&func) const {
            std::vector<TextChangeRange> out;
            out.reserve(m_changes.size());

            for (const auto& change : m_changes) {
                out.push_back(std::forward<F>(func)(change));
            }

            return NormalizedChangeCollection(std::move(out));
        }

        // Since our lexer operates in 16 (NEON) or 32 (AVX2) byte chunks, its not practical to lex
        // token by token as we would in a sequential, scalar lexer. Therefore, we just lex the whole
        // changed span AOT. Since, our lexer is so fast, it doesn't matter if some of these tokens could
        // have been reused.
        NormalizedChangeCollection lexAot(lex::Lexer lexer) const;

        std::size_t size() const noexcept { return m_changes.size(); }
        bool empty() const noexcept { return m_changes.empty(); }

        const TextChangeRange &operator[](std::size_t i) const noexcept { return m_changes[i]; }
        auto begin() const noexcept { return m_changes.begin(); }
        auto end() const noexcept { return m_changes.end(); }

    private:
        NormalizedChangeCollection(std::vector<TextChangeRange> changes, std::vector<std::vector<GreenToken>> aotLex)
            : m_changes(std::move(changes)), m_aotLex(std::move(aotLex)) { }

        void normalize();

        std::vector<TextChangeRange> m_changes;
        std::vector<std::vector<GreenToken>> m_aotLex;
    };

    class Blender {
    private:
        class Cursor {
        public:
            Cursor() noexcept = default;

            explicit Cursor(const SyntaxElement *nodeOrToken) noexcept
                : m_nodeOrToken(nodeOrToken) { }

            static Cursor from_root(const SyntaxNode *node) {
                return Cursor(node);
            }

            const SyntaxElement *nodeOrToken() const noexcept { return m_nodeOrToken; }

            bool isFinished() const noexcept {
                return m_nodeOrToken->kind() == NodeKind::EofToken || m_nodeOrToken->kind() == NodeKind::UndefinedSyntax;
            }

            Cursor moveToNextSibling(Cursor cursor) const;
            Cursor moveToFirstChild() const;
            Cursor moveToFirstToken() const;

        private:
            Cursor moveToParent() const {
                return Cursor(m_nodeOrToken->parent());
            }

            static bool isNonZeroWidthOrIsEndOfFile(const SyntaxElement *element) {
                return element->kind() == NodeKind::EofToken || element->fullWidth() != 0;
            }

            Cursor tryFindNextNonZeroWidthOrIsEndOfFileSibling() const;

            const SyntaxElement *m_nodeOrToken;
        };

        class Reader {
        public:
            Reader(lex::Lexer lexer,
                   shared::ArenaAllocator *arena,
                   Cursor cursor,
                   std::shared_ptr<const NormalizedChangeCollection> changes,
                   std::size_t currChange,
                   std::int64_t changeDelta,
                   NodePos newPosition)
                : m_lexer(lexer), m_arena(arena), m_oldTreeCursor(cursor), m_changes(std::move(changes)),
                  m_currChange(currChange), m_changeDelta(changeDelta), m_newPosition(newPosition) { }

            BlendedNode readNodeOrToken();

        private:
            void skipOldElement();
            void skipOldToken();
            void skipCurrent(const SyntaxElement *nodeOrToken);
            void skipPastChanges();

            BlendedNode readNewToken();
            std::optional<BlendedNode> tryReuse();

            bool intersectsCurrentChange(const SyntaxElement *element) const;
            bool canReuse(const SyntaxElement *element) const;

            BlendedNode createBlendedNode(const GreenNode *green) const;

            lex::Lexer m_lexer;
            shared::ArenaAllocator *m_arena;
            Cursor m_oldTreeCursor;
            std::shared_ptr<const NormalizedChangeCollection> m_changes;
            std::size_t m_currChange = 0;

            std::int64_t m_changeDelta;
            NodePos m_newPosition;
        };

    public:
        Blender(lex::Lexer lexer, const SyntaxNode* oldTree, std::span<const TextChangeRange> changes);

    private:
        Blender(lex::Lexer lexer,
                shared::ArenaAllocator *arena,
                Cursor oldTreeCursor,
                std::shared_ptr<const NormalizedChangeCollection> changes,
                std::size_t currChange,
                std::int64_t changeDelta,
                NodePos newPosition)
            : m_lexer(lexer), m_arena(arena), m_oldTreeCursor(oldTreeCursor), m_changes(std::move(changes)),
              m_currChange(currChange), m_changeDelta(changeDelta), m_newPosition(newPosition) { }

        static TextChangeRange extendToAffectedRange(const SyntaxNode& oldTree, const TextChangeRange &change);

        lex::Lexer m_lexer;
        shared::ArenaAllocator *m_arena;
        Cursor m_oldTreeCursor;

        std::shared_ptr<const NormalizedChangeCollection> m_changes = std::make_shared<const NormalizedChangeCollection>();
        std::size_t m_currChange = 0;

        // Given 'NodePos' is a uint32_t, this is a 'std::int64_t' to accommodate for such a change difference.
        // However, it is impractical in most cases. The only one of which really exists when we
        // are lexing a completely new file, where we continuously increment the delta.
        std::int64_t m_changeDelta; // old position - new position
        NodePos m_newPosition;

    };
} // namespace vela::parse

#endif //VELA_BLENDER_H
