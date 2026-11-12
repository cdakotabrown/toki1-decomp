/* Function: byouki_kega */
/* Address: 0x80073248 */
/* Size: 344 bytes (86 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x80073244 */
/* @SymbolSize: 348 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_AS1_PEEPREG_KEEP=0 */
#include "types.h"

extern u8 step_mode;
extern s32 step_counter;
extern s32 pad_trig;
extern s32 frame_counter;
extern u8 byouki_kind;
extern u8 ev_flag4b;
extern u8 season_no;
extern s32 event_girl_no;
extern s32 work_tarao;
extern char byouki_kega_str0[];
extern char byouki_kega_str1[];
extern char p_sei[];

extern void text_sys_init(void);
extern void basyo_init2(char *);
extern void gnsx(char *);
extern void sndi(char *, s32, s32);
extern void k_disp_start(s32);
extern void goto_step(s32);
extern void goto_sub_step(s32);

s32 byouki_kega(void) {
    s32 state = step_mode;

    switch (state) {
    case 0:
        text_sys_init();
        basyo_init2(byouki_kega_str0);
        gnsx(p_sei);
        sndi(byouki_kega_str1, 0, 31);
        byouki_kind = (byouki_kind & 0xFFF0) | 1;
        k_disp_start(3);
        step_mode++;
        break;
    case 1:
        if ((u32)step_counter < 61) { break; }
        if ((u32)step_counter < 257) {
            if (pad_trig == 0) { break; }
        }
        step_mode++;
        break;
    case 2: {
        s32 val = frame_counter;
        if ((u32)(val & 0xFF) < 25) {
            if (ev_flag4b == 0) {
                if (season_no == 0) {
                    goto_step(93);
                    event_girl_no = 11;
                    work_tarao = 2;
                    ev_flag4b = 1;
                    break;
                }
            }
        }
        goto_sub_step(255);
        break;
    }
    default:
        return;
    }
}
