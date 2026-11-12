/* Address: 0x8007ECA8   (real start 0x8007ECA0 -- see WINDOW below) */
/* Size: 268 bytes (67 instructions) */
/* @SymbolStart: 0x8007ECA0 */
/* @SymbolSize: 268 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=128 IDO52_AS1_NO_PASS1_PROP=8,35,37 IDO52_AS1_TIE_REJECT=65,81 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0x48:rt:a1:a0 0x50:rt:a1:a0 0x64:rt:a1:a0 0x70:rt:v1:a2 0x74:rt:v1:a2 0x7c:rt:v1:a2 0x84:rt:a0:v1 0x88:rt:a0:v1 0x90:rt:a0:v1 0xa4:rt:a0:v1 0xac:rt:v1:a2 */
#include "types.h"
#include "tokimeki/sprite.h"

extern volatile u8 bg_change_tbl2;
extern volatile s32 Sprites;
extern volatile s32 sprite1_attribute;
extern s32 fade_flag;

extern void bg_change(s32);
extern void indexed_byte_toggle(s32);
extern void sub_sub_step_up(void);

s32 sprite_fade(void) {
    u8  val;
    s32 k;

    val = bg_change_tbl2;
    if ((u32)val >= 8) {
        val = val - 8;
    } else {
        bg_change(1);
        indexed_byte_toggle(1);
        sub_sub_step_up();
        Sprites |= 0x80000000;
        sprite1_attribute |= 0x80000000;
        k = 128;
        (*(s8 *)&((SPRITE_REC *)&Sprites)[0].g) = k;
        (*(s8 *)&((SPRITE_REC *)&Sprites)[0].r) = k;
        *(u8 *)&(*(s8 *)&((SPRITE_REC *)&Sprites)[1].g) = 128;
        *(u8 *)&(*(s8 *)&((SPRITE_REC *)&Sprites)[1].r) = 128;
        fade_flag = 1;
        *(u8 *)&(*(s8 *)&((SPRITE_REC *)&Sprites)[1].b) = 128;
        (*(s8 *)&((SPRITE_REC *)&Sprites)[0].b) = k;
        return 0;
    }

    (*(s8 *)&((SPRITE_REC *)&Sprites)[1].g) = val;
    (*(s8 *)&((SPRITE_REC *)&Sprites)[1].r) = val;
    (*(s8 *)&((SPRITE_REC *)&Sprites)[0].g) = val;
    (*(s8 *)&((SPRITE_REC *)&Sprites)[0].r) = val;
    bg_change_tbl2 = val;
    (*(s8 *)&((SPRITE_REC *)&Sprites)[1].b) = val;
    (*(s8 *)&((SPRITE_REC *)&Sprites)[0].b) = val;
    bg_change(1);
}
