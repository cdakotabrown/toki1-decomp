/* @SymbolStart: 0x80137894 */
/* @SymbolSize: 832 */
/* @CompileFlags: -cckr */
/* @PostProc: swap_instructions --offset1 0x340 --offset2 0x34C */
/* @PostProc: swap_instructions --offset1 0x344 --offset2 0x34C */
/* @PostProc: swap_instructions --offset1 0x164 --offset2 0x168 */
#include "types.h"
#include "tokimeki/class_w.h"

typedef struct {
    u8  unk0[3];
    u8  flags;
    u8  unk4[20];
    s16 h24;
    s16 h26;
    s16 h28;
    u8  unk1E[8];
    s16 x;
    s16 h28b;
    s16 y;
    u8  unk2C[24];
} Rec;

extern Rec cw96_active[];
extern u8  step_mode;
extern u8  album_work[];
extern u8  screen_w[];
extern u8  option_talk_init_str0[];
extern CLASS_W class_w[];

option_talk_init()
{
    s32 i;
    s32 a;
    s32 b;
    s32 c;
    s16 xs[16];
    s16 ys[16];
    s16 fl[16];
    s16 sz[16];

    if (step_mode != 0) {
        class_w[159].scale_x = class_w[159].scale_x / 17 * 16;
        class_w[159].scale_y = class_w[159].scale_y / 17 * 16;
        if (class_w[159].scale_x < 4097) {
            if (class_w[159].scale_y) {
            }
            class_w[159].scale_x = 4096;
            class_w[159].scale_y = 4096;
            for (i = 0; i < 14; i++) {
                xs[i] = i / 7 * 128 - 120;
                ys[i] = i % 7 * 18 - 72;
                fl[i] = 128;
                sz[i] = 16;
                cw96_active[i + 15].h24 = i * 2 + 64;
                cw96_active[i + 15].flags = 0;
                cw96_active[i + 15].x = xs[i];
                cw96_active[i + 15].y = ys[i];
                if (album_work[i + 20] == 0 && screen_w[i + 1340] == 0) {
                    fl[i] = 0;
                }
            }
            menu_set(1, 14, xs, ys, fl, sz);
        }
    }
    if (step_mode == 0) {
        class_w[159].frame = 8;
        class_w[159].x.half[1] = -160;
        class_w[159].y.half[1] = -120;
        step_mode++;
    } else if (step_mode == 1) {
        for (i = 11; i < 15; i++) cw96_active[i].flags &= ~0x80;
        cw96_active[10].h28 = cw96_active[10].h28 / 9 * 8;
        if (cw96_active[10].h28 < 9) {
            step_mode = 2;
        }
    } else {
        k_disp_switch(6, 0);
        set_kanji_string(-128, -88, 0, option_talk_init_str0, 0);
        k_sub_disp_start(1);
        sub_sub_step_up();
    }
    cw96_active[10].flags = 0;
    class_w[159].disp_flag = 0;
    class_w[159].scale_x = 4096;
    class_w[159].scale_y = 4096;
    return 0;
}
