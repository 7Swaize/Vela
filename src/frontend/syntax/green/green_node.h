#ifndef VELA_GREEN_NODE_H
#define VELA_GREEN_NODE_H

#include <string_view>
#include <unordered_map>
#include <span>
#include <vector>

#include "green_node_fwds.h"
#include "frontend/diagnostic.h"

namespace vela::parse {
    enum class NodeKind : uint16_t {
#define X(Kind) Kind,
        VELA_NODE_KINDS(X)
#undef X
    };

    enum NodeFlags : uint16_t {
        None = 0,
        IsNotMissing = 1 << 0,

        // This node contains the diagnostics directly (i.e. not inherited).
        HasDiagnosticsDirectly = 1 << 1,

        // Inherited upwards
        ContainsDiagnostics = 1 << 2,
        ContainsSkippedText = 1 << 3,

        InheritMask = IsNotMissing | ContainsDiagnostics | ContainsSkippedText
    };

    constexpr NodeFlags operator&(NodeFlags lhs, NodeFlags rhs) {
        return static_cast<NodeFlags>(
            static_cast<std::underlying_type_t<NodeFlags>>(lhs) & static_cast<std::underlying_type_t<NodeFlags>>(rhs)
        );
    }

    constexpr NodeFlags operator|(NodeFlags lhs, NodeFlags rhs) {
        return static_cast<NodeFlags>(
            static_cast<std::underlying_type_t<NodeFlags>>(lhs) | static_cast<std::underlying_type_t<NodeFlags>>(rhs)
        );
    }

    class GreenNode {
    public:
        GreenNode(const GreenNode &) = delete;
        GreenNode& operator=(const GreenNode &) = delete;

        virtual ~GreenNode() {
            if (hasFlag(HasDiagnosticsDirectly)) {
                m_diagnosticsTable.erase(this);
            }
        }

        virtual const GreenNode *getSlot(int index) const = 0;
        virtual NodeKind underlyingKind() const { return NodeKind::UndefinedSyntax; }

        uint16_t slotCount() const { return m_data.slotCount(); }

        NodeKind kind() const { return m_kind; }
        virtual bool isToken() const { return false; }
        virtual bool isTrivia() const { return false; }

        NodeFlags flags() const { return m_data.flags(); }
        bool hasFlag(NodeFlags flag) const { return m_data.flags() & flag; }
        bool containsDiagnostics() const { return hasFlag(ContainsDiagnostics); }
        bool containsSkippedText() const { return hasFlag(ContainsSkippedText); }

        NodePos fullWidth() const { return m_fullWidth; }
        NodePos width() const { return m_fullWidth - leadingTriviaWidth() - trailingTriviaWidth(); }
        virtual NodePos leadingTriviaWidth() const {
            return m_fullWidth != 0 ? firstTerminal()->leadingTriviaWidth() : 0;
        }
        virtual NodePos trailingTriviaWidth() const {
            return m_fullWidth != 0 ? lastTerminal()->trailingTriviaWidth() : 0;
        }
        bool hasLeadingTrivia() const { return leadingTriviaWidth() > 0; }
        bool hasTrailingTrivia() const { return trailingTriviaWidth() > 0; }

        const GreenNode *firstTerminal() const {
            const GreenNode *node = this;

            do {
                const GreenNode *first = nullptr;
                for (int i = 0; i < node->m_data.slotCount(); ++i) {
                    first = node->getSlot(i);
                    if (first != nullptr) {
                        break;
                    }
                }
                node = first;
            } while (node && node->m_data.slotCount() > 0);

            return node;
        }

        const GreenNode *lastTerminal() const {
            const GreenNode *node = this;

            do {
                const GreenNode *first = nullptr;
                for (int i = node->m_data.slotCount() - 1; i >= 0; --i) {
                    first = node->getSlot(i);
                    if (first != nullptr) {
                        break;
                    }
                }
                node = first;
            } while (node && node->m_data.slotCount() > 0);

            return node;
        }

        virtual int getSlotOffset(int idx) const {
            int offset = 0;
            for (int i = 0; i < idx; ++i) {
                if (const GreenNode *child = getSlot(i); child != nullptr) {
                    offset += child->m_fullWidth;
                }
            }

            return offset;
        }

        std::span<Diagnostic *> diagnostics() const {
            if (hasFlag(HasDiagnosticsDirectly)) {
                if (const auto it = m_diagnosticsTable.find(this); it != m_diagnosticsTable.end()) {
                    return it->second;
                }
            }

            return {};
        }

    protected:
        explicit GreenNode(NodeKind kind) : m_kind(kind) { }

        GreenNode(NodeKind kind, std::vector<Diagnostic *> diagnostics) : m_kind(kind) {
            if (!diagnostics.empty()) {
                m_data.setFlags(HasDiagnosticsDirectly | ContainsDiagnostics);
                m_diagnosticsTable[this] = std::move(diagnostics);
            }
        }

        void setFlags(NodeFlags flags) { m_data.setFlags(flags); }
        void setSlotCount(int n) { m_data.setSlotCount(static_cast<uint16_t>(n)); }
        void setFullWidth(NodePos width) { m_fullWidth = width; }

