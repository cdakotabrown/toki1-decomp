/* Function: func_80076D10 */
/* Address: 0x80076D10 */
/* Size: 284 bytes (71 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_func_80076D10.py */
#include "types.h"

extern u8 machiawase_mode;
extern s32 reserved_date;
extern u8 step_mode;
extern s32 D_800C9918[];
extern s32 machiawase_show_str0;
extern s32 machiawase_show_str1;
extern s32 machiawase_show_str2;

extern void k_reset(s32);
extern void k_sub_reset_point_set(void);
extern void k_sub_reset(void);
extern void gnsx(s32);
extern void strcpy(void *, void *);
extern void strcat(void *, void *);
extern void get_g_name(void *, s32);
extern void sndisp(void *, s32, s32);
extern void k_sub_disp_start(s32);
extern void xa_wait(void);
extern void sub_sub_step_up(void);

extern u8 p_sei;

s32 func_80076D10(s32 sw) {
    s8 sp4C[260];
    s8 sp2C[32];

    switch (sw) {
    case 0:
        k_reset(1);
        k_sub_reset_point_set();
        k_sub_reset();
        gnsx((s32)&p_sei);
        strcpy(sp4C, &machiawase_show_str0);
        strcat(sp4C, (void *)&D_800C9918[machiawase_mode]);
        strcat(sp4C, &machiawase_show_str1);
        get_g_name(sp2C, (u32)(reserved_date << 3) >> 28);
        strcat(sp4C, sp2C);
        strcat(sp4C, &machiawase_show_str2);
        sndisp(sp4C, 0, 31);
        k_sub_disp_start(4);
        step_mode = step_mode + 1;
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        sub_sub_step_up();
        break;
    }
    return sw;
}
