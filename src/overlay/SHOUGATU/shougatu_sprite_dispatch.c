/* shougatu_sprite_dispatch - SHOUGATU overlay, Address: 0x80138240, Size: 276 bytes. */
#include "types.h"

extern void sprite_scatter_show(s32, s32, s32, s32, s32, s32);

typedef struct { s32 w[37]; } Tbl;
extern Tbl shougatu_sprite_dispatch_tbl;
extern u8 now_sub_sub_step;
extern volatile u8 shougatu_girl2_basyo3;

void shougatu_sprite_dispatch(void)
{
    u32 v0;
    Tbl buf;

    buf = shougatu_sprite_dispatch_tbl;

    if (shougatu_girl2_basyo3 == 0) {
        v0 = now_sub_sub_step;
        if (v0 >= 19U && v0 < 28U) {
            sprite_scatter_show(96, -16, 48, 8, 8, 13);
        }
    }

    if (shougatu_girl2_basyo3 == 1) {
        v0 = now_sub_sub_step;
        if (v0 >= 19U && v0 < 28U) {
            sprite_scatter_show(-60, -32, 32, 8, 8, 13);
        }
    }

    v0 = now_sub_sub_step;
    ((void (*)(void))buf.w[v0])();
}
