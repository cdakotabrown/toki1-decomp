/* @SymbolStart: 0x801339BC */
/* @SymbolSize: 348 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* Address: 0x801339BC (2-insn cross-prologue head) / 0x801339C4 (body) */
/* Size: 356 bytes (89 insns, head absorbed) */
#include "types.h"

extern u8 step_mode;
extern u8 *olh_page20_tbl_6;
extern u8 olh_memcard_menu_step3_str0[];
extern u8 olh_memcard_menu_step3_str1[];
extern u8 olh_memcard_menu_step3_str2[];
extern u8 olh_memcard_menu_step3_str3[];
extern u8 olh_memcard_menu_step3_str4[];
extern u8 olh_memcard_menu_step3_str5[];
extern u8 olh_memcard_menu_step3_str6[];

extern void k_sub_reset(void);
extern s32 set_kanji_string(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern s32 xa_wait2(void);
extern s32 goto_sub_sub_step(s32 a0);

s32 olh_memcard_menu_step3(void)
{
    s32 ret;

    ret = step_mode;
    switch (step_mode) {
    case 0:
        k_sub_reset();
        set_kanji_string(-128, -64, 1, (s32)olh_page20_tbl_6, 0);
        set_kanji_string(-128, -48, 15, (s32)olh_memcard_menu_step3_str0, 0);
        set_kanji_string(-128, -32, 15, (s32)olh_memcard_menu_step3_str1, 0);
        set_kanji_string(-128, 0, 15, (s32)olh_memcard_menu_step3_str2, 0);
        set_kanji_string(-128, 16, 15, (s32)olh_memcard_menu_step3_str3, 0);
        set_kanji_string(-128, 32, 15, (s32)olh_memcard_menu_step3_str4, 0);
        set_kanji_string(-128, 48, 15, (s32)olh_memcard_menu_step3_str5, 0);
        ret = set_kanji_string(-128, 64, 15, (s32)olh_memcard_menu_step3_str6, 0);
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
