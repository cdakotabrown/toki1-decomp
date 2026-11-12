/* @SymbolStart: 0x80152AA8 */
/* @SymbolSize: 236 */
/* @PostProc: patch_stack_complete --old 40 --new 48 */
/* @PostProc: remap_stack_slots --map '30:42' */
/* @PostProc: patch_insn_reg_field 0x5c:rs:t8:v0 0x5c:rt:v0:t8 */
/* Address: 0x80152AB0 */
/* Size: 228 bytes (57 insns). Honest re-derive of a pad_trailing_nop fake. */
#include "types.h"

extern s32 D_800EECBC;

extern volatile u8 D_800B1746;
extern volatile u8 D_800B1AF6;
extern s16 D_80094730;
extern u8 D_8012530C;

extern void func_8004CF30(void);
extern void func_80011DFC(void);

s32 func_80152AB0(void) {
    s32 t6_v = D_800EECBC;

    s32 v0;
    s16 old;

    if (t6_v == 1) {
        v0 = D_800B1746;
        if (v0 == 7) {
            old = D_800B1AF6;
            func_8004CF30();
            v0 = D_800B1AF6;
            if (v0 != old) {
                D_80094730 = 10;
                D_800B1AF6 = (u8)(v0 + 11);
            }
            return 0;
        }
        D_80094730 = 9;
        if (v0 == 2) {
            D_80094730 = 8;
        } else if (v0 == 4 || v0 == 8 || v0 == 5) {
            D_80094730 = 7;
        }
        v0 = D_800B1AF6;
        D_800B1AF6 = (u8)(v0 + 12);
        goto call_dfc;
    }
    D_8012530C = 1;
call_dfc:
    func_80011DFC();
}
