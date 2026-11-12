/* Function: func_800733A4 */
/* Address: 0x800733A4 */
/* Size: 220 bytes (55 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 step_mode;
extern s32 step_counter;
extern s32 pad_data;
extern u8 byouki_kind;
extern char byouki_noiroze_str0[];
extern char byouki_noiroze_str1[];
extern char p_sei[];

extern void text_sys_init(void);
extern void basyo_init2(char *);
extern void gnsx(char *);
extern void sndi(char *, s32, s32);
extern void k_disp_start(s32);
extern void goto_sub_step(s32);

void func_800733A4(void) {
    volatile s32 state;

    state = step_mode;

    switch (state) {
    case 0:
        text_sys_init();
        basyo_init2(byouki_noiroze_str0);
        gnsx(p_sei);
        sndi(byouki_noiroze_str1, 0, 31);
        byouki_kind = (byouki_kind & 0xFFF0) | 2;
        k_disp_start(3);
        step_mode++;
        break;
    case 1:
        if ((u32)step_counter < 61) {
            break;
        }
        if ((u32)step_counter < 257) {
            if (pad_data == 0) {
                return;
            }
        }
        goto_sub_step(255);
        break;
    }
}
