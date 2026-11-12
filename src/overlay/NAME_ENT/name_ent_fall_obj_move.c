/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x50 */
/* @PostProc: patch_insn_reg_field 0x64:rt:v1:t6 0x6c:rs:v1:t6 0x6c:rt:v0:t7 0x70:rs:v0:t7 0x74:rd:t6:t8 0x78:rt:v0:t7 0x80:rs:t6:t8 0x84:rt:t7:t9 0x88:rd:t8:t0 0x8c:rd:t9:t1 0x8c:rs:t7:t9 0x8c:rt:t8:t0 0x94:rt:t9:t1 0x98:rt:t0:t2 0xa0:rs:t0:t2 0xa0:rt:t1:t3 0xa4:rt:t1:t3 0xa8:rt:t2:t4 0xb0:rs:t2:t4 0xb0:rt:t3:t5 0xb4:rt:t3:t5 0xb8:rt:t4:t6 0xc0:rs:t4:t6 0x110:rd:t5:t7 0x114:rs:t5:t7 0x114:rt:t6:t8 0x11c:rt:t6:t8 0x148:rd:t7:t9 0x14c:rd:t8:t0 0x14c:rt:t7:t9 0x150:rt:t8:t0 */
/* name_ent_fall_obj_move - NAME_ENT overlay, Address: 0x80145520 */
#include "types.h"
#include "tokimeki/class_w.h"

extern u8 class_w[];
extern u8 cw8_rec[];
extern s32 rsin(s32);
extern s32 rand(void);

void name_ent_fall_obj_move(s32 arg0)
{
    u8 *p;
    u8 c;

    p = class_w;
    do {
        c = *(p + 6535);
        *(p + 6535) = c + 1;
        *(s32 *)(p + 6564) +=
            rsin(((c + 1) & 0xFF) << 5) << 3;
        if (arg0 == 0) {
            *(s16 *)(p + 6566) -= 1;
        }
        *(s16 *)(p + 6570) += 1;
        if (*(s16 *)(p + 6570) >= 41) {
            if (arg0 == 1 && p >= (&((CLASS_W *)class_w)[4].active)) {
                *(p + 6531) = 0;
            }
            *(s16 *)(p + 6566) = (rand() % 320) - 160;
            *(s16 *)(p + 6570) = -120 - (rand() % 48);
        }
        p += 68;
    } while (p != cw8_rec);
}
