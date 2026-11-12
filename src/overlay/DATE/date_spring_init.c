/* Function: date_spring_init */
/* Overlay: DATE */
/* @SymbolSize: 1044 */
/* @CompileFlags: -cckr */
#include "types.h"

typedef union {
    s32 word;
    struct {
        s16 x;
        s16 y;
    } h;
} DatePoint;

typedef struct {
      u8         unk00;
      u8         unk01;
      u8         unk02;
      u8         unk03;
      u8         unk04[3];
      u8         unk07;
      u8         unk08[4];
      s32       *unk0C;
      s32       *unk10;
      u8         unk14[2];
      s16        unk16;
      u8         unk18[2];
      s16        unk1A;
      s16        unk1C;
      u8         unk1E[6];
      DatePoint  unk24;
      u8         unk28[2];
      s16        unk2A;
      u8         unk2C[4];
      s32        unk30;
      s32       *unk34;
      s32        unk38;
      u8         unk3C[7];
      u8         unk43;
} DateObj;

extern s32       *date_spring_res;
extern s32       *date_spring_res_hdr1;
extern s32       *date_spring_res_img0;
extern s32       *date_spring_res_img1;
extern s32       *date_spring_res_anim_tbl0;
extern s32       *date_spring_res_anim_tbl1;
extern s32       *date_spring_res_pal0;
extern s32       *date_spring_res_pal1;
extern DatePoint  date_spring_pos[];
extern u8         date_pattern;
extern u8         now_sub_sub_step;
extern DateObj    class_w[];

extern s32  sub_sub_step_up(void);
extern void palette_upload_enqueue(s32 *a0, s32 a1, s32 a2, u8 a3, s32 a4);
extern void class_clear_n(s32 a0);
extern s32  rand(void);

s32 date_spring_init()
{
    short i;
    short spd;
    s32 rndA;
    s32 rndB;
    u8 k;

    if (date_spring_res[0] != 0x801E4000 ||
        date_spring_res_hdr1[0] != 0x801E8000 ||
        date_spring_res_hdr1[1] != 0x801EC000 ||
        date_spring_res_anim_tbl0[0] != 0x801F0540 ||
        date_spring_res_anim_tbl0[1] != 0x801F0558 ||
        date_spring_res_anim_tbl0[2] != 0x801F0570 ||
        date_spring_res_anim_tbl0[3] != 0x801F058C ||
        date_spring_res_anim_tbl1[0] != 0x801F0B64 ||
        date_spring_res_anim_tbl1[1] != 0x801F0B80) {
        now_sub_sub_step = now_sub_sub_step - 2;
        return 0;
    }
    k = 2;
    if (date_pattern == k) {
        palette_upload_enqueue(date_spring_res_pal0, 18, 1, k, 0);
    } else {
        palette_upload_enqueue(date_spring_res_pal1, 18, 1, k, 0);
    }

    for (i = 0; i < 20; i++) {
        class_clear_n(i + 98);
        class_w[i + 98].unk38 = 0x01000000;
        class_w[i + 98].unk01 = 13;
        class_w[i + 98].unk02 = 1;
        class_w[i + 98].unk03 = 4;
        if (date_pattern == k) {
            class_w[i + 98].unk0C = date_spring_res_img0;
            class_w[i + 98].unk10 = date_spring_res_anim_tbl0;
            class_w[i + 98].unk34 = date_spring_res;
        } else {
            class_w[i + 98].unk0C = date_spring_res_img1;
            class_w[i + 98].unk10 = date_spring_res_anim_tbl1;
            class_w[i + 98].unk34 = date_spring_res_hdr1;
        }
        class_w[i + 98].unk16 = rand() % 8;
        class_w[i + 98].unk43 = 18;
        spd = rand() % 2;
        class_w[i + 98].unk1A = 4096 / (spd * 2 + 2);
        class_w[i + 98].unk1C = 4096 / (spd * 2 + 2);
        class_w[i + 98].unk07 = 128;
        class_w[i + 98].unk24.h.y = rand() % 300 - 150;
        date_spring_pos[i].word = class_w[i + 98].unk24.word;
        rndA = rand();
        rndB = rand();
        date_spring_pos[i].h.y += ((rndB % 2 * 2 - 1) * (rndA % 8) + 8) / (spd + 1);
        class_w[i + 98].unk2A = rand() % 160 - 120;
        class_w[i + 98].unk30 =
            (((date_pattern != k) << 15) + 32768) / (spd + 1);
    }

    return sub_sub_step_up();
}
