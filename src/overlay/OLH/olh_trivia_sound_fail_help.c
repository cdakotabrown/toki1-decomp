/* @SymbolStart: 0x801363AC */
/* @SymbolSize: 264 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* Address: 0x801363AC (2-insn cross-prologue head) / 0x801363B4 (body) */
/* Size: 276 bytes (69 insns, head absorbed) */
#include "types.h"

extern u8 step_mode;
extern u8 *olh_page70_tbl_6;
extern u8 olh_main70_70_str0[];
extern u8 olh_main70_70_str1[];
extern u8 olh_main70_70_str2[];
extern u8 olh_main70_70_str3[];

extern void k_sub_reset(void);
extern s32 set_kanji_string(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern s32 xa_wait2(void);
extern s32 goto_sub_sub_step(s32 a0);

s32 olh_trivia_sound_fail_help(void)
{
    s32 ret;

    ret = step_mode;
    switch (step_mode) {
    case 0:
        k_sub_reset();
        set_kanji_string(-128, -64, 1, (s32)olh_page70_tbl_6, 0);
        set_kanji_string(-128, -48, 15, (s32)olh_main70_70_str0, 0);
        set_kanji_string(-128, -32, 15, (s32)olh_main70_70_str1, 0);
        set_kanji_string(-128, -16, 15, (s32)olh_main70_70_str2, 0);
        ret = set_kanji_string(-128, 0, 15, (s32)olh_main70_70_str3, 0);
        step_mode = step_mode + 1;
        break;
    case 1:
        ret = xa_wait2();
        break;
    case 2:
        ret = goto_sub_sub_step(0);
        break;
    }
    return ret;
}
