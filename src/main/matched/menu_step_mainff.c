/* Function: menu_step_mainff (o.bin name; prologue 0x8005BE14) */
/* Address: 0x8005BE0C (absorbed cross-prologue head; body 0x8005BE14) */
/* Size: 180 bytes (45 instructions, head included) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @SymbolStart: 0x8005BE0C */
/* @SymbolSize: 180 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0x20:rt:t8:t7 0x60:rt:t6:v1 0x64:rs:t6:v1 0x64:rt:t6:v1 0x6C:rs:t6:v1 0x6C:rt:t7:t6 0x74:rt:t7:t6 0x78:rs:t8:t7 0x78:rt:t8:t7 0x80:rs:t8:t7 0x80:rt:t9:t8 0x84:rs:t9:t8 */
#include "types.h"

extern u8 step_mode;
extern s32 pad_trig;
extern char menu_step_mainff_str0[];

extern void device_cmd_send(s32);
extern void k_sub_reset(void);
extern void set_kanji_string(s32, s32, s32, char *, s32);
extern s32  k_sub_disp_start(s32);
extern void cursor_work_copy(s32);
extern void goto_sub_step(s32);
extern s32  goto_sub_sub_step(s32);

s32 menu_step_mainff(void)
{
    s32 mode;
    s32 ret;

    mode = step_mode;
    ret = mode;
    switch (mode) {
    case 0:
        device_cmd_send(0x502);
        k_sub_reset();
        set_kanji_string(-128, 50, 1, menu_step_mainff_str0, 0);
        ret = k_sub_disp_start(1);
        step_mode++;
        break;
    case 1:
        if (pad_trig & 0x860) {
            cursor_work_copy(1);
            goto_sub_step(0);
            ret = goto_sub_sub_step(2);
        }
        break;
    }
    return ret;
}
