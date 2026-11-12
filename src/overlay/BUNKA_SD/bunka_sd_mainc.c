/* @SymbolStart: 0x801327E4 */
/* Address: 0x801327EC */
/* Size: 340 bytes (85 insns) */
/* @SymbolSize: 348 */
/* @PostProc: patch_insn_reg_field 0x54:rt:a0:v0 0x58:rs:a0:v0 0x58:rt:a0:v0 0x60:rs:a0:v0 0x68:rs:a0:v0 0x74:rt:v0:t1 0x80:rs:v0:t1 0x80:rt:v0:t1 0x88:rs:v0:t1 0x88:rt:v0:t2 0x8c:rt:v0:t2 0x94:rs:v0:t2 0xa4:rt:a0:v0 0xa8:rs:a0:v0 0xa8:rt:a0:v0 0xb4:rs:a0:v0 0xb8:rt:t1:t3 0xbc:rs:t1:t3 0xbc:rt:t1:t3 0xc4:rs:t1:t3 0xc8:rt:v0:t4 0xcc:rs:v0:t4 0xcc:rt:v0:t4 0xd4:rs:v0:t4 0xd4:rt:v0:t5 0xd8:rt:v0:t5 0xe0:rs:v0:t5 0xf0:rt:a0:v0 0xf4:rs:a0:v0 0xf4:rt:a0:v0 0x100:rs:a0:v0 0x104:rt:t2:t6 0x108:rs:t2:t6 0x108:rt:t2:t6 0x110:rs:t2:t6 0x114:rt:v0:t7 0x118:rs:v0:t7 0x118:rt:v0:t7 0x120:rs:v0:t7 0x120:rt:v0:t8 0x124:rt:v0:t8 0x12c:rs:v0:t8 0x13c:rt:t3:t9 0x140:rs:t3:t9 0x140:rt:t3:t9 0x148:rs:t3:t9 0x148:rt:t4:t0 0x14c:rs:t4:t0 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);

extern s32 bunka_sd_kind;
extern s32 bunka_sd_wait_cnt;

extern s32 step_counter;
extern CLASS_W class_w[];

void bunka_sd_mainc(void) {
    s32 v0;
    s32 n;

    if (step_counter == 1) {
        bunka_sd_wait_cnt = 0;
        class_w[98].disp_flag = 4;
        class_w[99].disp_flag = 132;
        class_w[99].anim_flag = 5;
    }

    v0 = bunka_sd_kind;
    if (v0 != 3 && v0 != 4) {
        if (class_w[99].frame == 0) {
            n = bunka_sd_wait_cnt + 1;
            bunka_sd_wait_cnt = n;
            if (n == 20) {
                device_cmd_send(1281);
            }
        }
    }

    v0 = bunka_sd_kind;
    if (v0 == 3 && class_w[99].frame == 11) {
        n = bunka_sd_wait_cnt + 1;
        bunka_sd_wait_cnt = n;
        if (n == 1) {
            device_cmd_send(1281);
        }
    }

    v0 = bunka_sd_kind;
    if (v0 == 4 && class_w[99].frame == 5) {
        n = bunka_sd_wait_cnt + 1;
        bunka_sd_wait_cnt = n;
        if (n == 1) {
            device_cmd_send(1281);
        }
    }

    if ((class_w[99].anim_flag & 1) == 0) {
        sub_sub_step_up();
    }
}
