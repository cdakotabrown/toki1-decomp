/* Function: date_effect_init */
/* Overlay: DATE */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=128:2:2 IDO52_EXCLUDE_REGS_WEB=148:s0+s1+s2+s3+s4+s5+s6+s7+s8 IDO52_CONST_DEF_BB=162:30:+ IDO52_UGEN_TEMP_REG=47:t8 */
#include "types.h"
#include "tokimeki/sprite.h"

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

typedef struct {
      s32 unk00;
      s32 unk04;
      s32 unk08;
      s32 unk0C;
      s16 unk10;
      s16 unk12;
      u8  unk14;
      u8  unk15;
      u8  unk16;
      u8  unk17;
      s32 unk18;
      s32 unk1C;
      s32 unk20;
} DateSlot;

typedef struct {
      u8  unk0000[4261];
      u8  flags[387];
      s32 vals[64];
} DateWork;

extern DateWork screen_w;
extern DateEnt  class_w[];
extern DateSlot Sprites[];

extern u8  bg_change_tbl1;
extern s32 date_effect_init_tbl0;
extern s32 date_effect_init_tbl1;
extern u8  update_anim_state_tbl4;
extern s32 bustup_check_init_tbl3;
extern s32 frame_counter;
extern u8  bustup2_cw_clut;
extern u8  cal_cw_disp;
extern s32 date_effect_on;
extern s32 date_effect_res[];
extern s32 date_effect_ht[];
extern s32 date_effect_cnt;
extern s16 date_effect_num;
extern u8  date_effect_bri;

extern void sub_sub_step_up(void);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void class_clear_n(s32);
extern void date_sprite_init(s32, s32, s32, s32, s32, s32);
extern s32  rand(void);
extern void srand(s32);
extern void date_buf_tbl_init(void);

void date_effect_init(void)
{
    s16 i;

    if (bg_change_tbl1 == 1) {
        for (i = 11; i < 16; i++) {
            screen_w.vals[i] = -1;
            screen_w.vals[i + 32] = -1;
        }
        date_effect_init_tbl0 = 0;
    } else {
        for (i = 21; i < 26; i++) {
            screen_w.vals[i] = -1;
            screen_w.vals[i + 16] = -1;
        }
        date_effect_init_tbl1 = 0;
    }

    update_anim_state_tbl4 = 13;
    date_effect_bri = 128;
    date_effect_on = 1;
    date_effect_cnt = 0;
    date_effect_num = 1;
    date_buf_tbl_init();

    palette_upload_enqueue(bustup_check_init_tbl3, 15, 1, 2, 0);
    bustup2_cw_clut = 15;
    ((SPRITE_REC *)Sprites)[0].cy = 495;
    ((SPRITE_REC *)Sprites)[1].cy = 495;

    for (i = 16; i < 32; i++) {
        palette_upload_enqueue(date_effect_res[4], i & 0xFF, 1, 0, i - 16);
    }

    palette_upload_enqueue(date_effect_res[5], 14, 1, 0, 0);
    palette_upload_enqueue(date_effect_res[6], 13, 1, 0, 0);
    srand(frame_counter);

    for (i = 0; i < 10; i++) {
        class_clear_n(i + 96);
        class_w[i + 96].unk01 = 13;
        class_w[i + 96].unk02 = 1;
        class_w[i + 96].unk38 = 0x01000000;
        class_w[i + 96].unk03 = 4;
        class_w[i + 96].unk0C = date_effect_res[1];
        class_w[i + 96].unk10 = date_effect_res[2];
        class_w[i + 96].unk34 = date_effect_res[0];
        class_w[i + 96].unk14 = (*(s16 *)&date_effect_res[3]);
        class_w[i + 96].unk16 = 3;
        class_w[i + 96].unk18 = 0;
        class_w[i + 96].unk06 = 1;
        class_w[i + 96].unk43 = 0;
        class_w[i + 96].unk26 = rand() % 256 - 128;
        class_w[i + 96].unk2A = 160;
        class_w[i + 96].unk07 = 0;
        class_w[i + 96].unk05 = 128;
        class_w[i + 96].unk04 = 8;
        class_w[i + 96].unk2C = rand() % 32768 - 16384;
        date_effect_ht[i] = ((-(rand() % 9)) << 16) - 0x280000;
    }

    cal_cw_disp |= 0x80;
    date_sprite_init(date_effect_res[7], date_effect_res[8], date_effect_res[9], date_effect_res[10], date_effect_res[11],
                  date_effect_res[12]);

    for (i = 58; i < 64; i++) {
        Sprites[i].unk00 &= 0x7FFFFFFF;
        Sprites[i].unk12 = 494;
        screen_w.flags[i] = 12;
    }

    for (i = 58; i < 64; i++) {
        Sprites[i].unk16 = 128;
        Sprites[i].unk15 = 128;
        Sprites[i].unk14 = 128;
    }

    sub_sub_step_up();
}
