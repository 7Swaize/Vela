#ifndef VELA_SYNTAX_NODE_H
#define VELA_SYNTAX_NODE_H

#include <algorithm>
#include <cassert>
#include <functional>
#include <iterator>
#include <span>
#include <string_view>
#include <type_traits>

#include "syntax_node_fwds.h"
#include "frontend/diagnostic.h"
#include "green/green_node.h"
#include "shared/helpers.h"

namespace vela::parse {
    class SyntaxElement {
    public:
        NodeKind kind() const { return m_green->kind(); }
        bool isToken() const { return m_green->isToken(); }
        bool isNode() const { return m_green->isNode(); }

        const GreenNode *green() const { return m_green; }
        const SyntaxNode *parent() const { return m_parent; }
        NodePos position() const { return m_position; }
        NodePos end() const { return m_position + m_green->fullWidth(); }
        int indexInParent() const { return m_index; }

        NodePos fullWidth() const { return m_green->fullWidth(); }
        shared::Range<NodePos> fullSpan() const {
            return shared::Range { .start = m_position, .end =  m_position + m_green->fullWidth() };
        }

        bool containsDiagnostics() const { return m_green->containsDiagnostics(); }
        std::span<Diagnostic *> diagnostics() const { return m_green->diagnostics(); }

        // clang-format off
        template <typename V>
            requires std::is_same_v<
                std::invoke_result_t<V, const SyntaxNode *>,
                std::invoke_result_t<V, const SyntaxToken *>
            >
        decltype(auto) visit_node_or_token_base(V &&visitor) const;
        // clang-format on

    protected:
        SyntaxElement(const GreenNode *green, const SyntaxNode *parent, const NodePos position, const int index)
            : m_green(green), m_parent(parent), m_position(position), m_index(index) { }

        const GreenNode *m_green;
        const SyntaxNode *m_parent;
        NodePos m_position;
        int m_index;
    };

    class SyntaxNode : public SyntaxElement {
    protected:
        const SyntaxElement *at(int index) const;

    public:
        int slotCount() const { return m_green->slotCount(); }
        const SyntaxElement *child(const int i) const { return at(i); }

        const SyntaxToken *findToken(const NodePos start) const {
            return findTokenH(this, start);
        }

        template <RequiresSyntaxElement T>
        const T *getOptionalNode(const int index) const {
            return static_cast<const T *>(at(index));
        }

        template <RequiresSyntaxElement T>
        const T *getRequiredNode(const int index) const {
            const T * target = static_cast<const T *>(at(index));
            assert(target != nullptr);
            return target;
        }

    protected:
        SyntaxNode(const GreenNode *green,
                  const SyntaxNode *parent,
                  const SyntaxTree *syntaxTree,
                  const NodePos position,
                  const int index,
                  const SyntaxElement **slots)
            : SyntaxElement(green, parent, position, index), m_slots(slots), m_syntaxTree(syntaxTree) { }

    protected:
        const SyntaxElement **m_slots;

    private:
        static const SyntaxToken *findTokenH(const SyntaxElement *curr, NodePos start);

        const SyntaxTree *m_syntaxTree;
    };

    class SyntaxToken final : public SyntaxElement {
    public:
        SyntaxToken(const GreenNode *green, const SyntaxNode *parent, const NodePos position, const int index)
            : SyntaxElement(green, parent, position, index) { }

        const GreenToken *token() const { return static_cast<const GreenToken *>(m_green); }
        std::string_view text() const { return token()->text(); }
    };

    struct SyntaxTrivia {
        const GreenNode *green;
        const SyntaxToken *token;
        int index;
        NodePos position;
    };

    class ListSyntax final : public SyntaxNode {
    public:
        ListSyntax(const GreenNode *green,
                   const SyntaxNode *parent,
                   const SyntaxTree *syntaxTree,
                   const NodePos position,
                   const int index,
                   const SyntaxElement **slots)
            : SyntaxNode(green, parent, syntaxTree, position, index, slots) { }

        int count() const { return slotCount(); }

    private:
        template <RequiresSyntaxElement T>
        friend class SyntaxList;
    };

    class SeparatedListSyntax final : public SyntaxNode {
    public:
        SeparatedListSyntax(const GreenNode *green,
                            const SyntaxNode *parent,
                            const SyntaxTree *syntaxTree,
                            const NodePos position,
                            const int index,
                            const SyntaxElement **slots)
            : SyntaxNode(green, parent, syntaxTree, position, index, slots) { }

