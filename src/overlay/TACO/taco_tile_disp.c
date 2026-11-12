/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: patch_insn_reg_field 0x4c:rt:v1:t0 0x50:rt:a2:a1 0x54:rd:t6:t7 0x54:rt:v1:t0 0x58:rd:t7:t9 0x58:rt:v1:t0 0x5c:rt:t8:t1 0x60:rs:a2:a1 0x60:rt:t7:t9 0x64:rs:a2:a1 0x64:rt:t6:t7 0x68:rs:a2:a1 0x68:rt:v1:t0 0x6c:rs:t8:t1 0x6c:rt:t8:t1 0x78:rd:t9:t2 0x78:rt:t8:t1 0x7c:rd:t0:t3 0x7c:rs:t9:v0 0x7c:rt:v0:t2 0x80:rs:t0:t3 0x80:rt:t1:t4 0x84:rt:t2:t5 0x88:rt:t4:t7 0x8c:rd:t3:t6 0x8c:rs:t1:t4 0x8c:rt:t2:t5 0x90:rs:a2:a1 0x90:rt:t3:t6 0x94:rs:t4:t7 0x94:rt:t4:t7 0x98:rt:t8:t1 0x9c:rd:t5:t8 0x9c:rt:t4:t7 0xa0:rd:t6:t9 0xa0:rs:t5:v0 0xa0:rt:v0:t8 0xa4:rs:t6:t9 0xa4:rt:t7:t0 0xa8:rt:t0:t3 0xac:rd:t9:t2 0xac:rs:t7:t0 0xac:rt:t8:t1 0xb0:rs:a2:a1 0xb0:rt:t9:t2 0xb4:rs:t0:t3 0xb4:rt:t0:t3 0xb8:rt:t3:t6 0xbc:rd:t1:t4 0xbc:rt:t0:t3 0xc0:rs:t1:t4 0xc0:rt:t2:t5 0xc4:rs:t3:t6 0xc4:rt:t3:t6 0xcc:rs:t2:t5 0xcc:rt:t3:t6 */
/* @SymbolSize: 208 */
/* Address: 0x80143454 */
/* Size: 208 bytes (52 instructions) */
#include "types.h"

extern s32 prim_alloc();
extern void SetTile1();
extern void SetSemiTrans();
extern void AddPrim();
extern volatile s32 activeBuff;
extern s16 screen_w[];
extern u8 ot_buff0[];

void taco_tile_disp(s32 a0arg, s32 a1arg, s32 a2arg)
{
    u8 *p;
    s16 *base;

    p = (u8 *)prim_alloc(12, activeBuff);
    SetTile1(p);
    SetSemiTrans(p, 0);
    p[4] = (u8)(a2arg >> 16);
    p[5] = (u8)(a2arg >> 8);
    p[6] = (u8)a2arg;
    base = screen_w;
    *(s16 *)(p + 8) = *(s16 *)((u8 *)(base + activeBuff) + 20) + a0arg;
    *(s16 *)(p + 10) = *(s16 *)((u8 *)(base + activeBuff) + 24) + a1arg;
    AddPrim(&ot_buff0[activeBuff * 1024 + 60]);
}
