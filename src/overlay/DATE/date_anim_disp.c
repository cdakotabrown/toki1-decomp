/* @CompileFlags: -Wc,-pic0 */
/* Function: date_anim_disp */
/* Overlay: DATE */
/* Address: 0x8014E390, 2000 bytes (500 instructions) */
#include "types.h"

typedef struct {
    s32 a;
    s32 b;
} Ent;

typedef struct {
    Ent e[4];
} Blk8;

extern s32 date_quad_disp();
extern s32 dtd_on_tpage();

extern Blk8 date_anim_disp_tbl;
extern u32 frame_counter;
extern s16 date_anim_ptn;
extern s16 date_anim_ptn2;

void date_anim_disp(void)
{
    Ent *buf;
    Blk8 blk;
    u32 frame;
    u32 v0;
    s32 idx;

    blk = date_anim_disp_tbl;
    buf = blk.e;

    frame = frame_counter;

    if (frame % 120 == 0) {
        date_anim_ptn = date_anim_ptn + 1;
    }
    if (date_anim_ptn >= 3) {
        date_anim_ptn = 0;
    }
    if (frame % 100 == 0) {
        date_anim_ptn2 = date_anim_ptn2 + 1;
    }
    if (date_anim_ptn2 >= 4) {
        date_anim_ptn2 = 0;
    }

    v0 = frame % 36;
    if (v0 < 12) {
        idx = date_anim_ptn2;
        date_quad_disp(160, -160, 180, -160, 110, 40, 130, 40,
                      buf[idx].a, buf[idx].b, 9, 1);
        idx = date_anim_ptn2;
        date_quad_disp(165, -160, 195, -160, 100, 40, 70, 40,
                      buf[idx].a, buf[idx].b, 9, 1);
        idx = date_anim_ptn2;
        date_quad_disp(170, -160, 180, -160, 65, 40, 50, 40,
                      buf[idx].a, buf[idx].b, 9, 1);
        idx = date_anim_ptn2;
        date_quad_disp(160, -160, 175, -160, 105, 40, 125, 40,
                      buf[idx].a, buf[idx].b, 9, 1);
    } else if (v0 < 24) {
        idx = date_anim_ptn2;
        date_quad_disp(155, -160, 175, -160, 105, 40, 130, 40,
                      buf[idx].a, buf[idx].b, 9, 1);
        idx = date_anim_ptn2;
        date_quad_disp(155, -160, 195, -150, 90, 40, 60, 40,
                      buf[idx].a, buf[idx].b, 9, 1);
        idx = date_anim_ptn2;
        date_quad_disp(165, -160, 175, -160, 60, 40, 45, 40,
                      buf[idx].a, buf[idx].b, 9, 1);
        idx = date_anim_ptn2;
        date_quad_disp(150, -160, 160, -160, 100, 40, 115, 40,
                      buf[idx].a, buf[idx].b, 9, 1);
    }
    dtd_on_tpage(0, 0, 9, 3, 0);

    frame = frame_counter;
    v0 = frame % 30;
    if (v0 < 10) {
        idx = date_anim_ptn;
        date_quad_disp(30, -160, 100, -160, -320, 40, -250, 40,
                      buf[idx].a, buf[idx].b, 13, 1);
        idx = date_anim_ptn;
        date_quad_disp(10, -160, 25, -160, -220, 40, -190, 40,
                      buf[idx].a, buf[idx].b, 13, 1);
        idx = date_anim_ptn;
        date_quad_disp(0, -160, 20, -160, -180, 40, -130, 40,
                      buf[idx].a, buf[idx].b, 13, 1);
        idx = date_anim_ptn;
        date_quad_disp(25, -160, 40, -160, -300, 40, -280, 40,
                      buf[idx].a, buf[idx].b, 13, 1);
    } else if (v0 < 20) {
        idx = date_anim_ptn;
        date_quad_disp(20, -160, 90, -160, -340, 40, -270, 40,
                      buf[idx].a, buf[idx].b, 13, 1);
        idx = date_anim_ptn;
        date_quad_disp(10, -160, 30, -160, -240, 40, -210, 40,
                      buf[idx].a, buf[idx].b, 13, 1);
        idx = date_anim_ptn;
        date_quad_disp(0, -160, 15, -160, -200, 40, -150, 40,
                      buf[idx].a, buf[idx].b, 13, 1);
        idx = date_anim_ptn;
        date_quad_disp(15, -160, 25, -160, -310, 40, -290, 40,
                      buf[idx].a, buf[idx].b, 13, 1);
    }
    dtd_on_tpage(0, 0, 13, 3, 0);
}
