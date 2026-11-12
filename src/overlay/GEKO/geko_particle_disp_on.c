/* geko_particle_disp_on -- GEKO overlay, Address: 0x8013FEC4, Size: 228 bytes (57 insns) */
#include "types.h"

extern void mes_window_switch(s32 a0);
extern void date_sprite_switch(s32 a0);
extern void sprite_rgb_set(s32 a0, s32 a1);
extern void sub_sub_step_up(void);

typedef struct {
    u8 pad_00[68];
} Elem68;

extern Elem68 class_w[];

extern s8  update_anim_state_tbl4;
extern u8  cal_cw_bright;
extern u8  cw97_bright;
extern s32 date_sprite_on;
extern s8  geko_fill_on;
extern s32 geko_ptc_cnt;

void geko_particle_disp_on(void)
{
    s32 i;
    u8 *p;

    update_anim_state_tbl4 = 10;
    mes_window_switch(0);
    date_sprite_switch(1);
    date_sprite_on = 1;
    cal_cw_bright = 128;
    cw97_bright = 128;

    for (i = 58; i < 64; i = (s16)(i + 1)) {
        sprite_rgb_set((s16)i, 128);
    }

    geko_fill_on = 0;

    for (i = 0; i < 10; i = (s16)(i + 1)) {
        p = (u8 *)&class_w[i];
        p[6735] |= 0x80;
    }

    geko_ptc_cnt = 1;
    sub_sub_step_up();
}
