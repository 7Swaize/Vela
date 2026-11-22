#ifndef VELA_DIAGNOSTIC_H
#define VELA_DIAGNOSTIC_H

#include <string>

struct Position {
    uint32_t line;
    uint32_t character;

    auto operator<=>(const Position&) const = default;
};

struct PositionSpan {
    Position start;
    Position end;
};

struct Diagnostic {
    std::string message;
    PositionSpan span;
};

#endif //VELA_DIAGNOSTIC_H
