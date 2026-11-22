/* C++ code produced by gperf version 3.0.3 */
/* Command-line: /Library/Developer/CommandLineTools/usr/bin/gperf gperf/vela_keywords.gperf  */
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

#line 1 "gperf/vela_keywords.gperf"

#include <cstddef>
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

#line 31 "gperf/vela_keywords.gperf"
struct PerfectKeywordHashTableEntry { const char* name; TokenKind kind; };
/* maximum key range = 28, duplicates = 0 */

class VelaKeywords
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct PerfectKeywordHashTableEntry *lookup (const char *str, unsigned int len);
};

inline unsigned int
VelaKeywords::hash (register const char *str, register unsigned int len)
{
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
      30, 30, 30, 30, 30, 30, 30, 15, 20, 10,
      10,  5,  5, 30, 30,  0, 30, 30,  5, 30,
      10, 30,  0, 30,  5,  0,  0,  5, 10, 10,
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
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[3]];
      /*FALLTHROUGH*/
      case 3:
      case 2:
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

const struct PerfectKeywordHashTableEntry *
VelaKeywords::lookup (register const char *str, register unsigned int len)
{
  enum
    {
      TOTAL_KEYWORDS = 20,
      MIN_WORD_LENGTH = 2,
      MAX_WORD_LENGTH = 8,
      MIN_HASH_VALUE = 2,
      MAX_HASH_VALUE = 29
    };

  static const unsigned char lengthtable[] =
    {
       0,  0,  2,  3,  4,  0,  6,  2,  3,  4,  5,  6,  0,  3,
       4,  5,  6,  0,  8,  4,  5,  0,  0,  0,  4,  5,  0,  7,
       0,  4
    };
  static const struct PerfectKeywordHashTableEntry wordlist[] =
    {
      {""}, {""},
#line 39 "gperf/vela_keywords.gperf"
      {"if", TokenKind::KwIf},
#line 50 "gperf/vela_keywords.gperf"
      {"int", TokenKind::KwInt},
#line 47 "gperf/vela_keywords.gperf"
      {"this", TokenKind::KwThis},
      {""},
#line 53 "gperf/vela_keywords.gperf"
      {"string", TokenKind::KwString},
#line 38 "gperf/vela_keywords.gperf"
      {"fn", TokenKind::KwFn},
#line 41 "gperf/vela_keywords.gperf"
      {"for", TokenKind::KwFor},
#line 44 "gperf/vela_keywords.gperf"
      {"true", TokenKind::KwTrue},
#line 45 "gperf/vela_keywords.gperf"
      {"false", TokenKind::KwFalse},
#line 36 "gperf/vela_keywords.gperf"
      {"struct", TokenKind::KwStruct},
      {""},
#line 48 "gperf/vela_keywords.gperf"
      {"new", TokenKind::KwNew},
#line 40 "gperf/vela_keywords.gperf"
      {"else", TokenKind::KwElse},
#line 37 "gperf/vela_keywords.gperf"
      {"class", TokenKind::KwClass},
#line 43 "gperf/vela_keywords.gperf"
      {"return", TokenKind::KwReturn},
      {""},
#line 34 "gperf/vela_keywords.gperf"
      {"comptime", TokenKind::KwComptime},
#line 46 "gperf/vela_keywords.gperf"
      {"null", TokenKind::KwNull},
#line 42 "gperf/vela_keywords.gperf"
      {"while", TokenKind::KwWhile},
      {""}, {""}, {""},
#line 49 "gperf/vela_keywords.gperf"
      {"void", TokenKind::KwVoid},
#line 51 "gperf/vela_keywords.gperf"
      {"float", TokenKind::KwFloat},
      {""},
#line 35 "gperf/vela_keywords.gperf"
      {"concept", TokenKind::KwConcept},
      {""},
#line 52 "gperf/vela_keywords.gperf"
      {"bool", TokenKind::KwBool}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        if (len == lengthtable[key])
          {
            register const char *s = wordlist[key].name;

            if (*str == *s && !memcmp (str + 1, s + 1, len - 1))
              return &wordlist[key];
          }
    }
  return 0;
}
#line 54 "gperf/vela_keywords.gperf"


}

#if defined(__clang__)
    #pragma clang diagnostic pop
#elif defined(__GNUC__)
    #pragma GCC diagnostic pop
#elif defined(_MSC_VER)
    #pragma warning(pop)
#endif