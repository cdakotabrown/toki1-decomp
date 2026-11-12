/* @CompileFlags: -Wc,-pic0 -Wo,-loopunroll,0 */
/* @SymbolSize: 820 */
/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=1 IDO52_FORCE_WEB_COLOR=16:s7 */
/* @PostProc: swap_instructions --offset1 0x15c --offset2 0x160 */
/* @PostProc: swap_instructions --offset1 0x158 --offset2 0x15c */
/* @PostProc: swap_instructions --offset1 0x174 --offset2 0x178 */
/* Address: 0x801464D4 */
/* Size: 820 bytes (205 insns) */
#include "types.h"

typedef struct {
    s16 h[12];
} Tbl12;

extern void class_clear_n(s32);

extern Tbl12 taiiku_sprite_init3_tbl0;
extern Tbl12 taiiku_sprite_init3_tbl1;
extern s32   taiiku_spr_img_9;
extern s32   taiiku_spr_anim_9;
extern s32   taiiku_spoon_sprite_tpg;
extern u8    class_w[];
extern volatile s16 cal_cw_x;
extern volatile s16 cal_cw_y;
extern volatile s16 cw97_x;
extern volatile s16 cw97_y;
extern volatile s16 cw98_x;
extern volatile s16 cw98_y;
extern volatile s16 cw99_x;
extern volatile s16 cw99_y;
extern volatile u8  cw100_prio;
extern volatile u8  cw100_anim_flag;
extern volatile u8  cw100_disp;
extern volatile s16 cw100_x;
extern volatile s16 cw100_y;
extern volatile u8  cw101_prio;
extern volatile u8  cw101_anim_flag;
extern volatile u8  cw101_disp;
extern volatile s16 cw101_x;
extern volatile s16 cw101_y;
extern volatile u8  cw102_prio;
extern volatile u8  cw102_anim_flag;
extern volatile u8  cw102_disp;
extern volatile s16 cw102_x;
extern volatile s16 cw102_y;
extern volatile u8  cw103_prio;
extern volatile u8  cw103_anim_flag;
extern volatile u8  cw103_disp;
extern volatile s16 cw103_x;
extern volatile s16 cw103_y;
extern volatile u8  cw104_prio;
extern volatile u8  cw105_prio;
extern volatile u8  cw106_prio;
extern volatile u8  cw107_prio;
extern volatile u8  cw107_anim_flag;
extern volatile u8  cw107_disp;
extern volatile s16 cw107_x;

typedef struct {
    u8  pad0;
    u8  f1;
    u8  f2;
    u8  f3;
    u8  f4;
    u8  f5;
    u8  f6;
    u8  f7;
    u8  pad8[4];
    s32 f12;
    s32 f16;
    s16 f20;
    s16 pad22;
    s16 f24;
    s16 f26;
    s16 f28;
    u8  pad30[8];
    s16 f38;
    s16 pad40;
    s16 f42;
    u8  pad44[8];
    s32 f52;
    s32 f56;
    u8  pad60[8];
} TkRec;
extern TkRec class_w_r[];
extern s32 taiiku_spr_aux[];

void taiiku_sprite_init3(void)
{
    s32 pad;
    Tbl12 tblA;
    Tbl12 tblB;
    s32 i;
    s32 n;
    s32 y;

    tblA = taiiku_sprite_init3_tbl0;
    tblB = taiiku_sprite_init3_tbl1;

    for (i = 0; i < 12; i++) {
        class_clear_n(i + 96);
        class_w_r[i + 96].f56 = 0x01000000;
        class_w_r[i + 96].f7 = 128;
        class_w_r[i + 96].f12 = taiiku_spr_img_9;
        class_w_r[i + 96].f16 = taiiku_spr_anim_9;
        class_w_r[i + 96].f52 = (s32)&taiiku_spoon_sprite_tpg;
        class_w_r[i + 96].f20 = (*(s16 *)&taiiku_spr_aux[9]);
        class_w_r[i + 96].f42 = tblA.h[i];
        class_w_r[i + 96].f24 = tblB.h[i];
        class_w_r[i + 96].f4 = 0;
        class_w_r[i + 96].f5 = 128;
        class_w_r[i + 96].f26 = 4096;
        class_w_r[i + 96].f28 = 4096;
        class_w_r[i + 96].f6 = 2;
    }

    n = 3;
    y = -160;
    i = 0;
    do {
        class_w_r[i + 96].f38 = y;
        y += 8;
        class_w_r[i + 96].f1 = n;
        n += 2;
        i++;
        class_w_r[i + 95].f2 = 64;
        class_w_r[i + 95].f3 = 164;
    } while (y < -128);

    cw100_prio = 2;
    cw100_x = cal_cw_x + 55;
    cw100_y = cal_cw_y + 28;
    cw100_anim_flag = 64;
    cw100_disp = 164;

    cw101_prio = 4;
    cw101_x = cw97_x + 55;
    cw101_y = cw97_y + 28;
    cw101_anim_flag = 64;
    cw101_disp = 164;

    cw102_prio = 6;
    cw102_x = cw98_x + 55;
    cw102_y = cw98_y + 28;
    cw102_anim_flag = 64;
    cw102_disp = 164;

    cw103_prio = 8;
    cw103_x = cw99_x + 55;
    cw103_y = cw99_y + 28;
    cw103_anim_flag = 64;
    cw103_disp = 164;

    cw104_prio = 9;
    cw105_prio = 8;
    cw106_prio = 9;
    cw107_prio = 9;
    cw107_x = -48;
    cw107_anim_flag = 64;
    cw107_disp = 164;
}
