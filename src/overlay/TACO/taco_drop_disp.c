/* @PostProc: patch_stack_complete --old 56 --new 72 */
/* @PostProc: remap_stack_slots --from 52 --to 60 --from 48 --to 56 --from 44 --to 52 --from 36 --to 48 */
/* @PostProc: swap_instructions --offset1 0x184 --offset2 0x190 */
/* @PostProc: swap_instructions --offset1 0x188 --offset2 0x190 */
/* @PostProc: swap_instructions --offset1 0x18c --offset2 0x190 */
/* @PostProc: patch_insn_reg_field 0x168:rs:v0:s1 0x168:rt:s1:v0 0x16c:rt:t5:t9 0x170:rs:t5:t9 0x170:rt:t5:t9 0x174:rt:t7:t5 0x178:rd:t6:t4 0x178:rt:t5:t9 0x17c:rd:t7:t5 0x17c:rs:t7:t5 0x17c:rt:t6:t4 0x180:rs:t7:t5 0x180:rt:t7:t5 0x18c:rd:t9:t6 0x190:rd:t4:t7 0x190:rt:t9:t6 0x194:rs:t4:t5 0x1ac:rs:v0:s1 0x1ac:rt:s1:v0 0x1b8:rt:t8:t5 0x1bc:rd:t7:t6 0x1c0:rd:t8:t5 0x1c0:rs:t8:t5 0x1c0:rt:t7:t6 0x1c4:rs:t8:t5 0x1c4:rt:t8:t5 0x1c8:rd:t5:t7 0x1cc:rd:t6:t8 0x1cc:rt:t5:t7 0x1d0:rs:t6:t5 0x1e4:rs:v0:s1 0x1e4:rt:s1:v0 0x1e8:rt:t4:t9 0x1ec:rs:t4:t9 0x1ec:rt:t4:t9 0x1f4:rd:t7:t4 0x1f4:rt:t4:t9 0x1f8:rt:t7:t4 0x200:rd:t9:t7 0x204:rt:t9:t7 0x208:rs:t5:t6 0x208:rt:t6:t5 0x234:rt:t6:t5 0x23c:rt:t5:t6 0x240:rs:t6:t5 0x240:rt:t6:t5 0x244:rs:t5:t6 0x244:rt:t6:t5 */
/* Address: 0x8013AD34 */
/* Size: 632 bytes (158 insns) */
#include "types.h"

extern s32 activeBuff;
extern s16 disp_ofs_x[];
extern s16 disp_ofs_y[];
extern s32 taco_drop_bright;
extern u8 ot_buff0[][1024];

extern u8 *prim_alloc();
extern void SetTile1();
extern void SetSemiTrans();
extern s32 rsin();
extern s32 rcos();
extern void AddPrim();

void taco_drop_disp(s32 obj, s32 dist, s32 rgb, s32 mode)
{
    s32 r;
    s32 g;
    s32 b;
    u8 *pk;
    s32 x;
    s32 y;

    r = (rgb >> 16) & 0xFF;
    g = (rgb >> 8) & 0xFF;
    b = rgb & 0xFF;

    if (mode < 2) {
        if (dist < 128) {
            r = r * dist / 128;
            g = g * dist / 128;
            b = b * dist / 128;
        }
        r = r * taco_drop_bright / 128;
        g = g * taco_drop_bright / 128;
        b = b * taco_drop_bright / 128;
    }

    pk = prim_alloc(12, activeBuff);
    SetTile1(pk);
    SetSemiTrans(pk, 0);
    pk[4] = (u8)r;
    pk[5] = (u8)g;
    pk[6] = (u8)b;

    x = disp_ofs_x[activeBuff] + ((rsin(obj) * dist) >> 12);

    if (mode == 0) {
        y = disp_ofs_y[activeBuff] + ((rcos(obj) * dist) >> 12);
        y = y - 40;
    } else {
        y = disp_ofs_y[activeBuff] + ((rcos(obj) * dist) >> 12);
    }

    *(s16 *)(pk + 8) = (s16)x;
    *(s16 *)(pk + 10) = (s16)y;

    if (mode < 2) {
        AddPrim(&ot_buff0[activeBuff][1016], pk);
    } else {
        AddPrim(&ot_buff0[activeBuff][64], pk);
    }
}
