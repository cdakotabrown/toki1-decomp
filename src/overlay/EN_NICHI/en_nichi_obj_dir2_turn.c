/* @CompileFlags: -Wc,-pic0 */
/* Address: 0x801351DC (true symbol start 0x801351C0, cross-prologue). */
/* @SymbolStart: 0x801351C0 */
/* @SymbolSize: 280 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* @PostProc: patch_insn_reg_field 0x24:rd:a2:v1 0x28:rs:a2:v1 0x3c:rs:a2:v1 0x58:rs:a2:v1 0x64:rt:t9:t2 0x68:rs:a2:v1 0x68:rt:t0:t9 0x6c:rs:a2:v1 0x6c:rt:t9:t2 0x70:rd:t1:t0 0x70:rt:t0:t9 0x74:rs:t1:t0 0x74:rt:t2:t1 0x78:rs:a2:v1 0x78:rt:t2:t1 0x80:rt:a2:v1 0x84:rt:a2:v1 0x9c:rs:a2:v1 0xa4:rs:a2:v1 0xb0:rt:t6:t9 0xb4:rs:a2:v1 0xb4:rt:t7:t6 0xb8:rs:a2:v1 0xb8:rt:t6:t9 0xbc:rd:t8:t7 0xbc:rt:t7:t6 0xc0:rs:t8:t7 0xc0:rt:t9:t8 0xc4:rs:a2:v1 0xc4:rt:t9:t8 0xcc:rt:a2:v1 0xd0:rt:a2:v1 0xe8:rs:a2:v1 0xec:rs:a2:v1 0xf0:rs:a2:v1 0x100:rt:v1:t3 0x104:rs:a2:v1 0x104:rt:v1:t3 0x108:rs:v1:t3 0x110:rt:t3:t4 0x114:rs:a2:v1 0x114:rt:t3:t4 */
#include "types.h"

extern s32 rand();

extern u8 en_nichi_obj_wk[];

void en_nichi_obj_dir2_turn(s32 a0, s32 a1)
{
    s32 *v1;
    s32 v0;
    s32 t3;

    v1 = (s32 *)(en_nichi_obj_wk + a0 * 48);
    if (v1[10] < 3) {
        v0 = v1[1];
        switch (v0) {
        case 1:
            if (a1 < 900) {
                v1[11] = 64;
                v1[10] = (v1[2] << 1) + 4;
                if ((rand() & 1) == 0) {
                    v1[9] = 1;
                } else {
                    v1[9] = 2;
                }
            }
            break;
        case 2:
            if (a1 < 2116) {
                v1[11] = 65;
                v1[10] = (v1[2] << 1) + 4;
                if ((rand() & 1) == 0) {
                    v1[9] = 1;
                } else {
                    v1[9] = 2;
                }
            }
            break;
        }
        v0 = v1[7];
        if (v0 >= 4096) {
            t3 = v0 - 4096;
            v1[7] = t3;
            v0 = t3;
        }
        if (v0 < 0) {
            v1[7] = v0 + 4096;
        }
    }
}
