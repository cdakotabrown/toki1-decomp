/* Address: 0x8007E5B4 (recorded) / real entry 0x8007E5AC */
/* Size: 260 bytes (recorded, OVER-STATED) / 200 bytes real */
/* Region: Game (0x80041000-0x80086000) */
/* Compiler: IDO 5.2 */
/* @SymbolStart: 0x8007E5AC */
/* @SymbolSize: 200 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=33:a2,29:a3,0:v1 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0x0:rt:v0:v1 0x4:rs:v0:v1 0x4:rt:v0:v1 0xc:rs:v0:v1 0x18:rs:v0:v1 */
#include "types.h"
#include "tokimeki/sprite.h"

extern u8  bg_change_tbl2;
extern s32 fade_flag;
extern volatile s32 Sprites;
extern volatile s32 sprite1_attribute;
extern u8  bustup2_cw_bright;

extern void sub_sub_step_up(void);

s32 fade_up(void)
{
    s32 v;
    s32 w;

    v = bg_change_tbl2;
    if ((u32)v < 128) {
        v = (u8)(v + 8);
    } else {
        sub_sub_step_up();
        fade_flag = 1;
        Sprites = Sprites & 0x7FFFFFFF;
        sprite1_attribute = sprite1_attribute & 0x7FFFFFFF;
        return 0;
    }
    w = v & 0xFF;
    ((SPRITE_REC *)&Sprites)[1].g = w;
    ((SPRITE_REC *)&Sprites)[1].r = w;
    ((SPRITE_REC *)&Sprites)[0].b = w;
    ((SPRITE_REC *)&Sprites)[0].g = w;
    ((SPRITE_REC *)&Sprites)[0].r = w;
    bustup2_cw_bright = v;
    bg_change_tbl2 = v;
    ((SPRITE_REC *)&Sprites)[1].b = v;
    return w;
}
