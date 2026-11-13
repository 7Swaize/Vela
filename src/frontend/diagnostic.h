#ifndef VELA_DIAGNOSTIC_H
#define VELA_DIAGNOSTIC_H

#include <string>

struct Position {
    uint32_t line;
    uint32_t character;
};

struct Range {
    Position start; // inclusive
    Position end; // exclusive
};

struct Diagnostic {
    std::string mesage;
    Range span;
};

#endif //VELA_DIAGNOSTIC_H
