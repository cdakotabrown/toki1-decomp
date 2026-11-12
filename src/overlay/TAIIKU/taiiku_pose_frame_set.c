/* @SymbolStart: 0x80146BFC */
/* @SymbolSize: 300 */
/* @PostProc: patch_insn_reg_field 0x2c:rt:v1:v0 0x30:rs:v1:v0 0x30:rt:v1:v0 0x38:rs:v1:v0 0x38:rt:v0:v1 0x3c:rs:v0:v1 0x40:rs:v1:v0 0xa4:rs:v1:v0 0xb0:rs:v0:v1 */
/* Size: 292 + 8 hoisted = 300 bytes (75 insns). */
#include "types.h"

extern u8 taiiku_spoon_bal_0_down;
extern s32 pad_data;
extern volatile u16 taiiku_spoon_pl_se_tick;
extern s32 taiiku_spoon_scrv;
extern u8 taiiku_spoon_pl;
extern u16 taiiku_spoon_pose_r[];
extern u16 taiiku_spoon_pose_l[];
extern u16 taiiku_spoon_pose_n[];
extern s16 cal_cw_frame;

extern void device_cmd_send();
extern void taiiku_pose_advance();

void taiiku_pose_frame_set(void) {
    s32 v0;
    s32 v1;

    if (taiiku_spoon_bal_0_down != 0) {
        return;
    }
    taiiku_pose_advance();

    v0 = pad_data;
    v1 = v0 & 0x000A;
    if (v1 != 0 && (v0 & 0x0005) == 0) {
        if ((u32)taiiku_spoon_pl_se_tick >= 21U && taiiku_spoon_scrv > 0) {
            device_cmd_send(1287);
            taiiku_spoon_pl_se_tick = 0;
        }
        cal_cw_frame = (s16)taiiku_spoon_pose_r[taiiku_spoon_pl];
    } else if ((v0 & 0x0005) != 0 && v1 == 0) {
        if ((u32)taiiku_spoon_pl_se_tick >= 21U && taiiku_spoon_scrv > 0) {
            device_cmd_send(1288);
            taiiku_spoon_pl_se_tick = 0;
        }
        cal_cw_frame = (s16)taiiku_spoon_pose_l[taiiku_spoon_pl];
    } else {
        cal_cw_frame = (s16)taiiku_spoon_pose_n[taiiku_spoon_pl];
    }
    return;
}
