/* C++ code produced by gperf version 3.0.1 */
/* Command-line: 'C:\\Program Files (x86)\\GnuWin32\\bin\\gperf.exe' '.\\gperf\\vela_keywords.gperf'  */
/* Computed positions: -k'1,4' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#include <cstring>
#include "token.h"

#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Weverything"
#elif defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wsign-conversion"
    #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    #pragma GCC diagnostic ignored "-Wunused-function"
#elif defined(_MSC_VER)
    #pragma warning(push, 0)
#endif

namespace vela::lex {
    struct PerfectKeywordHashTableEntry {
        const char *name;
        TokenKind kind;
    };

    /* maximum key range = 28, duplicates = 0 */

    class VelaKeywords {
    private:
        static inline unsigned int hash(const char *str, unsigned int len);

    public:
        static const struct PerfectKeywordHashTableEntry *lookup(const char *str, unsigned int len);
    };

    inline unsigned int
    VelaKeywords::hash(const char *str, const unsigned int len) {
        static const unsigned char asso_values[] =
        {
            30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
            30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
            30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
            30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
            30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
            30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
            30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
            30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
            30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
            30, 30, 30, 30, 30, 30, 30, 10, 20, 0,
            10, 5, 5, 30, 30, 0, 30, 30, 5, 30,
            10, 30, 30, 30, 5, 0, 0, 5, 10, 5,
            30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
            30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
            30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
            30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
            30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
            30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
            30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
            30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
            30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
            30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
            30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
            30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
            30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
            30, 30, 30, 30, 30, 30
        };
        int hval = len;

        switch (hval) {
            default:
                hval += asso_values[static_cast<unsigned char>(str[3])];
            /*FALLTHROUGH*/
            case 3:
            case 2:
            case 1:
                hval += asso_values[static_cast<unsigned char>(str[0])];
                break;
        }
        return hval;
    }

    inline const struct PerfectKeywordHashTableEntry *VelaKeywords::lookup(const char *str, const unsigned int len) {
        enum {
            TOTAL_KEYWORDS = 18,
            MIN_WORD_LENGTH = 2,
            MAX_WORD_LENGTH = 6,
            MIN_HASH_VALUE = 2,
            MAX_HASH_VALUE = 29
        };

        static const unsigned char lengthtable[] =
        {
            0, 0, 2, 3, 4, 5, 6, 2, 3, 4, 5, 6, 0, 3,
            4, 5, 6, 0, 0, 4, 5, 0, 0, 0, 4, 0, 0, 0,
            0, 4
        };
        static const struct PerfectKeywordHashTableEntry wordlist[] =
        {
            {.name = ""}, {.name = ""},
            {.name = "if", .kind = TokenKind::KwIf},
            {.name = "int", .kind = TokenKind::KwInt},
            {.name = "this", .kind = TokenKind::KwThis},
            {.name = "class", .kind = TokenKind::KwClass},
            {.name = "string", .kind = TokenKind::KwString},
            {.name = "fn", .kind = TokenKind::KwFn},
            {.name = "for", .kind = TokenKind::KwFor},
            {.name = "true", .kind = TokenKind::KwTrue},
            {.name = "false", .kind = TokenKind::KwFalse},
            {.name = "struct", .kind = TokenKind::KwStruct},
            {.name = ""},
            {.name = "new", .kind = TokenKind::KwNew},
            {.name = "else", .kind = TokenKind::KwElse},
            {.name = "while", .kind = TokenKind::KwWhile},
            {.name = "return", .kind = TokenKind::KwReturn},
            {.name = ""}, {.name = ""},
            {.name = "null", .kind = TokenKind::KwNull},
            {.name = "float", .kind = TokenKind::KwFloat},
            {.name = ""}, {.name = ""}, {.name = ""},
            {.name = "void", .kind = TokenKind::KwVoid},
            {.name = ""}, {.name = ""}, {.name = ""}, {.name = ""},
            {.name = "bool", .kind = TokenKind::KwBool}
        };

        if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH) {
            int key = hash(str, len);

            if (key <= MAX_HASH_VALUE && key >= 0)
                if (len == lengthtable[key]) {
                    const char *s = wordlist[key].name;

                    if (*str == *s && !memcmp(str + 1, s + 1, len - 1))
                        return &wordlist[key];
                }
        }
        return nullptr;
    }
}

#if defined(__clang__)
    #pragma clang diagnostic pop
#elif defined(__GNUC__)
    #pragma GCC diagnostic pop
#elif defined(_MSC_VER)
    #pragma warning(pop)
#endif
