#ifndef VELA_BLENDER_H
#define VELA_BLENDER_H

#include <stack>

#include "frontend/syntax/syntax_node_fwds.h"
#include "frontend/syntax/green/green_node_fwds.h"
#include "lexer/lex.h"
#include "lexer/token.h"

namespace vela::parse {
    class BlendEntry {
    public:
        BlendEntry(const GreenNode *node, NodePos newStart)
            : BlendEntry(node, newStart, false, 0, 0) { }

        BlendEntry(lex::LexPos textStart, lex::LexPos textEnd)
            : BlendEntry(nullptr, 0, true, textStart, textEnd) { }

        const GreenNode *node() const noexcept { return m_node; }
        NodePos newStart() const noexcept { return m_newStart; }
        bool isText() const noexcept { return m_isText; }
        lex::LexPos textStart() const noexcept { return m_textStart; }
        lex::LexPos textEnd() const noexcept { return m_textEnd; }

    protected:
        BlendEntry(const GreenNode *node, NodePos newStart, bool isText, lex::LexPos textStart, lex::LexPos textEnd)
            : m_node(node), m_newStart(newStart), m_isText(isText), m_textStart(textStart), m_textEnd(textEnd) { }

    private:
        const GreenNode *m_node;
        NodePos m_newStart;
        bool m_isText;
        lex::LexPos m_textStart;
        lex::LexPos m_textEnd;
    };

    struct TextChangeRange {
        
    };

    class Blender {
    public:
        class Cursor;
        class Reader;

        Blender(lex::Lexer lexer, const SyntaxNode *oldTree);

    private:
        lex::Lexer m_lexer;
        Cursor m_oldTreeCursor;
        std::stack<Text

        NodePos m_changeDelta;
        NodePos m_newPosition;

    };
} // namespace vela::parse

#endif //VELA_BLENDER_H
