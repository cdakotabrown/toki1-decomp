/* Address: 0x8007E9C8 */
/* Size: 176 bytes (44 instructions)   [0x8007E9C8 .. 0x8007EA77] */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x8007E9C8 */
/* @SymbolSize: 176 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_AS1_KEEP_ANDI=1 IDO52_AS1_NO_PASS1_PROP=18 IDO52_AS1_NO_COPY_PROP=0,1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0x20:rt:v0:v1 0x24:rs:v0:v1 0x24:rt:v0:a0 0x30:rt:a1:a2 0x38:rs:a1:a2 0x38:rt:a1:a2 0x3c:rt:v0:a0 0x44:rt:a1:a2 0x58:rt:a1:a2 0x64:rs:v0:a0 0x64:rd:v1:v0 0x68:rt:v1:v0 0x70:rt:v1:v0 0x78:rs:v0:a0 0x78:rt:a0:v0 0x7c:rt:a0:v0 0x84:rt:a0:v0 0x9c:rt:v0:a0 0xac:rt:v0:a0 */
#include "types.h"
#include "tokimeki/sprite.h"

extern u8  bustup2_cw_disp;
extern s8  bustup2_cw_bright;
extern u8  fade_bri;
extern volatile s32 Sprites;
extern volatile s32 sprite1_attribute;
extern s32 bustup_on;

extern void fade_in_step(void);

void fade_in(void) {
    s32 temp;

    bustup2_cw_disp = bustup2_cw_disp | 0x80;
    fade_in_step();
    temp = fade_bri;
    bustup2_cw_bright = temp;
    Sprites = Sprites & 0x7FFFFFFF;
    sprite1_attribute = sprite1_attribute & 0x7FFFFFFF;
    ((SPRITE_REC *)&Sprites)[0].g = temp;
    ((SPRITE_REC *)&Sprites)[0].r = temp;
    ((SPRITE_REC *)&Sprites)[1].g = temp & 0xFF;
    ((SPRITE_REC *)&Sprites)[1].r = temp & 0xFF;
    bustup_on = 1;
    (*(s8 *)&((SPRITE_REC *)&Sprites)[0].b) = temp;
    (*(s8 *)&((SPRITE_REC *)&Sprites)[1].b) = temp;
}
