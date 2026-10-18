#ifndef VELA_TABLE_GEN_H
#define VELA_TABLE_GEN_H

#include <array>
#include <string_view>

namespace vela::lex {
    struct NibblesTable {
        alignas(16) std::array<uint8_t, 16> high;
        alignas(16) std::array<uint8_t, 16> low;
        alignas(16) std::array<uint8_t, 16> ws;
    };
} // namespace vela::lex

namespace vela::lex::detail {
    struct Rect {
        int bit;
        std::string_view members;
    };

    inline constexpr std::array kRects = {
            Rect{.bit = 0, .members = "ABCDEFGHIJKLMNOabcdefghijklmno"}, // ident
            Rect{.bit = 1, .members = "PQRSTUVWXYZpqrstuvwxyz"}, // ident
            Rect{.bit = 2, .members = "_"}, // ident
            Rect{.bit = 3, .members = "0123456789"}, // ident
            Rect{.bit = 4, .members = "!%&()*+,-./"}, // punct
            Rect{.bit = 5, .members = ";<=>{|}~"}, // punct
            Rect{.bit = 6, .members = "[]^"} // punct
    };

    inline constexpr std::string_view kWhiteSpace = " \t\n\r";

    consteval NibblesTable compute_tables() {
        NibblesTable table{};

        for (const auto &[bit, members]: kRects) {
            uint16_t rows = 0, cols = 0;

            for (const char c: members) {
                rows |= 1u << (static_cast<uint8_t>(c) >> 4);
                cols |= 1u << (static_cast<uint8_t>(c) & 0x0F);
            }

            for (std::size_t i = 0; i < 16; ++i) {
                if (rows >> i & 1) {
                    table.high[i] |= static_cast<uint8_t>(1u << bit);
                }

                if (cols >> i & 1) {
                    table.low[i] |= static_cast<uint8_t>(1u << bit);
                }
            }
        }

        for (const char c: kWhiteSpace) {
            table.ws[static_cast<uint8_t>(c) & 0x0F] = static_cast<uint8_t>(c);
        }

        return table;
    }
} // namespace vela::lex::detail

namespace vela::lex {
    inline constexpr NibblesTable kTables = detail::compute_tables();
} // namespace vela::lex

#endif // VELA_TABLE_GEN_H
