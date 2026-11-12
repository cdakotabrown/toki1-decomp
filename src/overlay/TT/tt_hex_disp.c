/* Address: 0x80148B5C, 152 bytes (38 instructions) */
/* @SymbolSize: 152 */
/* @PostProc: patch_stack_complete --old 80 --new 72 */
/* @PostProc: remap_stack_slots --from 56 --to 48 */
/* @PostProc: patch_insn_reg_field 0x44:rt:v1:v0 0x4c:rs:v1:v0 0x4c:rt:v1:v0 0x50:rs:v1:v0 0x54:rt:v1:v0 */
#include "types.h"

extern s32 tt_str_disp(s16 arg0, s16 arg1, char *buf);

void tt_hex_disp(s16 arg0, s16 arg1, u32 value, s32 ndigits)
{
    char buf[24];
    char *p;
    u8 *ndp;
    u32 nd;
    s32 v0;
    s32 sh;
    s32 v1;
    s32 t2;

    ndp = (u8 *)&ndigits;
    nd = *ndp;
    if (nd == 0 || nd > 8U) {
        *ndp = 8;
    }
    v0 = *ndp;
    p = &buf[0];
    v0 = v0 - 1;
    if (v0 >= 0) {
        sh = v0 << 2;
        do {
            v1 = (s32)(value >> sh);
            t2 = v1 & 0xF;
            if (t2 < 10U) {
                *p = (char)(t2 + 48);
            } else {
                v1 = t2;
                *p = (char)(v1 + 55);
            }
            sh = sh - 4;
            p++;
        } while (sh >= 0);
    }
    *p = 0;
    tt_str_disp(arg0, arg1, &buf[0]);
}