        void adjustFlagsAndWidth(const GreenNode *node) {
            m_data.setFlags(node->m_data.flags() & InheritMask);
            m_fullWidth += node->m_fullWidth;
        }

        inline static std::unordered_map<const GreenNode *, std::vector<Diagnostic *>> m_diagnosticsTable;
        inline static std::vector<Diagnostic *> emptyDiagnosticsBuf = {};

    private:
        struct NodeDataPacked {
            uint16_t slotCount() const {
                return data & SlotCountMask;
            }

            NodeFlags flags() const {
                return static_cast<NodeFlags>((data & FlagsMask) >> ShiftCountForFlags);
            }

            void setFlags(NodeFlags flags) {
                data |= static_cast<uint16_t>(static_cast<uint16_t>(flags) << ShiftCountForFlags);
            }

            void setSlotCount(uint16_t slotCount) {
                data = (data & ~SlotCountMask) | (slotCount & SlotCountMask);
            }

            uint16_t data = 0;
        private:
            constexpr static uint16_t SlotCountMask = 0b0000111111111111;
            constexpr static uint16_t FlagsMask = 0b1111000000000000;
            constexpr static int ShiftCountForFlags = 12;
        };

        NodeKind m_kind;
        NodeDataPacked m_data;
        NodePos m_fullWidth = 0;
    };

    class GreenTrivia final : public GreenNode {
    public:
        GreenTrivia(NodeKind kind, std::string_view text, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNode(kind, std::move(diagnostics)), m_text(text)
        {
            setFlags(kind == NodeKind::UnexpectedToken ? IsNotMissing | ContainsSkippedText : IsNotMissing);
            setFullWidth(static_cast<NodePos>(text.size()));
        }

        bool isTrivia() const override { return true; }
        std::string_view text() const { return m_text; }

        // Trivia has no slots
        const GreenNode *getSlot(int index) const override {
            (void)index;
            return nullptr;
        }

        NodePos leadingTriviaWidth() const override { return 0; }
        NodePos trailingTriviaWidth() const override { return 0; }

    private:
        std::string_view m_text;
    };

    class GreenToken final : public GreenNode {
    public:
        GreenToken(NodeKind kind,
                   std::string_view text,
                   const GreenNode *leading = nullptr,
                   const GreenNode *trailing = nullptr,
                   std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNode(kind, std::move(diagnostics)), m_text(text),
              m_leading(leading), m_trailing(trailing)
        {
            setFlags(kind == NodeKind::UnexpectedToken ? IsNotMissing | ContainsSkippedText : IsNotMissing);
            setFullWidth(static_cast<NodePos>(text.size()));

            if (m_leading != nullptr) {
                adjustFlagsAndWidth(m_leading);
            }

            if (m_trailing != nullptr) {
                adjustFlagsAndWidth(m_trailing);
            }
        }

        bool isToken() const override { return true; }
        std::string_view text() const { return m_text; }
        const GreenNode *leading() const { return m_leading; }
        const GreenNode *trailing() const { return m_trailing; }

        // Tokens are terminal, thus they cannot have children
        const GreenNode *getSlot(int index) const override {
            (void)index;
            return nullptr;
        }

        NodePos leadingTriviaWidth() const override {
            return m_leading != nullptr ? m_leading->fullWidth() : 0;
        }
        NodePos trailingTriviaWidth() const override {
            return m_trailing != nullptr ? m_trailing->fullWidth() : 0;
        }

    private:
        std::string_view m_text;
        const GreenNode *m_leading;
        const GreenNode *m_trailing;
    };

    template <int N>
    class GreenNodeN : public GreenNode {
    public:
        const GreenNode *getSlot(int index) const override {
            return index >= 0 && index < N ? m_slots[index] : nullptr;
        }

    protected:
        GreenNodeN(NodeKind kind, std::array<const GreenNode *, N> &&slots, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNode(kind, std::move(diagnostics)), m_slots(std::move(slots))
        {
            setSlotCount(N);
            for (const GreenNode *node : m_slots) {
                if (node != nullptr) {
                    adjustFlagsAndWidth(node);
                }
            }
        }

    private:
        std::array<const GreenNode *, N> m_slots;
    };

    class GreenNodeList : public GreenNode {
    public:
        const GreenNode *getSlot(int index) const override {
            return index >= 0 && index < static_cast<int>(m_slots.size())
                ? m_slots[static_cast<std::size_t>(index)]
                : nullptr;
        }

        NodeKind underlyingKind() const override { return m_underlyingKind; }

    protected:
        GreenNodeList(NodeKind kind, NodeKind underlyingKind, std::vector<const GreenNode *> slots, std::vector<Diagnostic *> diagnostics = emptyDiagnosticsBuf)
            : GreenNode(kind, std::move(diagnostics)), m_slots(std::move(slots)), m_underlyingKind(underlyingKind)
        {
            setSlotCount(static_cast<int>(m_slots.size()));
            for (const GreenNode *node : m_slots) {
                if (node != nullptr) {
                    adjustFlagsAndWidth(node);
                }
            }
        }

    private:
        std::vector<const GreenNode *> m_slots;
        NodeKind m_underlyingKind;
    };
}

#endif //VELA_GREEN_NODE_H
