/* @SymbolStart: 0x80133204 */
/* Address: 0x8013320C */
/* Size: 812 bytes (203 insns) */
/* @SymbolSize: 820 */
/* @RodataBase: 0x80139FEC */
/* @PostProc: swap_instructions --offset1 0x1e4 --offset2 0x1ec */
/* @PostProc: swap_instructions --offset1 0x26c --offset2 0x274 */
/* @PostProc: patch_insn_reg_field 0x14c:rt:v0:v1 0x150:rs:v0:v1 0x17c:rs:s1:t6 0x17c:rt:t6:s1 0x1b0:rs:s1:t9 0x1b0:rt:t9:s1 0x1bc:rt:v0:v1 0x1c4:rs:v0:v1 0x1e4:rt:t6:t4 0x1ec:rt:t4:t5 0x1f0:rs:t4:t5 0x1f0:rt:t4:t5 0x1f4:rs:t6:t4 0x1f4:rt:t6:t4 0x1f8:rd:t5:t6 0x1f8:rt:t4:t5 0x1fc:rs:t5:t4 0x234:rt:v0:v1 0x23c:rt:v0:v1 0x24c:rs:v0:v1 0x264:rt:t5:t4 0x26c:rt:t4:t2 0x274:rt:t2:t3 0x278:rs:t2:t3 0x278:rt:t2:t3 0x27c:rs:t4:t2 0x27c:rt:t4:t2 0x280:rd:t3:t5 0x280:rt:t2:t3 0x284:rs:t3:t2 0x284:rt:t4:t5 0x2a8:rt:t5:t4 0x2b0:rt:t5:t4 0x2b4:rt:v0:v1 0x2bc:rt:v0:v1 */
#include "types.h"

extern void k_reset(s32);
extern s32 strlen(s32);
extern void set_kanji_string(s32, s32, s32, s32, s32);
extern void set_XAsector(s32, s32, s32);
extern void device_cmd_send(s32);
extern s32 get_CD_State(void);
extern s32 check_end_k(void);
extern void sub_sub_step_up(void);
extern void k_scroll_set(void);
extern void k_disp_inc(void);

extern s32 bunka_sd_kind;
extern u8  p_sei[];
extern u8  bunka_sd_rollcall_step_str0[];
extern u8  bunka_sd_rollcall_step_str1[];
extern u8  bunka_sd_rollcall_step_str2[];
extern u8  bunka_sd_rollcall_step_str3[];
extern u8  bunka_sd_rollcall_step_str4[];
extern s32 bunka_sd_line_top;
extern s32 bunka_sd_line_ptr;
extern s32 bunka_sd_xa_ptr;
extern u8  step_mode;
extern s32 pad_data;
extern u8  now_girl;
extern s32 step_counter;

void bunka_sd_rollcall_step(void)
{
    s32 w;
    s32 xoff;
    s32 row;
    s32 yval;
    s32 charId;
    s32 *p;

    if (step_counter == 1) {
        k_reset(1);

        switch ((u32)bunka_sd_kind) {
        case 0:
            w = strlen((s32)p_sei);
            xoff = (s16)(-104 - w * 4);
            set_kanji_string(xoff, 48, 0, (s32)p_sei, 0);
            break;
        case 1:
            set_kanji_string(-128, 48, 0, (s32)bunka_sd_rollcall_step_str0, 0);
            break;
        case 2:
            set_kanji_string(-120, 48, 0, (s32)bunka_sd_rollcall_step_str1, 0);
            break;
        case 3:
            set_kanji_string(-120, 48, 0, (s32)bunka_sd_rollcall_step_str2, 0);
            break;
        default:
        case 4:
            set_kanji_string(-128, 48, 0, (s32)bunka_sd_rollcall_step_str3, 0);
            break;
        }
        set_kanji_string(-86, 48, 0, (s32)bunka_sd_rollcall_step_str4, 0);

        for (row = bunka_sd_line_top; row < 100; row++) {
            yval = (s16)((row - bunka_sd_line_top) * 16 + 48);
            set_kanji_string(-72, yval, 0,
                          *(s32 *)(bunka_sd_line_ptr + row * 4) + 1, 0);

            charId = *(u8 *)*(s32 *)(bunka_sd_line_ptr + row * 4);
            if (charId == 103) {
                if ((pad_data & 0x20) == 0) {
                    if (bunka_sd_kind != 0) {
                        p = (s32 *)(bunka_sd_xa_ptr + step_mode * 4);
                        set_XAsector(p[0], p[1], now_girl);
                        device_cmd_send(768);
                    }
                }
                step_mode = step_mode + 2;
                bunka_sd_line_top = row + 1;
                row = 100;
            } else if (charId == 69) {
                if ((pad_data & 0x20) == 0) {
                    if (bunka_sd_kind != 0) {
                        p = (s32 *)(bunka_sd_xa_ptr + step_mode * 4);
                        set_XAsector(p[0], p[1], now_girl);
                        device_cmd_send(768);
                    }
                }
                step_mode = 255;
                bunka_sd_line_top = row + 1;
                row = 100;
            }
        }
    }

    if (get_CD_State() == 2) {
        if (check_end_k() != 0) {
            if (step_mode == 255) {
                k_reset(1);
                sub_sub_step_up();
            }
            step_counter = 0;
        }
    }
    k_scroll_set();
    k_disp_inc();
}
