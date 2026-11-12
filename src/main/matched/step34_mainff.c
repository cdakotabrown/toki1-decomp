/* Address: 0x8005A3F0 */
/* Size: 264 bytes (66 instructions) */
/* Region: Game (0x80041000-0x80086000) */
/* Compiler: IDO 5.2 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v1:11,v0:22 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0xf0 --offset2 0xf4 */
/* @PostProc: patch_insn_reg_field 0xac:rt:t8:v1 0xb0:rs:t8:v1 0xb0:rt:t8:v1 0xb8:rs:t8:v1 */
/* @PostProc: patch_insn_reg_field 0xe8:rt:t9:t8 0xf0:rt:t9:t8 0xf4:rs:t9:t8 */
/* `@PostProc: patch_stack_complete --old 24 --new 40` was replaced by */
/* `@CompileFlags: -Wc,-pic1`.  Verified the SAME +16: the object compiles */
#include "types.h"

extern s32 step_counter;
extern u8  now_sub_sub_step;
extern u8  screen_bri;

extern void step34_init0(void);
extern void step34_ffinit(void);
extern void step34_init2(void);
extern void step34_init3(void);
extern void hizuke_show(void);
extern void message_window_show(void);
extern void parameter_show(void);
extern void icon_cursor_move(s32);
extern void step_directional_scroll_once(void);
extern void basyo_show(void);

s32 step34_mainff(void) {
    u8 v1;

    step_counter = step_counter + 1;

    v1 = now_sub_sub_step;
    switch (v1) {
    case 0:
        step34_init0();
        break;
    case 1:
        step34_ffinit();
        break;
    case 2:
        step34_init2();
        break;
    case 3:
        step34_init3();
        break;
    }

    hizuke_show();
    message_window_show();
    parameter_show();
    icon_cursor_move(0);
    step_directional_scroll_once();

    v1 = now_sub_sub_step;
    if ((u32)v1 < 2) {
        return;
    }
    basyo_show();

    if ((u32)screen_bri < 128) {
        screen_bri = (u8)(screen_bri + 4);
        return screen_bri;
    }

}
