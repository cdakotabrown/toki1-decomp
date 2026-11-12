/* Function: byouki_sick (scene step; the o.bin title_* proximity guess was retired 2026-10-08) */
/* Address: 0x80073480 (absorbed cross-prologue head; body 0x80073488) */
/* Size: 236 bytes (59 instructions, head included) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x80073480 */
/* @SymbolSize: 236 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0x8c:rt:t9:v1 0x90:rs:t9:v1 0x90:rt:t9:v1 0x98:rs:t9:v1 0x98:rt:t0:t9 0xa0:rt:t0:t9 0xc0:rt:t1:t0 0xc4:rs:t1:t0 0xc4:rt:t1:t0 0xcc:rs:t1:t0 */
#include "types.h"

extern u8 step_mode;
extern volatile s32 step_counter;
extern s32 pad_data;
extern u8 byouki_kind;
extern char byouki_sick_str0[];
extern char byouki_sick_str1[];
extern char p_sei[];

extern void text_sys_init(void);
extern void basyo_init2(char *);
extern void gnsx(char *);
extern void device_port_init(s32, s32);
extern void sndi(char *, s32, s32);
extern s32  k_disp_start(s32);
extern s32  goto_sub_step(s32);

s32 byouki_sick(void)
{
    s32 r;

    r = step_mode;
    switch (r) {
    case 0:
        text_sys_init();
        basyo_init2(byouki_sick_str0);
        gnsx(p_sei);
        device_port_init(0, 514);
        sndi(byouki_sick_str1, 0, 31);
        byouki_kind = (byouki_kind & 0xFFF0) | 3;
        r = k_disp_start(3);
        step_mode++;
        break;
    case 1:
        r = step_counter;
        if ((u32)r < 61) {
            break;
        }
        if ((u32)r < 257) {
            if (pad_data == 0) {
                break;
            }
        }
        r = goto_sub_step(255);
        break;
    }
    return r;
}
