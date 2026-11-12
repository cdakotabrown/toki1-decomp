/* @PostProc: patch_stack_complete --old 56 --new 72 */
/* @PostProc: remap_stack_slots --from 54 --to 66 */
/* @PostProc: remap_stack_slots --from 52 --to 64 */
/* @PostProc: swap_instructions --offset1 0x7C --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0x7C --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x7C --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x8C */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x94 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0xA0 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0xA8 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0xB0 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x98 */
/* @PostProc: swap_instructions --offset1 0x9C --offset2 0xA4 */
/* @PostProc: swap_instructions --offset1 0x9C --offset2 0xAC */
/* @PostProc: swap_instructions --offset1 0x9C --offset2 0xB4 */
/* @PostProc: swap_instructions --offset1 0x9C --offset2 0xB8 */
/* @PostProc: swap_instructions --offset1 0x9C --offset2 0xBC */
/* @PostProc: patch_insn_reg_field 0x58:rt:t6:t5 0x5c:rt:t7:t8 0x60:rs:t7:t8 0x60:rt:t7:t8 0x64:rs:t6:t5 0x64:rt:t6:t5 0x68:rt:t8:t6 0x6c:rd:t9:t7 0x6c:rt:t7:t8 0x70:rd:t8:t6 0x70:rs:t8:t6 0x70:rt:t6:t5 0x74:rs:t8:t6 0x74:rt:t8:t6 0x78:rd:t9:t7 0x78:rs:t9:t7 0x78:rt:t7:t8 0x7c:rt:t3:t2 0x80:rd:t9:t7 0x80:rt:t9:t7 0x84:rs:t3:t2 0x84:rt:t3:t2 0x88:rt:t2:t3 0x8c:rd:t1:t9 0x8c:rs:t8:t6 0x8c:rt:t9:t7 0x90:rd:t2:t3 0x90:rs:t2:t3 0x90:rt:t1:t9 0x94:rs:t2:t3 0x94:rt:t2:t3 0x98:rs:t3:t2 0x98:rt:t5:t4 0x9c:rt:t5:t4 0xa0:rt:t7:t5 0xa4:rt:t4:t1 0xa8:rs:t7:t5 0xa8:rt:t7:t5 0xac:rd:t6:t4 0xac:rt:t2:t3 0xb0:rs:t4:t1 0xb0:rt:t4:t1 0xb4:rs:t6:t4 0xb4:rt:t7:t5 0xbc:rt:t9:t6 0xc0:rs:v1:t2 0xc0:rt:t3:v1 0xc4:rs:t4:t1 0xd0:rt:t5:t7 0xd4:rt:t5:t7 0xec:rs:t4:t1 0xf8:rt:t9:t6 0xfc:rt:t6:t9 0x100:rt:t8:t4 0x104:rs:t8:t4 0x104:rt:t8:t4 0x108:rs:t6:t9 0x108:rt:t6:t9 0x10c:rt:t7:t3 0x110:rd:t9:t5 0x110:rt:t8:t4 0x114:rd:t7:t3 0x114:rs:t7:t3 0x114:rt:t6:t9 0x118:rs:t7:t3 0x118:rt:t7:t3 0x11c:rd:t9:t5 0x11c:rs:t9:t5 0x11c:rt:t8:t4 0x120:rd:t9:t5 0x120:rt:t9:t5 0x124:rt:t2:t6 0x128:rd:t5:t8 0x128:rs:t7:t3 0x128:rt:t9:t5 0x12c:rd:t2:t6 0x12c:rs:t2:t6 0x12c:rt:t5:t8 0x130:rs:t2:t6 0x130:rt:t2:t6 0x134:rt:t8:t9 0x13c:rt:t4:t1 0x140:rs:t8:t9 0x140:rt:t8:t9 0x144:rd:t6:t7 0x144:rt:t2:t6 0x148:rs:t4:t1 0x148:rt:t4:t1 0x158:rs:t6:t7 0x158:rt:t8:t9 0x15c:rt:t7:t4 0x160:rt:t9:t3 0x168:rs:t0:t1 0x168:rt:t4:t0 0x188:rt:t4:t1 0x18c:rt:t7:t4 0x194:rt:t9:t3 */
/* @SymbolStart: 0x801408EC */
/* @SymbolSize: 416 */
/* func_801408F8 -- GYOZI overlay, Address: 0x801408F8, Size: 404 bytes. */
#include "types.h"

extern void func_80142A8C(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2,
                          u32 w0, u32 w1, s32 arg8, s32 arg9);
extern void func_80054FCC(s32 arg0);

extern u8 D_800F53DE;
extern s16 D_8012B920;
extern s16 D_8012B92A;
extern s16 D_8012B92E;
extern u8 D_801483E8[];
extern u8 D_80148818;
extern u8 D_80148828[];
extern u32 D_8014882C[][2];

void func_801408F8(void)
{
    s16 ofs;
    s16 flag;
    s32 idx;
    s32 n;
    s32 cx;
    s32 cy;

    ofs = 28 - (D_8012B920 / 512) * 4;
    flag = 1;
    if (D_8012B920 == 4096) {
        ofs = 0;
        flag = 0;
    }

    n = D_800F53DE * 12 + D_80148828[D_80148818];
    cy = D_8012B92E;
    idx = D_801483E8[n];
    cx = D_8012B92A;
    func_80142A8C(cx - flag - 28, cy + ofs - 28, cx - 1, cy + ofs - 28,
                  cx - flag - 28, cy - 1,
                  D_8014882C[idx][0], D_8014882C[idx][1], 9, 0);

    idx = D_801483E8[D_800F53DE * 12 + D_80148828[D_80148818]];
    cx = D_8012B92A;
    cy = D_8012B92E;
    func_80142A8C(cx + flag + 28, cy - ofs + 28, cx + flag + 28, cy,
                  cx, cy - ofs + 28,
                  D_8014882C[idx][0], D_8014882C[idx][1], 9, 0);

    func_80054FCC(11);
}
