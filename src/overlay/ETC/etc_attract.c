/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_AS1_TIE_REJECT=30,40 IDO52_COPYPROP_VAR=-8,-16,-4,-12 IDO52_COPYPROP_SITE=lod:1:deny,lod:6:deny,lod:8:deny,lod:9:deny IDO52_FORCE_WEB_COLOR=8:a3,81:t0,53:a2,55:a2,19:v1 */
/* @SymbolStart: 0x8014418C */
/* @SymbolSize: 780 */
/* Address: 0x80144194 (true entry 0x8014418C, 2 insns hoisted above .sym) */
#include "types.h"
#include "tokimeki/sprite.h"

extern goto_sub_sub_step();
extern get_CD_State();
extern debug_hex_show();
extern load_etc_tpage();
extern etc_roll_move();
extern etc_sprite_init();
extern etc_sprite_col_set();

extern u8 now_girl;
extern u32 step_counter;
extern s32 Sprites;
extern volatile s32 etc_roll_pic;
extern s32 etc_roll_girl_tbl[];

etc_attract()
{
    u32 rem;
    s32 quot;
    s32 n;
    s32 span;
    s32 idx;
    s32 val;
    s32 pad;

    Sprites = 0x80000000;
    (*(s32 *)&((SPRITE_REC *)&Sprites)[1].attribute) = 0x80000000;
    etc_roll_move();

    quot = step_counter / 701;
    rem = step_counter % 701;

    if (rem == 1) {
        idx = etc_roll_pic;
        if (idx == 0) {
            load_etc_tpage(now_girl);
        } else if (idx < 15) {
            val = etc_roll_girl_tbl[idx - 1];
            etc_roll_pic = idx;
            if (now_girl == val) {
                idx = idx + 1;
                val = etc_roll_girl_tbl[idx - 1];
                etc_roll_pic = idx;
            }
            load_etc_tpage(val);
        } else {
            etc_roll_pic = idx;
            load_etc_tpage(now_girl);
        }
        etc_roll_pic = etc_roll_pic + 1;
    } else if (quot < 15) {
        if (rem == 2) {
            etc_sprite_init(quot);
        } else if (rem < 160) {
            n = rem;
            span = n * 3;
            etc_sprite_col_set(2, span / 4);
            etc_sprite_col_set(3, (span - 60) / 4);
            etc_sprite_col_set(4, (span - 120) / 4);
        }
    }

    if (etc_roll_pic < 16) {
        rem = step_counter % 701;
        if (rem >= 542) {
            n = 701 - rem;
            if (0) {}
            span = n * 3;
            etc_sprite_col_set(2, (span - 120) / 4);
            etc_sprite_col_set(3, (span - 60) / 4);
            etc_sprite_col_set(4, span / 4);
        }
    }

    if (step_counter >= 601) {
        if ((get_CD_State() & 2) != 0) {
            goto_sub_sub_step(2);
        }
    }

    debug_hex_show(step_counter, 0, -10);
    debug_hex_show(quot, 0, 0);
    debug_hex_show(etc_roll_pic, 0, 10);
    debug_hex_show(etc_roll_girl_tbl[etc_roll_pic - 1], 0, 20);
}
