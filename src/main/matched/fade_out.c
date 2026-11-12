/* Address: 0x8007EAE0 */
/* Size: 272 bytes (68 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolSize: 272 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=128 IDO52_AS1_KEEP_ANDI=2 IDO52_AS1_NO_PASS1_PROP=34 IDO52_AS1_NO_COPY_PROP=0,1 IDO52_AS1_TIE_REJECT=91,112 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0x0:rt:t6:v1 0x4:rs:t6:v1 0x4:rt:t6:v1 0xc:rs:t6:v1 0x20:rt:t7:t6 0x24:rs:t7:t6 0x24:rt:t7:t6 0x34:rs:t7:t6 0x34:rt:t8:t7 0x38:rt:t8:t7 0x3c:rt:t9:t8 0x40:rs:t9:t8 0x40:rt:t9:t8 0x48:rs:t9:t8 0x48:rt:t0:t9 0x4c:rt:t0:t9 0x54:rt:v0:a0 0x58:rt:v0:a0 0x60:rt:v0:a0 0x68:rt:v1:v0 0x6c:rt:v1:v0 0x74:rt:v1:v0 0x78:rt:t1:t0 0x7c:rs:t1:t0 0x7c:rt:t1:t0 0x80:rt:a0:a2 0x88:rs:t1:t0 0x88:rt:a0:a2 0x88:rd:t2:t1 0x8c:rt:t2:t1 0x90:rt:t3:t2 0x94:rs:t3:t2 0x94:rt:t3:t2 0x9c:rs:t3:t2 0x9c:rt:a0:a2 0x9c:rd:t4:t3 0xa0:rt:t4:t3 0xa8:rt:v0:a0 0xb0:rt:v1:v0 0xbc:rt:v0:v1 0xc0:rs:v0:v1 0xc0:rt:v0:a0 0xc8:rt:v0:a0 0xd0:rs:v0:a0 0xd0:rd:v1:v0 0xd4:rt:v1:v0 0xdc:rt:v1:v0 0xe4:rs:v0:a0 0xe4:rt:a0:v0 0xe8:rt:a0:v0 0xf0:rt:a0:v0 0xfc:rt:v0:a0 0x10c:rt:v0:a0 */
#include "types.h"
#include "tokimeki/sprite.h"

extern u8  fade_bri;
extern volatile u8  bustup2_cw_disp;
extern volatile u8  bustup_cw_disp;
extern s8  bustup2_cw_bright;
extern s32 bustup_on;
extern s32 fade_flag;
extern volatile s32 Sprites;
extern volatile s32 sprite1_attribute;
extern volatile u8  sprite1_r;

extern void fade_out_step(void);

void fade_out(void) {
    s32 idx;
    s32 k;

    if ((u32)fade_bri < 8) {
        bustup_on = 0;
        fade_flag = 0;
        bustup2_cw_disp = bustup2_cw_disp & 0xFF7F;
        bustup_cw_disp = bustup_cw_disp & 0xFF7F;
        k = 128;
        *(s8 *)&((SPRITE_REC *)&Sprites)[0].g = k;
        *(s8 *)&((SPRITE_REC *)&Sprites)[0].r = k;
        ((SPRITE_REC *)&Sprites)[1].g = 128;
        sprite1_r = 128;
        Sprites = (s32)((u32)Sprites | 0x80000000u);
        sprite1_attribute = (s32)((u32)sprite1_attribute | 0x80000000u);
        (*(s8 *)&((SPRITE_REC *)&Sprites)[0].b) = k;
        *(u8 *)&(*(s8 *)&((SPRITE_REC *)&Sprites)[1].b) = 128;
    }
    fade_out_step();

    idx        = fade_bri;
    bustup2_cw_bright = idx;
    ((SPRITE_REC *)&Sprites)[0].g = idx;
    ((SPRITE_REC *)&Sprites)[0].r = idx;
    ((SPRITE_REC *)&Sprites)[1].g = idx & 0xFF;
    sprite1_r = idx & 0xFF;
    (*(s8 *)&((SPRITE_REC *)&Sprites)[0].b) = idx;
    (*(s8 *)&((SPRITE_REC *)&Sprites)[1].b) = idx;
}
