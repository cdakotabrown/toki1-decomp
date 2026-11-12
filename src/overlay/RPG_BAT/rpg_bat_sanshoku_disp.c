/* @SymbolSize: 516 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=47 */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --map '28:40' */
/* @PostProc: scheduler_normalizer --block-aware --const-fp --store-fp --load-fp --fence 110-128 */
/* @PostProc: patch_insn_reg_field 0x1c:rt:t7:t5 0x20:rt:t8:t6 0x2c:rt:t5:t4 0x34:rt:t5:t9 0x40:rs:t5:t9 0x4c:rs:t7:t5 0x4c:rt:t6:t7 0x50:rs:t8:t6 0x50:rt:t9:t8 0x54:rt:t5:t4 0x58:rt:t9:t8 0x5c:rt:t6:t7 0x88:rt:v1:a0 0x8c:rt:a1:t2 0x90:rt:a2:a1 0x94:rd:t0:t9 0x94:rt:v1:a0 0x98:rd:a3:t3 0x98:rt:v1:a0 0x9c:rd:t2:t4 0x9c:rt:a1:t2 0xa0:rd:t1:t5 0xa0:rt:a1:t2 0xa4:rs:a2:a1 0xa4:rt:a3:t3 0xa8:rs:a2:a1 0xa8:rt:t0:t9 0xac:rs:a2:a1 0xac:rt:v1:a0 0xb0:rs:a2:a1 0xb0:rt:a3:t3 0xb4:rs:a2:a1 0xb4:rt:t0:t9 0xb8:rs:a2:a1 0xb8:rt:v1:a0 0xbc:rs:a2:a1 0xbc:rt:t1:t5 0xc0:rs:a2:a1 0xc0:rt:t2:t4 0xc4:rs:a2:a1 0xc4:rt:a1:t2 0xc8:rs:a2:a1 0xc8:rt:t2:t4 0xcc:rt:t3:v1 0xd4:rs:a2:a1 0xd4:rt:a1:t2 0xd8:rs:a2:a1 0xd8:rt:t1:t5 0xf4:rt:t7:t4 0xf8:rd:t5:t3 0xf8:rt:t3:v1 0xfc:rs:a2:a1 0xfc:rt:t5:t3 0x100:rs:t7:t4 0x100:rt:t7:t4 0x104:rt:t5:t8 0x108:rd:t6:t5 0x108:rt:t7:t4 0x110:rs:t8:t7 0x110:rt:t9:t6 0x114:rt:t6:t3 0x118:rd:t7:t9 0x118:rs:t9:t6 0x118:rt:t5:t8 0x11c:rs:a2:a1 0x11c:rt:t7:t9 0x120:rs:t6:t3 0x120:rt:t6:t3 0x128:rd:t8:t4 0x128:rt:t6:t3 0x12c:rd:t9:t5 0x12c:rt:t8:t4 0x138:rd:t7:t6 0x138:rs:t5:t7 0x138:rt:t3:v1 0x13c:rs:a2:a1 0x13c:rt:t7:t6 0x144:rt:t7:t5 0x14c:rd:t5:t3 0x150:rs:t9:t3 0x150:rt:t5:t4 0x158:rd:t6:t7 0x158:rs:t5:t4 0x158:rt:t7:t5 0x15c:rs:a2:a1 0x15c:rt:t6:t7 0x164:rt:t4:v1 0x170:rt:t5:t3 0x174:rt:t8:t5 0x178:rd:t6:t4 0x178:rs:t7:t3 0x178:rt:t4:v1 0x17c:rs:a2:a1 0x17c:rt:t6:t4 0x180:rs:t8:t5 0x180:rt:t8:t5 0x184:rt:t6:t9 0x188:rd:t9:t7 0x188:rt:t8:t5 0x18c:rd:t5:t6 0x18c:rt:t9:t7 0x190:rs:t5:t6 0x190:rt:t7:t8 0x194:rt:t9:t4 0x198:rd:t8:t3 0x198:rs:t7:t8 0x198:rt:t6:t9 0x19c:rs:a2:a1 0x19c:rt:t8:t3 0x1a0:rs:t9:t4 0x1a0:rt:t9:t4 0x1a8:rt:t9:t4 0x1ac:rd:t8:t7 0x1ac:rs:t6:v0 0x1ac:rt:t4:t5 0x1b8:rs:v0:t6 0x1b8:rt:t6:v1 0x1bc:rs:a2:a1 0x1c4:rt:t8:t7 0x1c8:rd:t5:t3 0x1cc:rd:t7:t4 0x1cc:rt:t5:t3 0x1d0:rs:t7:t4 0x1d0:rt:t6:t5 0x1d4:rt:t5:t8 0x1d8:rd:t9:t6 0x1d8:rs:t6:t5 0x1d8:rt:t8:t7 0x1dc:rs:a2:a1 0x1dc:rt:t9:t6 0x1e0:rt:t6:t3 0x1e4:rs:t5:t8 0x1e4:rt:t5:t8 0x1e8:rt:t5:t7 0x1ec:rd:t8:t4 0x1ec:rt:t6:t3 0x1f0:rd:t7:t9 0x1f0:rt:t5:t8 0x1f4:rd:t9:t5 0x1f4:rs:t7:t9 0x1f4:rt:t8:t4 0x1f8:rs:t5:t7 0x1f8:rt:t5:t7 0x200:rs:t9:t5 0x200:rt:t5:t7 */
/* rpg_bat_sanshoku_disp - RPG_BAT overlay, Address: 0x80155868, Size: 516 bytes */
#include "types.h"

