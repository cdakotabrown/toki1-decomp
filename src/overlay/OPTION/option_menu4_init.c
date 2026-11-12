/* @PostProc: patch_stack_complete --old 200 --new 216 */
/* @PostProc: remap_stack_slots --from 72 --to 80 --from 104 --to 112 --from 136 --to 144 --from 168 --to 176 */
/* Address: 0x8013BEEC, Size: 300 bytes (75 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void text_sys_init(void);
extern void set_kanji_string(s32, s32, s32, s32, s32);
extern void k_disp_start(s32);
extern void menu_set(s32, s32, s16 *, s16 *, s16 *, s16 *);
extern void cursor_init(s32);
extern u8 schedule_cursor;
extern u8 option_menu4_init_str0[];
extern u8 option_menu4_init_str1[];

void option_menu4_init(void)
{
    s16 arrD[16];
    s16 arrC[16];
    s16 arrB[16];
    s16 arrA[16];
    s32 i;

    cursor_init(0);
    for (i = 0; i < 16; i++) {
        arrD[i] = 32;
        arrC[i] = (s16)(i * 12 - 96);
        arrB[i] = 16;
        arrA[i] = 12;
    }
    menu_set(0, 16, arrD, arrC, arrB, arrA);
    text_sys_init();
    set_kanji_string(-128, -24, 15, (s32)option_menu4_init_str0, 0);
    set_kanji_string(-128, 0, 15, (s32)option_menu4_init_str1, 0);
    k_disp_start(2);
    sub_sub_step_up();
    schedule_cursor = 0;
}
