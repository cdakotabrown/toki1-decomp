/* @SymbolStart: 0x80139620 */
/* @SymbolSize: 224 */
/* @PostProc: swap_registers a0:v1 --unidirectional --exclude-offsets 0x44,0x48,0x50,0x54,0x58,0x68,0x6c,0x80,0xc0,0xc8 */
/* @PostProc: rename_register_chain --chain t7:t6 --chain t6:v1 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x30:rs:v0:v1 0x4c:rt:v1:a0 0x5c:rt:v1:a0 0x60:rs:v1:a0 0x60:rt:v1:a0 0x70:rt:v1:a0 0x74:rs:v1:a0 0x74:rt:v1:a0 0x88:rd:v1:a0 0xd0:rd:v1:a0 */
/* Address: 0x80139628, Size: 244 bytes (61 insns w/ 7 cross-prologue trailing) */
#include "types.h"

extern char p_sei[];
extern char tel_rusu10_str0[];
extern char tel_rusu10_str1[];
extern u8 step_mode;

extern void k_sub_reset(void);
extern void device_cmd_send(s32);
extern void gnsx(char *);
extern void sndi(char *, s32, s32);
extern void k_sub_disp_start(s32);
extern void xa_wait(void);
extern void text_sys_init(void);
extern void class_clear_f(void);
extern void goto_step(s32);
extern void menu_bar_show(s32);
extern void cal_base_show(void);

void tel_rusu10(void) {
    u8 cmd;

    cmd = step_mode;
    switch (cmd) {
    case 0:
        k_sub_reset();
        device_cmd_send(1292);
        gnsx(p_sei);
        sndi(tel_rusu10_str0, 0, 31);
        sndi(tel_rusu10_str1, 1, 31);
        k_sub_disp_start(0);
        step_mode = step_mode + 1;
        goto common;
    case 1:
        xa_wait();
        goto common;
    case 2:
        text_sys_init();
        class_clear_f();
        goto_step(70);
        break;
    }
common:
    menu_bar_show(0);
    cal_base_show();
}
