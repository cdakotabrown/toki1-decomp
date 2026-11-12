/* Address: 0x8005CBE0 */
/* Size: 300 bytes (75 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v0 v1 */
/* @PostProc: patch_insn_reg_field --patch 0x38:rt:t6:v1 --patch 0x3c:rs:t6:v1 --patch 0x3c:rt:t6:v1 --patch 0x44:rs:t6:v1 --patch 0x44:rt:t7:t6 --patch 0x4c:rt:t7:t6 --patch 0x5c:rs:v1:v0 --patch 0x60:rt:t0:t8 --patch 0x6c:rt:t8:v1 --patch 0x70:rs:t8:v1 --patch 0x70:rt:t8:v1 --patch 0x78:rs:t8:v1 --patch 0x78:rt:t9:t7 --patch 0x80:rt:t9:t7 --patch 0x84:rs:t0:t8 --patch 0x84:rt:t0:t8 --patch 0x88:rt:t1:t9 --patch 0x8c:rs:t0:t8 --patch 0x98:rs:t1:t9 --patch 0x98:rt:t1:t9 --patch 0xa0:rs:t1:t9 --patch 0xcc:rs:v1:v0 --patch 0xd0:rt:t2:t0 --patch 0xf8:rt:t3:t1 --patch 0xfc:rs:t3:t1 --patch 0xfc:rt:t3:t1 --patch 0x104:rt:t3:t1 --patch 0x108:rt:t2:t0 */
/* (2026-08-29: a dead `@PostProc: strip_instructions` was removed here. */
#include "types.h"

extern u8 step_mode;
extern u32 card_cnt;
extern u8 card_result;
extern u8 now_album;
extern u8 album_status[];

extern void card_step_init(void);
extern s32 card_file_read_main2(void);
extern void card_state_clear(void);
extern s32 save_check(s32);
extern void goto_sub_sub_step(s32);
extern void album_info_disp(s32, s32);
extern void save_file_msg_disp(void);
extern void save_file_box_disp(void);

void save_file_read_main(void) {
    u8 mode = step_mode;

    switch (mode) {
    case 0:
        card_step_init();
        step_mode = step_mode + 1;
        break;
    case 1:
        if (card_file_read_main2() == 4) {
            card_state_clear();
            step_mode = step_mode + 1;
        } else {
            if (card_cnt >= 513) {
            } else if (card_result != 208) {
                break;
            }
            card_state_clear();
            goto_sub_sub_step(255);
        }
        break;
    case 2:
        if (save_check(0) == 1) {
            goto_sub_sub_step(4);
            album_info_disp(0, 0);
            save_file_msg_disp();
        } else {
            album_status[now_album] = 255;
            goto_sub_sub_step(255);
        }
        break;
    }
    save_file_box_disp();
}
