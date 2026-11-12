/* Address: 0x80076D08 */
/* Size: 292 bytes (73 instructions) */
/* Region: Game (0x80041000-0x80086000) */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:5 */
/* @PostProc: patch_stack_complete --old 328 --new 336 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: remap_stack_slots --map '72:76,40:44' */
/* @PostProc: patch_insn_reg_field 0xE4:rt:t0:v1 0xE8:rs:t0:v1 0xE8:rt:t0:v1 0xF0:rs:t0:v1 0xF0:rt:t1:t0 0xF8:rt:t1:t0 */
#include "types.h"

extern u8 step_mode;
extern u8 machiawase_mode;
extern s32 machiawase_menu_set_tbl0[];
extern s32 machiawase_show_str0;
extern s32 machiawase_show_str1;
extern s32 machiawase_show_str2;
extern s32 p_sei;
extern u32 reserved_date;

extern void k_reset(s32 count);
extern void k_sub_reset_point_set(void);
extern void k_sub_reset(void);
extern void gnsx(s32 a0);
extern void strcpy(void *dst, void *src);
extern void strcat(void *dst, void *src);
extern s32  get_g_name(s32 a0, s32 a1);
extern void sndisp(s32 arg0, s32 arg1, void *arg2);
extern s32 k_sub_disp_start(s32 a0);
extern s32 xa_wait(void);
extern s32 sub_sub_step_up(void);

s32 machiawase_show(void) {
    u8 buf[256];
    s32 decbuf[8];
    u8 state;
    s32 ret;

    state = step_mode;
    ret = state;
    switch (state) {
    case 0:
        k_reset(1);
        k_sub_reset_point_set();
        k_sub_reset();
        gnsx((s32)&p_sei);
        strcpy(buf, &machiawase_show_str0);
        strcat(buf, (void *)machiawase_menu_set_tbl0[machiawase_mode]);
        strcat(buf, &machiawase_show_str1);
        get_g_name((s32)decbuf, (s32)((reserved_date << 3) >> 28));
        strcat(buf, decbuf);
        strcat(buf, &machiawase_show_str2);
        sndisp((s32)buf, 0, (void *)31);
        ret = k_sub_disp_start(4);
        step_mode = step_mode + 1;
        break;
    case 1:
        ret = xa_wait();
        break;
    case 2:
        ret = sub_sub_step_up();
        break;
    }
    return ret;
}
