#ifndef VELA_SYNTAX_NODE_H
#define VELA_SYNTAX_NODE_H

#include <cassert>
#include <concepts>
#include <iterator>
#include <optional>
#include <span>
#include <vector>

#include "syntax_tree.h"
#include "frontend/diagnostic.h"
#include "green/green_node.h"

namespace vela::parse {
    class SyntaxNode;
    class SyntaxToken;

    template <typename T>
    concept RequiresSyntaxNode = std::derived_from<T, SyntaxNode>;

    class SyntaxNode {
    public:
        SyntaxNode(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : m_green(green), m_syntaxTree(syntaxTree), m_position(position), m_parent(parent) { }

        virtual ~SyntaxNode() { }

        virtual NodeKind underlyingKind() const { return NodeKind::UndefinedSyntax; }

        NodeKind kind() const { return m_green->kind(); }

        shared::Range<NodePos> fullSpan() const { return {.start = m_position, .end = m_position + m_green->fullWidth()}; }
        std::span<Diagnostic *> diagnostics() const { return m_green->diagnostics(); }
        NodePos position() const { return m_position; }
        const SyntaxNode *parent() const { return m_parent; }

        template <RequiresSyntaxNode T>
        const T *getOptionalNode(const T *target, int index) {
            if (target == nullptr) {
                target = static_cast<const T *>(getOrCreateChildren()[index]);
            }

            return target;
        }

        template <RequiresSyntaxNode T>
        const T *getRequiredNode(const T *target, int index) {
            if (target == nullptr) {
                target = static_cast<const T *>(getOrCreateChildren()[index]);
            }

            assert(target != nullptr);
            return target;
        }

    protected:
        std::span<const SyntaxNode *> getOrCreateChildren() {
            if (m_children.has_value()) {
                return m_children.value();
            }

            createChildren();
            return m_children.value();
        }

        const GreenNode *m_green;
        const SyntaxTree *m_syntaxTree;
        NodePos m_position;

    private:
        void createChildren();

        const SyntaxNode *m_parent;
        std::optional<std::vector<const SyntaxNode*>> m_children = std::nullopt;
    };

    class SyntaxToken final : public SyntaxNode {
    public:
        SyntaxToken(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : SyntaxNode(green, parent, syntaxTree, position) { }

        const GreenToken *token() const { return static_cast<const GreenToken *>(m_green); }
        std::string_view text() const { return token()->text(); }
    };

    class SyntaxTrivia final : public SyntaxNode {
    public:
        SyntaxTrivia(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : SyntaxNode(green, parent, syntaxTree, position) { }
    };

    class ListSyntax final : public SyntaxNode {
    public:
        ListSyntax(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : SyntaxNode(green, parent, syntaxTree, position) { }

        uint16_t count() const { return m_green->slotCount(); }

        const SyntaxNode *at(const int index) const {
            return const_cast<ListSyntax *>(this)->getOrCreateChildren()[static_cast<std::size_t>(index)];
        }

        std::span<const SyntaxNode *> children() const {
            return const_cast<ListSyntax *>(this)->getOrCreateChildren();
        }
    };

    class SeparatedListSyntax final : public SyntaxNode {
    public:
        SeparatedListSyntax(const GreenNode *green, const SyntaxNode *parent, const SyntaxTree *syntaxTree, const NodePos position)
            : SyntaxNode(green, parent, syntaxTree, position) { }

        uint16_t count() const { return m_green->slotCount(); }
        uint16_t elementCount() const { return (m_green->slotCount() + 1) / 2; }
        uint16_t separatorCount() const { return m_green->slotCount() / 2; }

        const SyntaxNode *at(const int index) const {
            return const_cast<SeparatedListSyntax *>(this)->getOrCreateChildren()[static_cast<std::size_t>(index)];
        }

        std::span<const SyntaxNode *> children() const {
            return const_cast<SeparatedListSyntax *>(this)->getOrCreateChildren();
        }
    };

