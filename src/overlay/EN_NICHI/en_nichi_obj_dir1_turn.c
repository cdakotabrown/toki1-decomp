/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: remap_stack_slots --from 24 --to 40 */
/* @PostProc: patch_insn_reg_field 0x24:rd:a2:v1 0x28:rs:a2:v1 0x3c:rs:a2:v1 0x58:rs:a2:v1 0x64:rt:t9:t2 0x68:rs:a2:v1 0x68:rt:v0:t9 0x6c:rs:a2:v1 0x6c:rt:t9:t2 0x70:rt:v0:t9 0x78:rs:a2:v1 0x80:rt:a2:v1 0x84:rt:a2:v1 0x88:rt:t2:t3 0x8c:rs:t2:t3 0x90:rt:t4:t5 0x94:rt:t3:t4 0x9c:rs:a2:v1 0x9c:rt:t3:t4 0xa4:rs:a2:v1 0xa4:rt:t4:t5 0xb0:rt:t5:t9 0xb4:rs:a2:v1 0xb4:rt:v0:t6 0xb8:rs:a2:v1 0xb8:rt:t5:t9 0xbc:rd:t6:t7 0xbc:rt:v0:t6 0xc0:rs:t6:t7 0xc0:rt:t7:t8 0xc4:rs:a2:v1 0xc4:rt:t7:t8 0xcc:rt:a2:v1 0xd0:rt:a2:v1 0xd4:rt:t8:t0 0xd8:rs:t8:t0 0xdc:rt:t0:t2 0xe0:rt:t9:t1 0xe8:rs:a2:v1 0xe8:rt:t9:t1 0xec:rs:a2:v1 0xec:rt:t0:t2 0xf0:rs:a2:v1 0x100:rt:v1:t3 0x104:rs:a2:v1 0x104:rt:v1:t3 0x108:rs:v1:t3 0x110:rt:t1:t4 0x114:rs:a2:v1 0x114:rt:t1:t4 */
/* @SymbolStart: 0x801350A8 */
/* @SymbolSize: 280 */
/* en_nichi_obj_dir1_turn - EN_NICHI overlay, Address: 0x801350C4, Size: 252 bytes (63 insns) */
#include "types.h"

extern s32 rand();
extern s32 en_nichi_obj_wk[];

void en_nichi_obj_dir1_turn(s32 a0, s32 a1)
{
    s32 *v1;
    s32 v0;
    s32 ret;
    s32 t;

    v1 = (s32 *)((u8 *)en_nichi_obj_wk + a0 * 48);
    if (v1[10] >= 3) {
        return;
    }
    v0 = v1[1];
    switch (v0) {
    case 1:
        if (a1 < 900) {
            v1[11] = 128;
            t = v1[2];
            v1[10] = t * 2 + 4;
            ret = rand();
            if ((ret & 1) == 0) {
                v1[9] = 1;
            } else {
                v1[9] = 2;
            }
        }
        break;
    case 2:
        if (a1 < 2116) {
            v1[11] = 129;
            t = v1[2];
            v1[10] = t * 2 + 4;
            ret = rand();
            if ((ret & 1) == 0) {
                v1[9] = 1;
            } else {
                v1[9] = 2;
            }
        }
        break;
    }
    v0 = v1[7];
    if (v0 >= 4096) {
        t = v0 - 4096;
        v1[7] = t;
        v0 = t;
    }
    if (v0 < 0) {
        v1[7] = v0 + 4096;
    }
}
