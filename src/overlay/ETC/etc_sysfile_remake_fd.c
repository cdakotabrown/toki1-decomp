/* @SymbolStart: 0x80142560 */
/* @RodataBase: 0x8014D0D4 */
/* Address: 0x80142568 */
/* Size: 880 bytes (220 instructions) */
/* @SymbolSize: 888 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rt:v0:v1 0x8c:rt:v0:v1 0x90:rs:v0:v1 0x90:rt:v0:v1 0x98:rs:v0:v1 0xac:rs:v0:v1 0x144:rt:v0:v1 0x148:rs:v0:v1 0x148:rt:v0:v1 0x150:rs:v0:v1 0x210:rt:v0:v1 0x214:rs:v0:v1 0x214:rt:v0:v1 0x21c:rs:v0:v1 0x258:rt:v0:v1 0x25c:rs:v0:v1 0x25c:rt:v0:v1 0x264:rs:v0:v1 0x290:rt:v0:v1 0x294:rs:v0:v1 0x294:rt:v0:v1 0x29c:rs:v0:v1 0x2e0:rt:v0:v1 0x2e4:rs:v0:v1 0x2e4:rt:v0:v1 0x2ec:rs:v0:v1 0x314:rt:v0:v1 0x318:rs:v0:v1 0x318:rt:v0:v1 0x320:rs:v0:v1 */
#include "types.h"

extern void goto_step(s32 a0);
extern void goto_sub_sub_step(s32 a0);
extern void device_state_init(void);
extern void set_kanji_string(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void k_sub_reset(void);
extern void k_sub_disp_start(s32 a0);
extern void card_state_clear(void);
extern s32 card_file_write_main(void);
extern s32 card_file_make(s32 a0);
extern s32 card_file_delete(s32 a0);
extern void save_state_set(void);
extern void dec_bg_show(s32 a0);
extern s32 get_etc_yes_button(void);
extern s32 get_etc_no_button(void);
extern s32 pad_trig;
extern u8 step_mode;
extern u8 card_result;
extern u8 now_album;
extern u32 card_cnt;
extern s32 step_counter;
extern u8 album_status;
extern u8 last_album;

extern u8 etc_sysfile_remake_fd_str0[];
extern u8 etc_sysfile_remake_fd_str1[];
extern u8 etc_yes_str[];
extern u8 etc_no_str[];
extern u8 etc_sysfile_remake_fd_str2[];
extern u8 etc_sysfile_remake_fd_str3[];
extern u8 etc_sysfile_remake_fd_str4[];
extern u8 etc_sysfile_remake_fd_str5[];
extern u8 etc_sysfile_remake_fd_str6[];

void etc_sysfile_remake_fd(void)
{
    s32 v0;
    s32 r;

    switch ((u32)step_mode) {
    case 0:
        k_sub_reset();
        set_kanji_string(-120, 48, 0, (s32)etc_sysfile_remake_fd_str0, 0);
        set_kanji_string(-120, 76, 0, (s32)etc_sysfile_remake_fd_str1, 0);
        k_sub_disp_start(3);
        step_mode++;
        break;
    case 1:
        if ((u32)step_counter >= 65) {
            step_mode = step_mode + 1;
        }
        break;
    case 2:
        set_kanji_string(-136, 90, 0, (s32)etc_yes_str, 0);
        r = get_etc_yes_button();
        set_kanji_string(-52, 90, 1, r, 0);
        set_kanji_string(8, 90, 0, (s32)etc_no_str, 0);
        r = get_etc_no_button();
        set_kanji_string(92, 90, 2, r, 0);
        k_sub_disp_start(6);
        step_mode++;
        break;
    case 3:
        v0 = pad_trig;
        if (v0 & 0x20) {
            k_sub_reset();
            set_kanji_string(-120, 48, 0, (s32)etc_sysfile_remake_fd_str2, 0);
            set_kanji_string(-120, 62, 0, (s32)etc_sysfile_remake_fd_str3, 0);
            set_kanji_string(-120, 76, 0, (s32)etc_sysfile_remake_fd_str4, 0);
            set_kanji_string(-120, 90, 0, (s32)etc_sysfile_remake_fd_str5, 0);
            set_kanji_string(-120, 104, 0, (s32)etc_sysfile_remake_fd_str6, 0);
            k_sub_disp_start(5);
            step_mode++;
        } else if (v0 & 0x40) {
            device_state_init();
            goto_step(196);
        }
        break;
    case 4:
        if (card_file_delete(0) == 1) {
            step_mode++;
        } else {
            goto_sub_sub_step(255);
        }
        break;
    case 5:
        if (card_file_make(0) == 1) {
            step_mode++;
        } else {
            goto_sub_sub_step(255);
        }
        break;
    case 6:
        last_album = now_album;
        album_status = 1;
        now_album = 0;
        save_state_set();
        step_mode++;
        break;
    case 7:
        if (card_file_write_main() == 19) {
            card_state_clear();
            step_mode++;
        } else {
            if ((u32)card_cnt < 513 && card_result != 240) {
                break;
            }
            card_state_clear();
            goto_sub_sub_step(255);
        }
        break;
    case 8:
        goto_sub_sub_step(5);
        break;
    }

    dec_bg_show(2);
}
