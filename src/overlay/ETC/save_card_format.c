/* @SymbolStart: 0x80145544 */
/* @RodataBase: 0x8014D450 */
/* @SymbolSize: 944 */
/* @PostProc: swap_registers a0:v1 */
/* @PostProc: patch_insn_reg_field 0x54:rt:v1:a0 0x6c:rt:v1:a0 0x70:rt:t7:v1 0x74:rs:t7:v1 0x74:rt:t7:v1 0x7c:rs:t7:v1 0x7c:rt:t8:t7 0x84:rt:t8:t7 0x88:rt:t9:v0 0x8c:rs:t9:v0 0x8c:rt:t9:v0 0x90:rt:t1:t9 0x94:rs:t9:v0 0x94:rt:t0:t8 0x98:rs:t0:t8 0xa4:rt:t1:t9 0xb8:rt:v1:a0 0xd4:rt:v1:a0 0xf0:rt:v1:a0 0x110:rt:v1:a0 0x12c:rt:v1:a0 0x148:rt:v1:a0 0x14c:rt:t2:v1 0x150:rs:t2:v1 0x150:rt:t2:v1 0x158:rs:t2:v1 0x158:rt:t3:t0 0x160:rt:t3:t0 0x170:rt:t4:t1 0x174:rs:t4:t1 0x178:rt:t6:t2 0x18c:rt:t5:v1 0x198:rt:t5:v1 0x1a8:rd:v1:a0 0x1b4:rs:t6:t2 0x1b8:rt:t7:t3 0x1c4:rt:t7:t3 0x1d8:rt:v1:a0 0x1f4:rt:v1:a0 0x210:rt:v1:a0 0x22c:rt:v1:a0 0x248:rt:v1:a0 0x264:rt:v1:a0 0x268:rt:t8:v1 0x26c:rs:t8:v1 0x26c:rt:t8:v1 0x274:rs:t8:v1 0x274:rt:t9:t4 0x27c:rt:t9:t4 0x280:rt:t0:v0 0x284:rs:t0:v0 0x284:rt:t0:v0 0x28c:rs:t0:v0 0x28c:rt:t1:t5 0x290:rs:t1:t5 0x2a4:rd:v1:a0 0x2b4:rd:v1:a0 0x2c0:rt:t2:v1 0x2c4:rs:t2:v1 0x2c4:rt:t2:v1 0x2cc:rs:t2:v1 0x2cc:rt:t3:t6 0x2d4:rt:t3:t6 0x2e4:rd:v1:a0 0x2fc:rt:t4:t7 0x300:rt:t4:t7 0x318:rt:t5:v1 0x31c:rs:t5:v1 0x31c:rt:t5:v1 0x324:rs:t5:v1 0x324:rt:t6:t8 0x32c:rt:t6:t8 0x340:rt:t9:t0 0x34c:rt:t7:v1 0x350:rs:t7:v1 0x350:rt:t7:v1 0x358:rs:t7:v1 0x358:rt:t8:t9 0x360:rt:t8:t9 0x364:rs:t9:t0 0x364:rt:v0:t0 0x368:rt:t0:t1 0x36c:rs:v0:t0 0x378:rs:t0:t1 0x378:rt:t0:t1 0x380:rs:t0:t1 0x394:rd:v1:a0 0x3ac:rd:v1:a0 */
/* Address: 0x8014554C */
/* Size: 936 bytes (234 instructions) */
#include "types.h"

extern void goto_sub_sub_step(s32);
extern void text_sys_init(void);
extern void set_kanji_string(s32, s32, s32, s32, s32);
extern void k_disp_start(s32);
extern void card_step_init(void);
extern s32 card_state_clear(void);
extern s32 card_format_run(void);
extern s32 card_file_write_main(void);
extern s32 card_file_make(s32);
extern void save_state_set(void);
extern s32 get_etc_yes_button(void);
extern s32 get_etc_no_button(void);

extern volatile s32 pad_trig;
extern volatile u8 step_mode;
extern u8 card_result;
extern volatile s32 card_cnt;
extern volatile u8 now_album;
extern volatile u8 album_status;
extern volatile u8 last_album;

extern u8 save_card_format_str0[];
extern u8 save_card_format_str1[];
extern u8 save_card_format_str2[];
extern u8 save_card_format_str3[];
extern u8 save_card_format_str4[];
extern u8 save_card_format_str5[];
extern u8 etc_yes_str[];
extern u8 etc_no_str[];

void save_card_format(void)
{
    s32 v1 = step_mode;

    s32 v0;
    u8 *pv;

    switch ((u32)v1) {
    case 0:
        text_sys_init();
        set_kanji_string(-128, -60, 15, (s32)save_card_format_str0, 0);
        k_disp_start(1);
        step_mode = step_mode + 1;
        return;

    case 1:
        if (pad_trig & 0x0860) {
            step_mode = v1 + 1;
        }
        return;

    case 2:
        text_sys_init();
        set_kanji_string(-128, -60, 15, (s32)save_card_format_str1, 0);
        set_kanji_string(-144, -18, 0, (s32)etc_yes_str, 0);
        v0 = get_etc_yes_button();
        set_kanji_string(-60, -18, 1, v0, 0);
        set_kanji_string(-144, -4, 0, (s32)etc_no_str, 0);
        v0 = get_etc_no_button();
        set_kanji_string(-60, -4, 2, v0, 0);
        k_disp_start(5);
        step_mode = step_mode + 1;
        return;

    case 3:
        {
            s32 flags = pad_trig;

            if (flags & 0x20) {
                if (card_format_run() == 1) {
                    step_mode = 16;
                    return;
                }
                card_step_init();
                goto_sub_sub_step(0);
                return;
            }
            if (flags & 0x40) {
                step_mode = v1 + 1;
            }
        }
        return;

    case 4:
        text_sys_init();
        set_kanji_string(-128, -60, 15, (s32)save_card_format_str2, 0);
        set_kanji_string(-128, -46, 1, (s32)save_card_format_str3, 0);
        set_kanji_string(-128, -46, 15, (s32)save_card_format_str4, 0);
        set_kanji_string(-128, -32, 15, (s32)save_card_format_str5, 0);
        v0 = get_etc_yes_button();
        set_kanji_string(-128, -32, 1, v0, 0);
        k_disp_start(5);
        step_mode = step_mode + 1;
        return;

    case 5:
        if (pad_trig & 0x20) {
            card_step_init();
            goto_sub_sub_step(0);
        }
        return;

    case 16:
        v0 = card_file_make(0);
        if (v0 == 1) {
            step_mode = step_mode + 1;
            return;
        }
        card_step_init();
        goto_sub_sub_step(0);
        return;

    case 17:
        last_album = 0;
        album_status = 1;
        now_album = 0;
        card_step_init();
        save_state_set();
        step_mode = step_mode + 1;
        return;

    case 18:
        v0 = card_file_write_main();
        pv = (u8 *)0x800F0000;
        if (v0 == 19) {
            card_state_clear();
            step_mode = step_mode + 1;
            return;
        }
        {
            s32 remaining = *(s32 *)(pv - 1920);
            if ((u32)remaining < 513U) {
                if (card_result != 240) {
                    return;
                }
            }
        }
        card_step_init();
        goto_sub_sub_step(0);
        return;

    case 19:
        card_step_init();
        goto_sub_sub_step(0);
        return;

    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    default:
        return;
    }
}
