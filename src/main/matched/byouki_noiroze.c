/* Function: byouki_noiroze (o.bin proximity guess "title init light" retired 2026-10-08: the title_init_* family is at 0x800619C0-0x80061B3C) */
/* Address: 0x800733A0 (absorbed cross-prologue head; body 0x800733A8) */
/* Size: 224 bytes (56 instructions, head included) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x800733A0 */
/* @SymbolSize: 224 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0x80:rt:t9:v1 0x84:rs:t9:v1 0x84:rt:t9:v1 0x8c:rs:t9:v1 0x8c:rt:t0:t9 0x94:rt:t0:t9 0xb4:rt:t1:t0 0xb8:rs:t1:t0 0xb8:rt:t1:t0 0xc0:rs:t1:t0 */
#include "types.h"

extern u8 step_mode;
extern volatile s32 step_counter;
extern s32 pad_data;
extern u8 byouki_kind;
extern char byouki_noiroze_str0[];
extern char byouki_noiroze_str1[];
extern char p_sei[];

extern void text_sys_init(void);
extern void basyo_init2(char *);
extern void gnsx(char *);
extern void sndi(char *, s32, s32);
extern s32  k_disp_start(s32);
extern s32  goto_sub_step(s32);

s32 byouki_noiroze(void)
{
    s32 r;

    r = step_mode;
    switch (r) {
    case 0:
        text_sys_init();
        basyo_init2(byouki_noiroze_str0);
        gnsx(p_sei);
        sndi(byouki_noiroze_str1, 0, 31);
        byouki_kind = (byouki_kind & 0xFFF0) | 2;
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
