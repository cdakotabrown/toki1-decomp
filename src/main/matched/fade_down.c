/* Address: 0x8007E780 (true start 0x8007E778: the bg_change_tbl2 read is */
/* Size: 296 bytes (74 instructions) over the corrected window */
/* Region: Game (IDO 5.2) */
/* @SymbolStart: 0x8007E778 */
/* @SymbolSize: 296 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    s32 flags;
    u8 pad4[16];
    u8 l;
    u8 r;
    u8 x;
    u8 pad23[13];
} Voice;

extern u8 bg_change_tbl2;
typedef struct {
    u8 a;
    u8 pad1[67];
    u8 b;
    u8 pad69[3];
    u8 c;
} Snd;

extern Snd bustup_cw_disp;
extern Voice Sprites[2];
extern s32 bustup_on;
extern s32 fade_flag;

fade_down()
{
    if (bg_change_tbl2 >= 8) {
        bg_change_tbl2 -= 8;
    } else {
        bustup_cw_disp.b &= ~0x80;
        bustup_cw_disp.a &= ~0x80;
        sub_sub_step_up();
        Sprites[0].flags |= 0x80000000;
        Sprites[1].flags |= 0x80000000;
        Sprites[0].l = Sprites[0].r = Sprites[0].x = 128;
        Sprites[1].l = Sprites[1].r = Sprites[1].x = 128;
        bustup_on = 0;
        fade_flag = 1;
        return 0;
    }
    Sprites[0].l = Sprites[0].r = Sprites[1].l = Sprites[1].r = Sprites[0].x = Sprites[1].x = bg_change_tbl2;
    bustup_cw_disp.c = bg_change_tbl2;
}
