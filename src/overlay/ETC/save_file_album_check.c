/* @SymbolStart: 0x80144C50 */
/* Address: 0x80144C58 */
/* Size: 292 bytes (73 instructions) */
/* @SymbolSize: 300 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x30:rs:v0:v1 0x48:rt:t6:v1 0x4c:rs:t6:v1 0x4c:rt:t6:v1 0x54:rs:t6:v1 0x54:rt:t7:t6 0x5c:rt:t7:t6 0x70:rt:t0:t8 0x7c:rt:t8:v1 0x80:rs:t8:v1 0x80:rt:t8:v1 0x88:rs:t8:v1 0x88:rt:t9:t7 0x90:rt:t9:t7 0x94:rs:t0:t8 0x94:rt:t0:t8 0x98:rt:t1:t9 0x9c:rs:t0:t8 0xa8:rs:t1:t9 0xa8:rt:t1:t9 0xb0:rs:t1:t9 0xe0:rt:t2:t0 0x108:rt:t3:t1 0x10c:rs:t3:t1 0x10c:rt:t3:t1 0x114:rt:t3:t1 0x118:rt:t2:t0 */
#include "types.h"

extern void card_step_init(void);
extern s32  card_file_read_main2(void);
extern void card_state_clear(void);
extern void goto_sub_sub_step(s32);
extern s32  save_check(s32);
extern void album_info_disp(s32, s32);
extern void save_file_msg_disp(void);
extern void save_file_box_disp(void);

extern u8 step_mode;
extern s32 card_cnt;
extern u8 card_result;
extern u8 now_album;
extern u8 album_status[];

void save_file_album_check(void)
{
    s32 sel = step_mode;

    switch (sel) {
    case 0:
        card_step_init();
        step_mode++;
        break;
    case 1:
        if (card_file_read_main2() == 4) {
            card_state_clear();
            step_mode++;
            break;
        }
        if ((u32)card_cnt < 513) {
            if (card_result != 208) {
                break;
            }
        }
        card_state_clear();
        goto_sub_sub_step(255);
        break;
    case 2:
        if (save_check(0) == 1) {
            goto_sub_sub_step(2);
            album_info_disp(0, 0);
            save_file_msg_disp();
            break;
        }
        album_status[now_album] = 255;
        goto_sub_sub_step(255);
        break;
    }
    save_file_box_disp();
}
