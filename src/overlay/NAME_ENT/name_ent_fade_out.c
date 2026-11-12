/* @SymbolStart: 0x80139808 */
/* @SymbolSize: 196 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* name_ent_fade_out - NAME_ENT overlay, Address: 0x80139810, Size: 192 bytes (48 insns) */
#include "types.h"

extern u32 step_counter;
extern u8 step_mode;
extern u8 screen_bri;

extern u8 name_ent_fade_out_str0[];

extern s32 set_dec_bri(u32);
extern void goto_step(s32);
extern void kanji_cell_width_set(s32);
extern s32 printf(u8 *);

s32 name_ent_fade_out(void)
{
    s32 ret;
    u32 timer;

    ret = step_mode;
    switch (step_mode) {
        case 0:

            timer = step_counter;
            if (timer >= 65) {
                step_mode = step_mode + 1;
            }
            break;

        case 1:

            if (screen_bri >= 4U) {
                screen_bri -= 4;
            } else {
                step_mode = step_mode + 1;
                screen_bri = 0;
            }
            ret = set_dec_bri(screen_bri);
            break;

        case 2:

            goto_step(21);
            kanji_cell_width_set(14);
            ret = printf(name_ent_fade_out_str0);
            break;
    }
    return ret;
}
