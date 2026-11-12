/* Address: 0x80144D58 */
/* Size: 356 bytes (89 insns) */
/* @SymbolSize: 356 */
/* @PostProc: patch_stack_complete --old 168 --new 192 */
/* @PostProc: remap_stack_slots --from 140 --to 108 --from 92 --to 168 --from 104 --to 156 --from 116 --to 144 --from 128 --to 132 */
#include "types.h"

typedef struct {
    s32 v[6];
} Words6;

extern Words6 bunkaken_menu_init_tbl;
extern s32 menu_on;

extern void k_reset(s32);
extern void set_kanji_string(s16, s16, s32, s32, s32);
extern void k_disp_start(s32);
extern void menu_set(s32, s32, s16 *, s16 *, s16 *, s16 *);
extern void cursor_init(s32);

void bunkaken_menu_init(void) {
    s32 i;
    Words6 imgs;
    s16 h[6];
    s16 w[6];
    s16 y[6];
    s16 x[6];

    imgs = bunkaken_menu_init_tbl;
    k_reset(1);
    for (i = 0; i != 6; i++) {
        x[i] = (i & 1) * 120 - 128;
        y[i] = (i >> 1) * 16 + 50;
        set_kanji_string(x[i], y[i], 0, imgs.v[i], 0);
        w[i] = 120;
        h[i] = 16;
    }
    menu_on = 1;
    k_disp_start(7);
    menu_set(0, 6, x, y, w, h);
    cursor_init(0);
}
