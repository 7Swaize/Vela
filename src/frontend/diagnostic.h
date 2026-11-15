#ifndef VELA_DIAGNOSTIC_H
#define VELA_DIAGNOSTIC_H

#include <string>

#include "shared/helpers.h"

struct Position {
    uint32_t line;
    uint32_t character;
};

struct Diagnostic {
    std::string mesage;
    vela::shared::Range<Position> span;
};

#endif //VELA_DIAGNOSTIC_H
