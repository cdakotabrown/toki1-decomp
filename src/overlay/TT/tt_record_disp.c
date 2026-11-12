/* @SymbolSize: 240 */
#include "types.h"

extern u8 *tt_anim_buf;

extern void tt_tile_disp(void *rec);
extern void tt_rec_sprite_disp(void *rec);
extern void tt_obj_prim_disp(void *rec);

void tt_record_disp(void)
{
    u8 *s0;
    s32 s1;
    s32 s2, s3, s4, s5, s6;
    s32 v0;

    s0 = tt_anim_buf;
    s1 = 0;
    s2 = 128;
    s3 = -4;
    s4 = 96;
    s5 = 100;
    s6 = 128;

    for (; s1 != s2; s1++, s0 += 100) {
        if (s0[0] == 0) {
            continue;
        }
        if (s0[1] != 0) {
            continue;
        }
        v0 = s0[59] & s3;
        switch (v0) {
        case 96:
            tt_tile_disp(s0);
            break;
        case 100:
            tt_rec_sprite_disp(s0);
            break;
        case 128:
            tt_obj_prim_disp(s0);
            break;
        }
    }
}
