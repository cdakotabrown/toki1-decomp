/* @CompileFlags: -Wo,-loopunroll,0 */
/* Address: 0x801474B4 */
/* Size: 520 bytes (130 insns) */
/* @PostProc: swap_instructions --offset1 0x9C --offset2 0xB0 */
/* @PostProc: swap_instructions --offset1 0xA8 --offset2 0xB4 */
/* @PostProc: swap_instructions --offset1 0xAC --offset2 0xB0 */
/* @PostProc: patch_insn_reg_field 0xac:rd:a0:a1 0xb4:rd:a0:a1 0xb4:rs:a0:a1 0xb8:rd:a0:a1 0xb8:rt:a0:a1 0xbc:rd:a0:a1 0xbc:rs:a0:a1 0xc8:rd:a1:a0 0xd0:rs:a0:a1 0xd0:rt:a0:a1 0x114:rt:a0:a1 0x11c:rs:a1:a0 0x140:rt:a0:a1 0x148:rs:a1:a0 0x164:rt:a0:a1 0x16c:rs:a1:a0 */
#include "types.h"

typedef struct {
    u8  pad00[24];
    s16 f24;
    u8  pad26[42];
} Rec68;

typedef struct {
    u8  f00;
    u8  pad01[3];
    s32 f04;
    s32 f08;
    u8  f12;
    u8  f13;
    u8  pad14[14];
} Rec28;

extern Rec68 class_w[];
extern Rec28 taiiku_spoon_cpu[];
extern Rec28 taiiku_spoon_bal[];
extern u16 taiiku_spoon_pose_n[];
extern u16 taiiku_spoon_pose_r[];
extern u16 taiiku_spoon_pose_l[];

extern void game_reset(void);

void taiiku_runner_anim(void)
{
    s32 i;
    s32 v;
    s32 n;
    s32 k;
    Rec28 *p;

    for (i = 0; i < 3; i++) {
        v = taiiku_spoon_bal[i + 1].f12;
        switch ((u32)v) {
        case 0:
            p = &taiiku_spoon_cpu[i];
            if (p->f04 >= 6U) {
                k = i * 19 + 19;
                if (p->f00 >= 7U) {
                    p->f00 = 2;
                    p->f04 = 0;
                } else {
                    p->f00 = (u8)(p->f00 + 1);
                    p->f04 = 0;
                }
                n = taiiku_spoon_bal[i + 1].f08;
                if (n > 0) {
                    class_w[97 + i].f24 =
                        (s16)(taiiku_spoon_pose_r[p->f00] + k);
                } else if (n < 0) {
                    class_w[97 + i].f24 =
                        (s16)(taiiku_spoon_pose_l[p->f00] + k);
                } else {
                    class_w[97 + i].f24 =
                        (s16)(taiiku_spoon_pose_n[p->f00] + k);
                }
            }
            break;
        case 1:
            if (taiiku_spoon_bal[i + 1].f13 < 50U) {
                class_w[97 + i].f24 = (s16)(i * 2 + 83);
            }
            break;
        case 2:
            if (taiiku_spoon_bal[i + 1].f13 < 50U) {
                class_w[97 + i].f24 = (s16)(i * 2 + 84);
            }
            break;
        default:
            game_reset();
            break;
        }
    }
}
