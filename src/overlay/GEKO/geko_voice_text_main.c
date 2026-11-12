/* @SymbolStart: 0x801370CC */
/* @SymbolSize: 436 */
/* @PostProc: swap_instructions --offset1 0xd4 --offset2 0xdc */
/* @PostProc: patch_insn_reg_field 0x24:rt:a0:v0 0x28:rs:a0:v0 0x28:rt:a0:v0 0x30:rs:a0:v0 0x38:rs:a0:v0 0x40:rd:t9:t8 0x48:rt:t9:t8 0x50:rs:t9:t8 0x60:rt:a0:v0 0x64:rs:a0:v0 0x64:rt:a0:v0 0x6c:rs:a0:v0 0x90:rt:t0:t9 0x94:rs:t0:t9 0xa0:rt:t1:t0 0xa4:rs:t1:t0 0xa4:rt:t1:t0 0xa8:rd:t3:t2 0xac:rd:t2:t1 0xac:rt:t1:t0 0xb0:rd:t2:t1 0xb0:rs:t2:t1 0xb0:rt:t1:t0 0xb4:rd:t4:t3 0xb4:rs:t2:t1 0xb4:rt:t3:t2 0xbc:rt:t4:t3 0xcc:rt:t5:t4 0xd0:rs:t5:t4 0xd0:rt:t6:t5 0xd4:rs:t6:t5 0xf0:rt:t8:t7 0xf4:rs:t8:t7 0x140:rt:t9:t8 0x144:rs:t9:t8 0x144:rt:t9:t8 0x150:rd:t0:t9 0x150:rs:t9:t8 0x154:rs:t0:t9 0x19c:rt:t1:t0 0x1a0:rs:t1:t0 0x1a0:rt:t1:t0 0x1a8:rs:t1:t0 0x1a8:rt:t2:t1 0x1b0:rt:t2:t1 */
/* geko_voice_text_main -- GEKO overlay, Address: 0x801370D4, Size: 428 bytes */
#include "types.h"

extern void device_cmd_send(s32);
extern void k_reset(s32);
extern void set_kanji_string(s32, s32, s32, u8 *, s32);
extern s32 sub_sub_step_up(void);

extern u8 holiday_tel_exit_tbl0[];
extern u8 now_girl;
extern s32 pad_trig;
extern volatile s32 step_counter;
extern u8 geko_voice_text_main_str0[];
extern s32 geko_voice_done;
extern s16 geko_voice_cnt;

s32 geko_voice_text_main(void)
{
    s32 v;
    s32 h;
    s32 n;

    if (geko_voice_done == 0) {
        h = geko_voice_cnt;
        if (h < 90) {
            n = (s16)(h + 1);
            geko_voice_cnt = n;
            if (n == 1) {
                device_cmd_send(1546);
            }
            if (geko_voice_cnt == 30) {
                device_cmd_send(1540);
            }
            return 0;
        }
        v = step_counter;
        if ((v & 7) == 0 && (u32)v < 64U) {
            step_counter = v;
            device_cmd_send((u16)((u8)(holiday_tel_exit_tbl0[(now_girl * 7) +
                ((u32)v >> 3)] + 1285) | 0x600));
            v = step_counter;
        }
        v = v + 1;
        if ((v & 0x3F) == 0 && (u32)v < 320U) {
            step_counter = v;
            k_reset(1);
            set_kanji_string(-128, 48, 0, geko_voice_text_main_str0, 0);
            device_cmd_send(1544);
            v = step_counter;
        }
        if ((pad_trig & 0x400040) != 0 && (u32)v >= 64U) {
            step_counter = v;
            device_cmd_send(5384);
        } else {
            step_counter = v;
            if ((u32)v < 320U) {
                return 0;
            }
        }
        k_reset(1);
    }
    geko_voice_done = geko_voice_done + 1;
    return sub_sub_step_up();
}
