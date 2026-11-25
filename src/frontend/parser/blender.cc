#include <cassert>

#include "blender.h"

namespace vela::parse {
    struct BlendedNode {
        const GreenNode *green;
        Blender blender;
    };

    void NormalizedChangeCollection::normalize() {
        if (m_changes.size() < 2) {
            return;
        }

        std::ranges::sort(m_changes, [](const TextChangeRange& a, const TextChangeRange& b) {
            return a.range().start < b.range().start;
        });

        auto out = m_changes.begin();
        for (auto it = std::next(m_changes.begin()); it != m_changes.end(); ++it) {
            if (out->touches(*it)) {
                *out = TextChangeRange::merge(*it, *out);
            } else {
                *++out = *it;
            }
        }
        m_changes.erase(std::next(out), m_changes.end());
    }

    NormalizedChangeCollection NormalizedChangeCollection::lexAot(lex::Lexer lexer) const {
        std::int64_t srcSize = static_cast<std::int64_t>(lexer.src().rawSize());
        std::int64_t cumulativeShift = 0;

        for (const TextChangeRange &change : m_changes) {
            std::int64_t newStart = static_cast<std::int64_t>(change.range().start) + cumulativeShift;
            std::int64_t newEnd = newStart + static_cast<std::int64_t>(change.newLength());
            cumulativeShift += change.delta();

            assert(newStart > 0 && newEnd <= srcSize);
        }
    }

    // Addresses normalization mentioned here in ctor: https://github.com/dotnet/roslyn/blob/main/src/Compilers/CSharp/Portable/Parser/Blender.cs
    Blender::Blender(lex::Lexer lexer, shared::ArenaAllocator &arena, const SyntaxNode* oldTree, std::span<const TextChangeRange> changes)
        : m_lexer(lexer), m_arena(arena), m_changeDelta(0)
    {
        if (oldTree != nullptr && !changes.empty()) {
            m_changes = std::make_shared<const NormalizedChangeCollection>(
                NormalizedChangeCollection(changes).transform([&](const TextChangeRange &range) {
                    return extendToAffectedRange(*oldTree, range);
                })
                .lexAot(lexer)
            );
        }

        if (oldTree == nullptr) {
            m_oldTreeCursor = Cursor();
        } else {
            m_oldTreeCursor = Cursor::from_root(oldTree);
            m_newPosition = 0;
        }
    }

    TextChangeRange Blender::extendToAffectedRange(const SyntaxNode& oldTree, const TextChangeRange &change) {
        // It's very unfortunate that we have to lookahead by 2 tokens to accommodate for packs: '..."
        constexpr int tokenLookahead = 2;

        NodePos last = oldTree.fullWidth();

        // If end of tree, clamp back
        NodePos start = std::max(std::min(change.range().start, last), static_cast<NodePos>(0));

        for (NodePos i = 0; start > 0 && i <= tokenLookahead;) {
            const SyntaxToken *token = oldTree.findToken(start);
            assert(token != nullptr && "this shouldn't happen");

            start = static_cast<NodePos>(
                std::max(static_cast<std::int64_t>(0), static_cast<std::int64_t>(token->position()) - 1)
            );

            if (token->fullWidth() > 0) {
                ++i;
            }
        }

        shared::Range updatedRange = shared::Range{ .start = start, .end = change.range().end };
        NodePos updatedLength = change.newLength() + (change.range().start - start);
        return TextChangeRange(updatedRange, updatedLength);
    }

    Blender::Cursor Blender::Cursor::tryFindNextNonZeroWidthOrIsEndOfFileSibling() const {
        const SyntaxNode *parent = m_nodeOrToken->parent();

        if (parent != nullptr) {
            for (int i = m_nodeOrToken->indexInParent() + 1; i < parent->slotCount(); ++i) {
                const SyntaxElement *sibling = parent->child(i);

                if (sibling != nullptr && isNonZeroWidthOrIsEndOfFile(sibling)) {
                    return Cursor(sibling);
                }
            }
        }

        return Cursor();
    }

    Blender::Cursor Blender::Cursor::moveToNextSibling(Cursor cursor) const {
        while (cursor.m_nodeOrToken->green() != nullptr) {
            Cursor nextSibling = tryFindNextNonZeroWidthOrIsEndOfFileSibling();

            // We found a valid parent, return it.
            if (cursor.m_nodeOrToken->green() != nullptr) {
                return nextSibling;
            }

            // We reached the end of the sibling chain and need to walk the parents sibling chain.
            cursor = cursor.moveToParent();
        }

        return Cursor();
    }

    Blender::Cursor Blender::Cursor::moveToFirstChild() const {
        assert(!m_nodeOrToken->isToken() && "token cannot have children");
        const SyntaxNode *asnode = static_cast<const SyntaxNode *>(m_nodeOrToken);

        for (int i = 0; i < asnode->slotCount(); ++i) {
            const SyntaxElement *child = asnode->child(i);
            if (child != nullptr && isNonZeroWidthOrIsEndOfFile(child)) {
                return Cursor(child);
            }
        }

        return Cursor();
    }