        int count() const { return slotCount(); }

        int elementCount() const { return (slotCount() + 1) / 2; }
        int separatorCount() const { return slotCount() / 2; }

        const SyntaxElement *element(const int i) const { return at(2 * i); }
        const SyntaxElement *separator(const int i) const { return at(2 * i + 1); }

    private:
        template <RequiresSyntaxElement T>
        friend class SeparatedSyntaxList;
    };

    template <RequiresSyntaxElement T>
    class SyntaxList {
    public:
        explicit SyntaxList(const ListSyntax *node) : m_node(node) { }

        const ListSyntax *node() const { return m_node; }
        int count() const { return m_node != nullptr ? m_node->count() : 0; }

        const T *operator[](const int index) const {
            const SyntaxElement *element = m_node->child(index);
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
            explicit Iterator_(const ListSyntax *node, const int index) noexcept
                : m_node(node), m_index(index) { }

            reference operator*() const { return static_cast<reference>(m_node->child(m_index)); }

            Iterator_ &operator++() {
                ++m_index;
                return *this;
            }

            Iterator_ operator++(int) {
                Iterator_ tmp = *this;
                ++(*this);
                return tmp;
            }

            friend bool operator==(const Iterator_ &a, const Iterator_ &b) = default;

        private:
            const ListSyntax *m_node = nullptr;
            int m_index = 0;
        };

        using iterator = Iterator_;
        using const_iterator = Iterator_;

        const_iterator begin() const {
            return m_node == nullptr
                ? const_iterator(nullptr)
                : const_iterator(m_node, 0);
        }

        const_iterator end() const {
            return m_node == nullptr
                ? const_iterator(nullptr)
                : const_iterator(m_node, m_node->count());
        }

    private:
        const ListSyntax *m_node = nullptr;
    };

    template <RequiresSyntaxElement T>
    class SeparatedSyntaxList {
    public:
        explicit SeparatedSyntaxList(const SeparatedListSyntax *node) : m_node(node) { }

        const SeparatedListSyntax *node() const { return m_node; }
        int count() const { return m_node != nullptr ? m_node->count() : 0; }
        int elementCount() const { return m_node != nullptr ? m_node->elementCount() : 0; }
        int seperatorCount() const { return m_node != nullptr ? m_node->separatorCount() : 0; }

        // Indexer and iterator only expose elements
        const T *operator[](const int index) const {
            return static_cast<const T *>(m_node->element(index));
        }

        const T *first() const { return elementCount() == 0 ? nullptr : operator[](0); }
        const T *last() const { return elementCount() == 0 ? nullptr : operator[](elementCount() - 1); }

        class Iterator_ final {
        public:
            using iterator_concept = std::forward_iterator_tag;
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = const T *;
            using reference = const T *;
            using pointer = void;

            Iterator_() noexcept = default;
            explicit Iterator_(const SeparatedListSyntax *m_node, int index) noexcept
                : m_node(m_node), m_index(index) { }

            reference operator*() const { return static_cast<reference>(*m_node->child(m_index)); }

            Iterator_ &operator++() {
                m_index = std::min(m_index + 2, m_node->count());
                return *this;
            }

            Iterator_ operator++(int) {
                Iterator_ tmp = *this;
                ++(*this);
                return tmp;
            }

            friend bool operator==(const Iterator_ &a, const Iterator_ &b) = default;

        private:
            const SeparatedListSyntax *m_node = nullptr;
            int m_index = 0;
        };

        using iterator = Iterator_;
        using const_iterator = Iterator_;

        const_iterator begin() const {
            return m_node == nullptr
                ? const_iterator(nullptr)
                : const_iterator(m_node, 0);
        }

        const_iterator end() const {
            return m_node == nullptr
                ? const_iterator(nullptr)
                : const_iterator(m_node, m_node->count());
        }

    private:
        const SeparatedListSyntax *m_node = nullptr;
    };

    // clang-format off
    template <typename V>
        requires std::is_same_v<
            std::invoke_result_t<V, const SyntaxNode *>,
            std::invoke_result_t<V, const SyntaxToken *>
        >
    decltype(auto) SyntaxElement::visit_node_or_token_base(V &&visitor) const {
        if (isToken()) {
            return std::invoke(std::forward<V>(visitor), static_cast<const SyntaxToken *>(this));
        }

        return std::invoke(std::forward<V>(visitor), static_cast<const SyntaxNode *>(this));
    }
    // clang-format on
}

#endif //VELA_SYNTAX_NODE_H
