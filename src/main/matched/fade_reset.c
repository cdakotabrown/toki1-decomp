/* Function: fade_reset (true entry 0x8007E8A0 -- the old symbol address */
/* Address: 0x8007E8A0 */
/* Size: 192 bytes (48 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @SymbolStart: 0x8007E8A0 */
/* @SymbolSize: 192 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:1:deny,lod:2:deny,lod:3:deny,lod:4:deny,lod:5:deny,lod:6:deny,lod:7:deny IDO52_FORCE_WEB_COLOR=51:a2 IDO52_AS1_NO_PASS1_PROP=28,30 IDO52_AS1_REPL_TGT_KEEP=0 */
#include "types.h"
#include "tokimeki/sprite.h"

#ifdef PC_PORT

extern u8 bustup_cw_disp;
extern u8 bustup2_cw_disp;
extern u32 D_80129CB0;
#define FLAG_EF bustup2_cw_disp
#define FLAG_AB bustup_cw_disp
#define WORD_B0 D_80129CB0
#define WORD_D4 Sprites[1].attribute
#else
extern u8 bustup_cw_disp[];
extern u32 D_80129CB0[];
#define FLAG_EF bustup_cw_disp[0x44]
#define FLAG_AB bustup_cw_disp[0]
#define WORD_B0 D_80129CB0[0]
#define WORD_D4 D_80129CB0[9]
#endif
extern s32 bustup_on;
extern s32 fade_flag;
extern SPRITE_REC Sprites[];
extern void sub_sub_step_up(void);

void fade_reset(void) {
    s32 a;
    s32 b;

    FLAG_EF &= 0xFF7F;
    FLAG_AB &= 0xFF7F;
    WORD_B0 |= 0x80000000;
    WORD_D4 |= 0x80000000;
    a = 128;
    b = a;
    Sprites[0].g = b;
    Sprites[0].r = b;
    b = 128;
    Sprites[1].g = b;
    Sprites[1].r = b;
    bustup_on = 0;
    fade_flag = 1;
    Sprites[0].b = a;
    Sprites[1].b = b;
    sub_sub_step_up();
}