    Blender::Cursor Blender::Cursor::moveToFirstToken() const {
        Cursor curr = *this;
        if (!curr.isFinished()) {
            for (const SyntaxElement *node = curr.m_nodeOrToken; node->kind() != NodeKind::UndefinedSyntax && !node->isToken(); node = curr.m_nodeOrToken) {
                curr = curr.moveToFirstChild();
            }
        }

        return curr;
    }

    BlendedNode Blender::Reader::readNodeOrToken() {
        while (true) {
            if (m_oldTreeCursor.isFinished()) {
                return readNewToken();
            }

            // If we are further ahead in the new text (change delta is negative), we keep
            // skipping tokens until we catch up. Our approach also skips whole nodes if they are
            // shorter than 'm_changeDelta'. This is an optimization mentioned here: https://github.com/dotnet/roslyn/blob/main/src/Compilers/CSharp/Portable/Parser/Blender.Reader.cs
            //
            // If we are further ahead in the old text (change delta is positive), we read tokens
            // to sync up with the old text. This can happen when skipping over poritions of the old tree
            // that overlapped with changes.
            if (m_changeDelta < 0) {
                skipOldElement();
            } else if (m_changeDelta > 0) {
                return readNewToken();
            } else {
                if (std::optional<BlendedNode> blended = tryReuse(); blended.has_value()) {
                    return *std::move(blended);
                }

                if (m_oldTreeCursor.nodeOrToken()->isNode()) {
                    m_oldTreeCursor = m_oldTreeCursor.moveToFirstChild();
                } else {
                    skipOldToken();
                }
            }
        }
    }

    void Blender::Reader::skipOldElement() {
        assert(!m_oldTreeCursor.isFinished());

        // Greedily take the largest element (preferably a node) that is smaller than the change delta.
        while (m_oldTreeCursor.nodeOrToken()->isNode()) {
            const SyntaxNode *node = static_cast<const SyntaxNode *>(m_oldTreeCursor.nodeOrToken());
            if (node->fullWidth() <= m_changeDelta && !intersectsCurrentChange(node)) {
                skipCurrent(node);
                return;
            }

            m_oldTreeCursor = m_oldTreeCursor.moveToFirstChild();
        }

        skipCurrent(m_oldTreeCursor.nodeOrToken());
    }

    void Blender::Reader::skipOldToken() {
        assert(!m_oldTreeCursor.isFinished());

        m_oldTreeCursor = m_oldTreeCursor.moveToFirstToken();
        if (m_oldTreeCursor.isFinished()) {
            return;
        }

        skipCurrent(m_oldTreeCursor.nodeOrToken());
    }

    void Blender::Reader::skipCurrent(const SyntaxElement *nodeOrToken) {
        m_changeDelta += nodeOrToken->fullWidth();
        m_oldTreeCursor = m_oldTreeCursor.moveToNextSibling(m_oldTreeCursor);
        skipPastChanges();
    }

    void Blender::Reader::skipPastChanges() {
        NodePos oldPos = m_oldTreeCursor.nodeOrToken()->position();
        while (m_currChange < m_changes->size() && oldPos >= m_changes->operator[](m_currChange).range().end) {
            m_changeDelta += m_changes->operator[](m_currChange).delta();
            ++m_currChange;
        }
    }

    bool Blender::Reader::intersectsCurrentChange(const SyntaxElement *element) const {
        if (m_currChange >= m_changes->size()) {
            return false;
        }

        shared::Range oldRange = element->fullSpan();
        shared::Range changeRange = m_changes->operator[](m_currChange).range();

        return oldRange.intersects(changeRange);
    }

    BlendedNode Blender::Reader::readNewToken() {
        assert(m_changeDelta > 0 || m_oldTreeCursor.isFinished());

        // Pull a token corresponding to the current change that has already been lexed.

    }

    std::optional<BlendedNode> Blender::Reader::tryReuse() {
        const SyntaxElement *current = m_oldTreeCursor.nodeOrToken();
        if (!canReuse(current)) {
            return std::nullopt;
        }

        m_newPosition += current->fullWidth();
        m_oldTreeCursor = m_oldTreeCursor.moveToNextSibling(m_oldTreeCursor);

        return createBlendedNode(current->green());
    }

    bool Blender::Reader::canReuse(const SyntaxElement *element) const {
        const GreenNode *green = element->green();

        // The only non-error or valid-for-reuse 0-width token that exists is the EofToken.
        if (element->fullWidth() == 0) {
            return false;
        }

        // Obviously, if it intersects a change we cannot use it.
        if (intersectsCurrentChange(element)) {
            return false;
        }

        // Nodes with diagnostics can't be reused
        if (green->containsDiagnostics() ||
            (green->isToken() && green->containsSkippedText()
                && element->parent() != nullptr && element->parent()->green()->containsDiagnostics()))
        {
            return false;
        }

        return true;
    }

    BlendedNode Blender::Reader::createBlendedNode(const GreenNode *green) const {
        return BlendedNode{
            .green = green,
            .blender = Blender(m_lexer, m_arena, m_oldTreeCursor, m_changes, m_currChange, m_changeDelta, m_newPosition)
        };
    }
} // namespace vela::parse