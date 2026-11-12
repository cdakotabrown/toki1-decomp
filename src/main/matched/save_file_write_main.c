/* Function: save_file_write_main */
/* Address: 0x8005CEAC  -> window now starts at 0x8005CEA4 */
/* Size: 768 bytes (192 instructions)  -> window 776 bytes / 194 insns */
/* @SymbolStart: 0x8005CEA4 */
/* @SymbolSize: 776 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_instructions --offset1 0x290 --offset2 0x29c */
/* @PostProc: swap_instructions --offset1 0x290 --offset2 0x2a0 */
/* @PostProc: swap_instructions --offset1 0x290 --offset2 0x298 */
/* @PostProc: swap_instructions --offset1 0x290 --offset2 0x294 */
/* @PostProc: patch_insn_reg_field --patch 0x0:rt:a0:v1 --patch 0x4:rs:a0:v1 --patch 0x4:rt:a0:v1 --patch 0xc:rs:a0:v1 --patch 0x18:rt:a0:v1 --patch 0xd8:rt:a0:v1 --patch 0xdc:rs:a0:v1 --patch 0xdc:rt:a0:v1 --patch 0xe4:rs:a0:v1 --patch 0xf8:rs:a0:v1 --patch 0x128:rt:v0:v1 --patch 0x130:rt:v0:v1 --patch 0x148:rs:v0:v1 --patch 0x14c:rt:v1:t2 --patch 0x150:rs:v1:t2 --patch 0x150:rt:v1:t2 --patch 0x158:rt:v1:t2 --patch 0x188:rt:t2:t3 --patch 0x18c:rs:t2:t3 --patch 0x18c:rt:t2:t3 --patch 0x194:rs:t2:t3 --patch 0x1d4:rt:a0:v1 --patch 0x1d8:rs:a0:v1 --patch 0x1d8:rt:a0:v1 --patch 0x1e0:rs:a0:v1 --patch 0x1e0:rt:t3:t4 --patch 0x1e8:rt:t3:t4 --patch 0x1fc:rt:t6:t8 --patch 0x200:rt:v1:t6 --patch 0x204:rs:v1:t6 --patch 0x204:rt:v1:t6 --patch 0x208:rt:a0:v1 --patch 0x210:rs:a0:v1 --patch 0x210:rt:a0:v1 --patch 0x214:rt:t4:t5 --patch 0x218:rt:v1:t6 --patch 0x21c:rt:t4:t5 --patch 0x224:rs:a0:v1 --patch 0x224:rt:t5:t7 --patch 0x22c:rt:t5:t7 --patch 0x238:rs:t6:t8 --patch 0x238:rt:t6:t8 --patch 0x23c:rt:t7:t9 --patch 0x240:rs:t6:t8 --patch 0x24c:rs:t7:t9 --patch 0x24c:rt:t7:t9 --patch 0x254:rs:t7:t9 --patch 0x274:rt:t8:t0 --patch 0x278:rs:t8:t0 --patch 0x278:rt:t8:t0 --patch 0x280:rs:t8:t0 --patch 0x290:rt:t9:t1 --patch 0x298:rt:t9:t1 --patch 0x29c:rt:v1:t2 --patch 0x2a0:rs:v1:t2 --patch 0x2a0:rt:v1:t2 --patch 0x2a8:rt:v1:t2 */
#include "types.h"

extern void k_sub_reset(void);
extern s32 set_kanji_string(s32 x, s32 y, s32 arg2, void *arg3, s32 arg4);
extern void *get_pad_str(void);
extern void *get_pad_str2(void);
extern void k_sub_disp_start(s32 arg0);
extern void goto_sub_sub_step(s32 arg0);
extern void card_file_delete(s32 arg0);
extern s32 card_slot_write_main(void);
extern void card_state_clear(void);
extern void card_step_init(void);
extern void save_state_set(void);
extern void cursor_work_copy(s32 arg0);
extern void goto_sub_step(s32 arg0);
extern void save_file_box_disp(void);

extern u8 save_file_write_main_str0[];
extern u8 save_format_main_str6[];
extern u8 save_format_main_str7[];
extern u8 save_file_write_main_str1[];
extern u8 save_file_write_main_str2[];
extern u8 save_file_write_main_str3[];
extern u8 step_mode;
extern s32 pad_trig;
extern u8 now_album;
extern u8 album_status[];
extern u8 card_sys_present;
extern u32 card_cnt;
extern u8 card_result;
extern u8 card_mode;
extern u8 last_album;

void save_file_write_main(void)
{
    u32 mode = step_mode;
    void *str;
    u8 charIdx;

    switch (mode) {
    case 0:
        k_sub_reset();
        set_kanji_string(-128, 50, 1, save_file_write_main_str0, 0);
        set_kanji_string(-128, 66, 0, save_format_main_str6, 0);
        str = get_pad_str();
        set_kanji_string(-44, 66, 1, str, 0);
        set_kanji_string(-128, 82, 0, save_format_main_str7, 0);
        str = get_pad_str2();
        set_kanji_string(-44, 82, 2, str, 0);
        k_sub_disp_start(5);
        step_mode++;
        break;

    case 1:
        if (pad_trig & 0x20) {
            step_mode++;
            break;
        }
        if (pad_trig & 0x40) {
            goto_sub_sub_step(3);
            charIdx = now_album;
            if (album_status[charIdx] == 255) {
                card_file_delete(charIdx);
                album_status[now_album] = 0;
            }
        }
        break;

    case 2:
        k_sub_reset();
        set_kanji_string(-128, 50, 1, save_file_write_main_str1, 0);
        if (card_sys_present != 0) {
            set_kanji_string(-128, 66, 1, save_file_write_main_str2, 0);
            set_kanji_string(-128, 82, 1, save_file_write_main_str3, 0);
        }
        k_sub_disp_start(3);
        step_mode++;
        break;

    case 3:
        if (card_slot_write_main() == 20) {
            charIdx = now_album;
            album_status[charIdx] = 1;
            step_mode++;
            card_state_clear();
        } else {
            if (card_cnt < 513) {
                if (card_result != 240) {
                    break;
                }
            }
            card_state_clear();
            goto_sub_sub_step(255);
        }
        break;

    case 4:
        if (card_sys_present != 0) {
            card_step_init();
            card_mode = 1;
            last_album = now_album;
            now_album = 0;
            save_state_set();
            cursor_work_copy(1);
            goto_sub_step(0);
            goto_sub_sub_step(2);
        } else {
            cursor_work_copy(1);
            goto_sub_step(0);
            goto_sub_sub_step(2);
        }
        break;
    }

    save_file_box_disp();
}