    template <typename T>
    class SyntaxList {
    public:
        explicit SyntaxList(const ListSyntax *node) : m_node(node) { }

        const ListSyntax *node() const { return m_node; }
        int count() const { return m_node != nullptr ? m_node->count() : 0; }

        const T *operator[](const int index) const {
            const SyntaxNode *element = m_node->at(index);
            return static_cast<const T *>(element);
        }

        const T *first() const { return count() == 0 ? nullptr : operator[](0); }
        const T *last() const { return count() == 0 ? nullptr : operator[](count() - 1); }

        class Iterator_ final {
        public:
            using iterator_concept = std::forward_iterator_tag;
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = const T *;
            using reference = const T *;
            using pointer = void;

            Iterator_() noexcept = default;
            explicit Iterator_(const SyntaxNode *const *ptr) noexcept : m_ptr(ptr) { }

            reference operator*() const { return *m_ptr; }

            Iterator_ &operator++() {
                ++m_ptr;
                return *this;
            }

            Iterator_ operator++(int) {
                Iterator_ tmp = *this;
                ++(*this);
                return tmp;
            }

            friend bool operator==(const Iterator_ &a, const Iterator_ &b) = default;

        private:
            const SyntaxNode *const *m_ptr = nullptr;
        };

        using iterator = Iterator_;
        using const_iterator = Iterator_;

        const_iterator begin() const {
            return m_node == nullptr ? const_iterator(nullptr) : const_iterator(m_node->children().data());
        }

        const_iterator end() const {
            return m_node == nullptr ? const_iterator(nullptr) : const_iterator(m_node->children().end());
        }

    private:
        const ListSyntax *m_node = nullptr;
    };

    template <typename T>
    class SeparatedSyntaxList {
    public:
        explicit SeparatedSyntaxList(const SeparatedListSyntax *node) : m_node(node) { }

        const SeparatedListSyntax *node() const { return m_node; }
        int count() const { return m_node != nullptr ? m_node->count() : 0; }
        int elementCount() const { return m_node != nullptr ? m_node->elementCount() : 0; }
        int seperatorCount() const { return m_node != nullptr ? m_node->separatorCount() : 0; }

        const T *operator[](const int index) const {
            const SyntaxNode *element = m_node->at(index);
            return static_cast<const T *>(element);
        }

        const T *first() const { return count() == 0 ? nullptr : operator[](0); }
        const T *last() const { return count() == 0 ? nullptr : operator[](count() - 1); }

        class Iterator_ final {
        public:
            using iterator_concept = std::forward_iterator_tag;
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = const T *;
            using reference = const T *;
            using pointer = void;

            Iterator_() noexcept = default;
            explicit Iterator_(const SyntaxNode *const *ptr, const SyntaxNode *const *end) noexcept : m_ptr(ptr), m_end(end) { }

            reference operator*() const { return *m_ptr; }

            Iterator_ &operator++() {
                m_end - m_ptr <= 2 ? m_ptr = m_end : m_ptr += 2;
                return *this;
            }

            Iterator_ operator++(int) {
                Iterator_ tmp = *this;
                ++(*this);
                return tmp;
            }

            friend bool operator==(const Iterator_ &a, const Iterator_ &b) = default;

        private:
            const SyntaxNode *const *m_ptr = nullptr;
            const SyntaxNode *const *m_end = nullptr;
        };

        using iterator = Iterator_;
        using const_iterator = Iterator_;

        const_iterator begin() const {
            return m_node == nullptr ? const_iterator(nullptr) : const_iterator(m_node->children().data(), m_node->children().end());
        }

        const_iterator end() const {
            return m_node == nullptr ? const_iterator(nullptr) : const_iterator(m_node->children().end());
        }

    private:
        const SeparatedListSyntax *m_node = nullptr;
    };
}

#endif //VELA_SYNTAX_NODE_H
