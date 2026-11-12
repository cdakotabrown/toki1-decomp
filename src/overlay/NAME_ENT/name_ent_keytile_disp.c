/* @CompileEnv: IDO52_AS1_KEEP_DEAD_RELOAD=2,3 IDO52_AS1_NO_COPY_PROP=3,4 */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 frame_counter;
extern u8 now_sub_step;
extern u8 album_work[];
extern u8 card_sys_present;
extern CLASS_W class_w[];

extern void _sprite_set_box_shade();
extern void dtd_on_tpage();

void name_ent_keytile_disp(void)
{
    s32 i;
    s32 v;
    s32 seven;
    s32 stride;
    s32 ff;

    seven = 7;
    stride = 31;
    ff = 255;
    for (i = 0; i != 14; i++) {
        v = album_work[i + 5];
        if (v == 0) {
            _sprite_set_box_shade((i / seven) * stride - 144, (i % seven) * 16 - 18,
                          30, 15, 2, 0x808080, 129);
            v = album_work[i + 5];
        }
        if (ff == v) {
            _sprite_set_box_shade((i / seven) * stride - 144, (i % seven) * 16 - 18,
                          30, 15, 2, 0x8080, 129);
        }
    }

    dtd_on_tpage(0, 0, 2, 2, 0);

    if (now_sub_step != 0) {
        if (card_sys_present != 0) {
            v = frame_counter & 0x1F;
            if (v != 0) {
                if (v == 16) {
                    class_w[126].disp_flag &= 0x7F;
                }
            } else {
                class_w[126].disp_flag |= 0x80;
            }
            class_w[126].clut = 17;
            class_w[127].clut = 0;
        } else {
            v = frame_counter & 0x1F;
            if (v != 0) {
                if (v == 16) {
                    class_w[127].disp_flag &= 0x7F;
                }
            } else {
                class_w[127].disp_flag |= 0x80;
            }
            class_w[126].clut = 0;
            class_w[127].clut = 17;
        }
    }
}
