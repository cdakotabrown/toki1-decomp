/* @SymbolStart: 0x8013261C */
/* Address: 0x80132624 */
/* Size: 228 bytes (57 insns) */
/* @SymbolSize: 236 */
/* @PostProc: patch_insn_reg_field 0x60:rt:a1:v0 0x64:rs:t0:v0 0x64:rt:a1:t0 0x68:rt:v0:t1 0x6c:rs:v0:t1 0x6c:rt:v0:t1 0x74:rs:v0:t1 0x74:rt:v0:t2 0x78:rs:v0:t2 0x78:rt:a1:v0 0x7c:rt:v0:t2 0x88:rt:t1:t3 0x8c:rs:t1:t3 0x8c:rt:t1:t3 0x94:rs:t1:t3 0x98:rt:a1:v0 0x9c:rt:t2:t4 0xa0:rs:t2:t4 0xa0:rt:t2:t4 0xa8:rs:t2:t4 0xac:rt:v0:t5 0xb0:rs:v0:t5 0xb0:rt:v0:t5 0xb8:rs:v0:t5 0xb8:rt:v0:t6 0xbc:rs:v0:t6 0xbc:rt:a1:v0 0xc0:rt:v0:t6 0xcc:rt:t3:t7 0xd0:rs:t3:t7 0xd0:rt:t3:t7 0xd8:rs:t3:t7 0xd8:rt:t4:t8 0xdc:rs:t4:t8 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);

extern s16 cw98_frame;
extern s32 bunka_sd_kind;
extern s32 bunka_sd_pose_cnt;

extern s32 step_counter;
extern CLASS_W class_w[];

void bunka_sd_club0_banner_anim_wait(void) {
    s32 n;

    if (step_counter == 1) {
        bunka_sd_pose_cnt = 0;
        class_w[98].disp_flag = 132;
        class_w[98].anim_flag = 5;
    }

    if (bunka_sd_kind != 4 && cw98_frame == 20) {
        n = bunka_sd_pose_cnt + 1;
        bunka_sd_pose_cnt = n;
        if (n == 20) {
            device_cmd_send(1281);
        }
    }

    if (bunka_sd_kind == 4 && cw98_frame == 5) {
        n = bunka_sd_pose_cnt + 1;
        bunka_sd_pose_cnt = n;
        if (n == 20) {
            device_cmd_send(1281);
        }
    }

    if ((class_w[98].anim_flag & 1) == 0) {
        sub_sub_step_up();
    }
}
