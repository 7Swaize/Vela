#ifndef VELA_SEMA_H
#define VELA_SEMA_H

#include "ast.h"

namespace vela::parse::sema {
    void build_symbol_table(const ast::Program *root);
} // vela::parse::sema

#endif //VELA_SEMA_H
