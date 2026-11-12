/* @SymbolStart: 0x80155708 */
/* @SymbolSize: 672 */
/* Function: date_girl5_init4 */
/* Overlay: DATE */
/* Address: 0x80155740 (true entry 0x80155708) */
/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: patch_insn_reg_field 0x134:rt:v0:v1 0x13c:rt:v0:v1 0x14c:rt:v0:v1 */
#include "types.h"

typedef struct {
      u8  unk00;
      u8  unk01;
      u8  unk02;
      u8  unk03;
      u8  unk04;
      u8  unk05;
      u8  unk06;
      u8  unk07;
      s32 unk08;
      s32 unk0C;
      s32 unk10;
      s16 unk14;
      s16 unk16;
      s16 unk18;
      s16 unk1A;
      s32 unk1C;
      s32 unk20;
      s16 unk24;
      s16 unk26;
      s16 unk28;
      s16 unk2A;
      s32 unk2C;
      s32 unk30;
      s32 unk34;
      s32 unk38;
      s32 unk3C;
      u8  unk40;
      u8  unk41;
      u8  unk42;
      u8  unk43;
} DateEnt;

extern DateEnt class_w[];

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;

extern s16 girl5_tokimeki;
extern s16 girl5_yuukou;
extern s16 girl5_syoushin;

extern s32 date_girl5_init4_res[];

extern u8  date_fall_st[];
extern s16 date_fall_x_tbl[];
extern s16 date_fall_y_tbl[];
extern u8  date_fall_wait_cnt[];
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

extern void sub_sub_step_up(void);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void param_clamp(void);
extern void date_sprite_init(s32, s32, s32, s32, s32, s32);
extern void date_object_set(s32, s32, s32, s32);
extern s32  rand(void);
extern void date_girl5_mode4_bufptr_set(void);

void date_girl5_init4(void)
{
    s16 i;

    date_ss_serifuev = date_serifuev_tbl[45];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[45];
    date_sss_soundev = date_soundev_tbl[45];

    girl5_tokimeki = girl5_tokimeki + 2;
    girl5_yuukou = girl5_yuukou + 1;
    girl5_syoushin = girl5_syoushin - 20;

    param_clamp();
    date_girl5_mode4_bufptr_set();

    palette_upload_enqueue(date_girl5_init4_res[4], 17, 1, 2, 0);
    date_sprite_init(date_girl5_init4_res[5], date_girl5_init4_res[6], date_girl5_init4_res[7], date_girl5_init4_res[8],
                  date_girl5_init4_res[9], date_girl5_init4_res[10]);
    date_object_set(date_girl5_init4_res[1], date_girl5_init4_res[2], date_girl5_init4_res[0], (*(s16 *)&date_girl5_init4_res[3]));

    class_w[96].unk04 = class_w[97].unk04 = 8;

    for (i = 0; i < 8; i++) {
        date_fall_st[i] = 0;
    }

    for (i = 0; i < 8; i++) {
        class_w[i + 98] = class_w[96];
        class_w[i + 98].unk16 = 5;
        class_w[i + 98].unk26 = date_fall_x_tbl[i] - 160;
        class_w[i + 98].unk2A = date_fall_y_tbl[i] - 120;
        date_fall_wait_cnt[i] = rand() % 120;
    }

    class_w[98].unk16 = 4;
    sub_sub_step_up();
}
