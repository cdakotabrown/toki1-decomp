/*
 * permuter_macros.h - IDO-specific permutation macros for decomp-permuter
 *
 * Include this in your permuter source files to access proven patterns.
 * Updated January 2026 with byte-perfect matching discoveries.
 */

#ifndef PERMUTER_MACROS_H
#define PERMUTER_MACROS_H

#ifdef PERMUTER

/*
 * PERM_SELF_ASSIGN - Self-assignment for register forcing
 *
 * Usage: PERM_SELF_ASSIGN(D_GLOBAL)
 * Expands to: PERM_GENERAL(D_GLOBAL, (D_GLOBAL = D_GLOBAL))
 *
 * Forces the compiler to use same register for address and value.
 * Produces the `lui v1` -> `lbu v1,offset(v1)` pattern.
 *
 * BREAKTHROUGH: This achieved byte-perfect match on Fade_In_Main!
 */
#define PERM_SELF_ASSIGN(var) PERM_GENERAL(var, (var = var))

/*
 * PERM_SELF_ASSIGN_LOAD - Self-assignment with load for register forcing
 *
 * Usage: temp = PERM_SELF_ASSIGN_LOAD(D_GLOBAL);
 * Try both variants to match target instruction ordering.
 */
#define PERM_SELF_ASSIGN_LOAD(var) (var = var)

/*
 * PERM_EXPR_ORDER - Expression order variations
 *
 * IDO treats v = v + 4, v += 4, and (v = v + 4) differently
 * for instruction scheduling. Use this macro to try all variants.
 *
 * Usage: PERM_EXPR_ORDER(v, +, 4)
 */
#define PERM_EXPR_ORDER(var, op, val) \
    PERM_GENERAL(var = var op val, var op##= val, (var = var op val))

/*
 * PERM_DO_WHILE_LL - do-while with long long 0 pattern
 *
 * Prevents compiler from optimizing away the do-while loop.
 * Useful for preserving variables across function calls.
 *
 * Usage:
 *   PERM_DO_WHILE_LL_BEGIN
 *   preserved = arg0;
 *   func_A();
 *   func_B(preserved);
 *   PERM_DO_WHILE_LL_END
 */
#define PERM_DO_WHILE_LL_BEGIN do {
#define PERM_DO_WHILE_LL_END } while ((long long) 0);

/*
 * PERM_EMPTY_IF - Empty if(1) padding for delay slots
 *
 * Insert padding to affect delay slot filling.
 * Try placing AFTER comparisons, not just at returns.
 *
 * Usage: PERM_EMPTY_IF(3)  // Insert 3 empty if(1) statements
 */
#define PERM_EMPTY_IF_1 if (1) { }
#define PERM_EMPTY_IF_2 if (1) { } if (1) { }
#define PERM_EMPTY_IF_3 if (1) { } if (1) { } if (1) { }

/*
 * PERM_ADDRESS_AS_INT - Address-as-integer pattern
 *
 * Forces register reuse in address/value operations.
 *
 * Usage:
 *   register unsigned int v;
 *   PERM_ADDRESS_AS_INT(v, D_GLOBAL, unsigned char)
 */
#define PERM_ADDRESS_AS_INT(reg, global, type) \
    reg = (unsigned int)&global; \
    reg = *(type *)reg

/*
 * PERM_VAR_CHAIN - Variable chaining for register forcing
 *
 * Creates explicit register transfer chain.
 *
 * Usage: PERM_VAR_CHAIN(v1, t6, func_call(arg))
 * Expands to: t6 = func_call(arg); v1 = t6;
 */
#define PERM_VAR_CHAIN(dst, tmp, expr) tmp = expr; dst = tmp

#endif /* PERMUTER */

#endif /* PERMUTER_MACROS_H */
