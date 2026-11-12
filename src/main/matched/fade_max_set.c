/* Function: fade_max_set (true entry 0x8007E674 -- the old symbol address */
/* Address: 0x8007E674 */
/* Size: 112 bytes (28 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @SymbolStart: 0x8007E674 */
/* @SymbolSize: 112 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:1:deny,lod:2:deny,lod:3:deny,lod:4:deny,lod:5:deny,lod:6:deny,lod:7:deny IDO52_AS1_NO_PASS1_PROP=13,15 IDO52_AS1_REPL_TGT_KEEP=0 */
#include "types.h"
#include "tokimeki/sprite.h"

extern s32 bustup_on;
extern u8 bustup2_cw_bright;
extern SPRITE_REC Sprites[];
extern void sub_sub_step_up(void);

void fade_max_set(void) {
    s32 a;
    s32 b;

    bustup_on = 1;
    bustup2_cw_bright = 128;
    a = 128;
    b = a;
    Sprites[0].g = b;
    Sprites[0].r = b;
    b = 128;
    Sprites[1].g = b;
    Sprites[1].r = b;
    Sprites[0].b = a;
    Sprites[1].b = b;
    sub_sub_step_up();
}
