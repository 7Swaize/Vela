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
            return m_newLength - m_range.length();
        }

        constexpr bool touches(const TextChangeRange& other) const noexcept {
            return m_range.start <= other.m_range.start && other.m_range.end <= m_range.end;
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
        NormalizedChangeCollection lexAot(lex::Lexer lexer) const {
            std::vector<std::vector<GreenToken>> aotLex;
            aotLex.reserve(m_changes.size());

            for (const auto& change : m_changes) {
                m_changes.push_back(lexer.lex_subset(/* TODO */));
            }

            return NormalizedChangeCollection(std::move(m_changes), std::move(aotLex));
        }

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

            Cursor(const SyntaxElement *nodeOrToken, int indexInParent) noexcept
                : m_nodeOrToken(nodeOrToken), m_indexInParent(indexInParent) { }

            static Cursor from_root(const SyntaxNode *node) {
                return Cursor(node, 0);
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
                const SyntaxNode *parent = m_nodeOrToken->parent();
                return Cursor(parent, m_nodeOrToken->indexInParent());
            }

            static bool isNonZeroWidthOrIsEndOfFile(const SyntaxElement *element) {
                return element->kind() == NodeKind::EofToken || element->fullWidth() != 0;
            }

            Cursor tryFindNextNonZeroWidthOrIsEndOfFileSibling() const;

            const SyntaxElement *m_nodeOrToken;
            int m_indexInParent = 0;
        };

        class Reader {
        public:
            Reader(lex::Lexer lexer,
                    Cursor cursor,
                    const NormalizedChangeCollection* changes,
                    NodePos changeDelta,
                    NodePos newPosition)
                : m_lexer(lexer), m_oldTreeCursor(cursor),
                  m_changes(changes), m_changeDelta(changeDelta), m_newPosition(newPosition) { }

            BlendedNode readNodeOrToken();

        private:
            void skipOldElement();
            void skipCurrent(const SyntaxElement *nodeOrToken);
            void skipPastChanges();

            BlendedNode readNewToken();
            std::optional<BlendedNode> tryTokenReuse();

            bool intersectsCurrentChange(const SyntaxElement *element) const;
            bool canReuse(const SyntaxElement *element) const;

            lex::Lexer m_lexer;
            Cursor m_oldTreeCursor;
            const NormalizedChangeCollection *m_changes;
            std::size_t m_currChange = 0;

            NodePos m_changeDelta;
            NodePos m_newPosition;
        };

    public:
        Blender(lex::Lexer lexer, const SyntaxNode* oldTree, std::span<const TextChangeRange> changes);

    private:
        static TextChangeRange extendToAffectedRange(const SyntaxNode& oldTree, const TextChangeRange &change);

        lex::Lexer m_lexer;
        Cursor m_oldTreeCursor;

        std::shared_ptr<const NormalizedChangeCollection> m_changes = std::make_shared<const NormalizedChangeCollection>();
        std::size_t m_currChange = 0;

        NodePos m_changeDelta; // old position - new position
        NodePos m_newPosition;

    };
} // namespace vela::parse

#endif //VELA_BLENDER_H