extern s32 prim_alloc(s32 a0, s32 a1);
extern void SetPolyG4(s32 a0);
extern void SetSemiTrans(s32 a0, s32 a1);
extern void AddPrim(s32 a0);

extern volatile s32 activeBuff;
extern u8 screen_w[];
extern u8 ot_buff0[];

void rpg_bat_sanshoku_disp(s32 xa, s32 ya, s32 yb, s32 xb, s32 yc, s32 yd,
                   s32 c0, s32 c1, s32 col)
{
    s32 slot;
    s32 r0;
    s32 g0;
    s32 r1;
    s32 g1;
    s32 xr;
    s32 xs;
    u8 *p;

    xa = 47 - xa;
    ya = ya - 31;
    yb = yb - 31;
    yc = yc - 31;
    yd = yd - 31;
    xb = 47 - xb;

    slot = prim_alloc(36, activeBuff);
    SetPolyG4(slot);
    SetSemiTrans(slot, 1);

    p = (u8 *)slot;
    r0 = c0 >> 16;
    g0 = c0 >> 8;
    r1 = c1 >> 16;
    g1 = c1 >> 8;
    p[5] = g0;
    p[4] = r0;
    p[6] = c0;
    p[13] = g0;
    p[12] = r0;
    p[14] = c0;
    p[21] = g1;
    p[20] = r1;
    p[22] = c1;
    p[28] = r1;
    xr = xa;
    xa = 0;
    p[30] = c1;
    p[29] = g1;

    *(s16 *)(p + 8) = *(s16 *)&screen_w[activeBuff * 2 + 20] + xr;
    *(s16 *)(p + 10) = *(s16 *)&screen_w[activeBuff * 2 + 24] + ya;
    *(s16 *)(p + 16) = *(s16 *)&screen_w[activeBuff * 2 + 20] + xr;
    *(s16 *)(p + 18) = *(s16 *)&screen_w[activeBuff * 2 + 24] + yb;
    xs = xb;
    xb = 0;
    *(s16 *)(p + 24) = *(s16 *)&screen_w[activeBuff * 2 + 20] + xs;
    *(s16 *)(p + 26) = *(s16 *)&screen_w[activeBuff * 2 + 24] + yc;
    *(s16 *)(p + 32) = *(s16 *)&screen_w[activeBuff * 2 + 20] + xs;
    *(s16 *)(p + 34) = *(s16 *)&screen_w[activeBuff * 2 + 24] + yd;

    AddPrim((s32)&ot_buff0[(activeBuff << 10) + (col << 2)]);
}
