/* @PostProc: patch_insn_reg_field 0x1a4:rd:v1:v0 0x1a8:rt:v1:v0 0x1bc:rt:v1:v0 */
/* geko_particle_init -- GEKO overlay, Address: 0x8013A7D0, Size: 508 bytes (127 insns) */
#include "types.h"
#include "tokimeki/date_obj_res_ex.h"

extern void geko_event0_ptr_set1(void);
extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void date_sprite_init(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void date_object_set(s32 a0, s32 a1, s32 a2, s32 a3);
extern s32  rand(void);
extern void sub_sub_step_up(void);

typedef struct {
    s32 f0;
    s16 f4;
    s16 f6;
    s16 f8;
    s16 f10;
    s16 f12;
    u8  f14;
    u8  f15;
    s16 f16;
    s16 f18;
    u8  f20;
    u8  f21;
    u8  f22;
    s16 f24;
    s16 f26;
    s16 f28;
    s16 f30;
    s32 f32;
} Ent;

extern Ent Sprites[];
extern u8  sprite_state[];

extern s16 geko_ptc_phase[];
extern s32 geko_event0_res[];
extern GEKO_OBJ_RES_EX geko_ptc_res;

void geko_particle_init(void)
{
    s16 i;
    Ent *e;

    geko_event0_ptr_set1();
    palette_upload_enqueue(geko_ptc_res.pal, 17, 1, 2, 0);
    date_sprite_init(geko_ptc_res.tile[0], geko_ptc_res.tile[1], geko_ptc_res.tile[2], geko_ptc_res.tile[3],
                  geko_ptc_res.tile[4], geko_ptc_res.tile[5]);
    date_object_set(geko_ptc_res.img, geko_ptc_res.anim_tbl, geko_ptc_res.hdr, (s32)geko_ptc_res.aux);
    palette_upload_enqueue(geko_ptc_res.pal2, 18, 1, 3, 0);

    for (i = 4; i < 19; i++)
    {
        e = &Sprites[i];
        e->f0 = 0xC1000000;
        e->f4 = (s16)(rand() % 320 - 159);
        e->f6 = -120;
        e->f8 = 8;
        e->f10 = 8;
        e->f12 = 5;
        e->f14 = 0;
        e->f15 = 0;
        e->f16 = 0;
        e->f18 = 498;
        e->f20 = 0;
        e->f21 = 0;
        e->f22 = 0;
        e->f24 = 0;
        e->f26 = 0;
        e->f28 = 4096;
        e->f30 = 4096;
        e->f32 = 0;
        sprite_state[i] = 12;
        ((s16 *)&geko_event0_res[9])[i] = 0;
        (&geko_ptc_phase[12])[i] = 0;
    }

    sub_sub_step_up();
}
