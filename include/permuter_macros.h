#ifndef PERMUTER_MACROS_H
#define PERMUTER_MACROS_H

#ifdef PERMUTER

#define PERM_SELF_ASSIGN(var) PERM_GENERAL(var, (var = var))

#define PERM_SELF_ASSIGN_LOAD(var) (var = var)

#define PERM_EXPR_ORDER(var, op, val) \
    PERM_GENERAL(var = var op val, var op##= val, (var = var op val))

#define PERM_DO_WHILE_LL_BEGIN do {
#define PERM_DO_WHILE_LL_END } while ((long long) 0);

#define PERM_EMPTY_IF_1 if (1) { }
#define PERM_EMPTY_IF_2 if (1) { } if (1) { }
#define PERM_EMPTY_IF_3 if (1) { } if (1) { } if (1) { }

#define PERM_ADDRESS_AS_INT(reg, global, type) \
    reg = (unsigned int)&global; \
    reg = *(type *)reg

#define PERM_VAR_CHAIN(dst, tmp, expr) tmp = expr; dst = tmp

#endif

#endif
