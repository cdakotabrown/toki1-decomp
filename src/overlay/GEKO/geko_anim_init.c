/* @PostProc: patch_insn_reg_field 0xec:rd:v1:t9 0xf0:rs:v1:t9 0xf0:rt:t9:t0 0xf8:rt:t9:t0 0x100:rt:t0:t3 0x128:rt:t1:t4 0x12c:rt:t2:t5 0x138:rt:t0:t3 0x13c:rt:t1:t4 0x14c:rt:t2:t5 0x168:rd:v1:t1 0x16c:rs:v1:t1 0x16c:rt:t3:t2 0x170:rt:t3:t2 0x180:rt:t4:t6 0x184:rt:t4:t6 0x18c:rd:v0:v1 0x190:rt:v0:v1 0x1a0:rd:t5:t7 0x1a4:rt:v0:v1 0x1a8:rd:t6:t8 0x1a8:rt:t5:t7 0x1b0:rs:t6:t8 0x1b8:rs:t6:t8 */
/* geko_anim_init -- GEKO overlay, Address: 0x8013E14C, Size: 484 bytes */
#include "types.h"
#include "tokimeki/date_obj_res_ex.h"

extern void geko_loadbuf_init(void);
extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void load_image_rect(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern s32  rand(void);
extern void sub_sub_step_up(void);


typedef struct {
    s32 f00;
    s16 f04;
    s16 f06;
    s16 f08;
    s16 f0A;
    s16 f0C;
    u8  f0E;
    u8  f0F;
    s16 f10;
    s16 f12;
    u8  f14;
    u8  f15;
    u8  f16;
    s16 f18;
    s16 f1A;
    s16 f1C;
    s16 f1E;
    s32 f20;
} AnimEntry;

extern AnimEntry Sprites[];
extern u8  sprite_state[];
extern s32 tpage_buf[];
extern s16 geko_ptc_phase[];
extern s32 geko_event0_res[];
extern GEKO_OBJ_RES_EX geko_ptc_res;

void geko_anim_init(void)
{
    s16 i;
    s32 yDiv;
    s32 xDiv;
    s32 speed;
    s32 v0;
    s32 y;
    s32 x;

    tpage_buf[5] = 1;
    load_image_rect(320, 0, 64, 128, geko_ptc_res.ext_tile);
    geko_loadbuf_init();
    palette_upload_enqueue(geko_ptc_res.pal2, 18, 1, 3, 0);

    yDiv = 84;
    xDiv = 130;
    speed = 4096;

    for (i = 4; i < 14; i++) {
        Sprites[i].f00 = (s32)0xC1000000;

        v0 = rand();
        y = (yDiv != 0) ? (v0 % yDiv) : 0;
        Sprites[i].f04 = (s16)(y - 159);

        v0 = rand();
        x = (xDiv != 0) ? (v0 % xDiv) : 0;

        Sprites[i].f08 = 8;
        Sprites[i].f0A = 24;
        Sprites[i].f0C = 5;
        Sprites[i].f0E = 0;
        Sprites[i].f0F = 0;
        Sprites[i].f10 = 0;
        Sprites[i].f12 = 498;
        Sprites[i].f14 = 0;
        Sprites[i].f15 = 0;
        Sprites[i].f16 = 0;
        Sprites[i].f18 = 0;
        Sprites[i].f1A = 0;
        Sprites[i].f1C = (s16)speed;

        Sprites[i].f06 = (s16)(x - 95);
        Sprites[i].f1E = (s16)speed;
        Sprites[i].f20 = 0;

        sprite_state[i] = 140;

        ((s16 *)&geko_event0_res[9])[i] = 0;
        (&geko_ptc_phase[12])[i] = 0;
    }

    sub_sub_step_up();
}
