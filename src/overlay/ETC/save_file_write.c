/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x30:rs:v0:v1 0x34:rt:t0:t8 0x3c:rs:v0:v1 0xf8:rt:t2:v1 0xfc:rs:t2:v1 0xfc:rt:t2:v1 0x104:rs:t2:v1 0x104:rt:t3:t2 0x10c:rt:t3:t2 0x120:rt:t8:t6 0x124:rt:t5:t4 0x128:rs:t5:t4 0x128:rt:t5:t4 0x12c:rt:at:v1 0x130:rt:t6:at 0x134:rs:t6:v1 0x134:rt:t6:v1 0x138:rt:t4:t3 0x13c:rt:t5:t4 0x140:rt:t4:t3 0x148:rs:t6:v1 0x148:rt:t7:t5 0x150:rt:t7:t5 0x15c:rs:t8:t6 0x15c:rt:t8:t6 0x160:rt:t9:t7 0x164:rs:t8:t6 0x170:rs:t9:t7 0x170:rt:t9:t7 0x178:rs:t9:t7 0x1a0:rs:t0:t8 0x1a0:rt:t0:t8 0x1a8:rs:t0:t8 0x1b8:rt:t1:t9 0x1c0:rt:t1:t9 0x1c4:rt:t2:t0 0x1c8:rs:t2:t0 0x1c8:rt:t2:t0 0x1d0:rt:t2:t0 0x1e0:rt:t3:v1 0x1e4:rs:t3:v1 0x1e4:rt:t3:v1 0x1ec:rs:t3:v1 0x1ec:rt:t4:t1 0x1f4:rt:t4:t1 0x218:rt:t5:t2 0x234:rs:t5:t2 0x234:rt:t5:t2 0x238:rt:t6:t3 0x23c:rs:t5:t2 0x248:rs:t6:t3 0x248:rt:t6:t3 0x250:rs:t6:t3 */
/* @SymbolStart: 0x80144F00 */
/* @SymbolSize: 616 */
/* Address: 0x80144F08 (window starts 0x80144F00 - absorbed cross-prologue head) */
/* Size: 616 bytes (154 instructions, head included) */
#include "types.h"

extern void goto_sub_step(s32);
extern void goto_sub_sub_step(s32);
extern void set_kanji_string(s32, s32, s32, u8 *, s32);
extern void k_sub_reset(void);
extern void k_sub_disp_start(s32);
extern void card_step_init(void);
extern void card_state_clear(void);
extern s32 card_file_write_main(void);
extern s32 card_slot_write_main(void);
extern void save_state_set(void);
extern void save_file_box_disp(void);
extern void printf(u8 *);

extern u8 screen_w[];
extern u8 now_girl;
extern u8 card_result;
extern u8 now_album[];
extern u8 step_mode;
extern s32 card_cnt;
extern u8 album_work[];
extern u8 album_status[];
extern u8 last_album;
extern u8 card_sys_present;
extern u8 save_file_write_str0[];
extern u8 save_file_write_str1[];
extern u8 save_file_write_str2[];

void save_file_write(void)
{
    u8 *p;

    switch (step_mode) {
    case 0:
        k_sub_reset();
        p = screen_w;
        if (p[1340 + (p[3935] & 0xF)] < 99U) {
            p[1340 + (p[3935] & 0xF)] = p[1340 + (p[3935] & 0xF)] + 1;
        }
        if (album_work[20 + (p[3935] & 0xF)] < 99U) {
            album_work[20 + (p[3935] & 0xF)] =
                album_work[20 + (p[3935] & 0xF)] + 1;
        }
        set_kanji_string(-128, 50, 1, save_file_write_str0, 0);
        set_kanji_string(-128, 66, 1, save_file_write_str1, 0);
        k_sub_disp_start(2);
        step_mode = step_mode + 1;
        break;

    case 1:
        if (card_slot_write_main() == 20) {
            album_status[now_album[0]] = 1;
            step_mode = step_mode + 1;
            card_state_clear();
        } else if (card_cnt >= 513U || card_result == 240) {
            printf(save_file_write_str2);
            card_state_clear();
            goto_sub_sub_step(255);
        }
        break;

    case 2:
        if (card_sys_present != 0) {
            card_step_init();
            now_album[3] = 1;
            last_album = now_album[0];
            now_album[0] = 0;
            save_state_set();
            step_mode = step_mode + 1;
        } else {
            goto_sub_step(2);
        }
        break;

    case 3:
        if (card_file_write_main() == 19) {
            card_state_clear();
            goto_sub_step(2);
        } else if (card_cnt >= 513U || card_result == 240) {
            goto_sub_sub_step(255);
        }
        break;
    }

    save_file_box_disp();
}
