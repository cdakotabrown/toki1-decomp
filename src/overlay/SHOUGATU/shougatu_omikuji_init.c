/* Address: 0x80134CB4, Size: 604 bytes (151 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

typedef struct {
    u8 b00;
    u8 b01;
    u8 b02;
    u8 b03;
    u8 b04;
    u8 b05;
    u8 b06;
    u8 b07;
    u8 pad08[4];
    s32 w0C;
    s32 w10;
    s16 h14;
    s16 h16;
    s16 h18;
    u8 pad1A[12];
    s16 h26;
    u8 pad28[2];
    s16 h2A;
    u8 pad2C[8];
    s32 w34;
    s32 w38;
    u8 pad3C[7];
    u8 b43;
} Rec68;

extern void sub_sub_step_up(void);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void class_clear_n(s32);
extern void shougatu_bank_init(void);

extern Rec68 class_w[];

extern s32 shougatu_omikuji_res;
extern s32 shougatu_omikuji_res_hdr1;
extern s32 shougatu_omikuji_res_img0;
extern s32 shougatu_omikuji_res_img1;
extern s32 shougatu_omikuji_res_anim_tbl0;
extern s32 shougatu_omikuji_res_anim_tbl1;
extern s16 shougatu_omikuji_res_aux0;
extern s16 shougatu_omikuji_res_aux1;
extern s32 shougatu_omikuji_res_pal0;
extern s32 shougatu_omikuji_res_pal1;
extern u8 shougatu_omikuji_box_on;

void shougatu_omikuji_init(void)
{
    s16 i;

    shougatu_omikuji_box_on = 0;
    shougatu_bank_init();
    palette_upload_enqueue(shougatu_omikuji_res_pal0, 17, 1, 2, 0);

    for (i = 0; i < 3; i++) {
        class_clear_n(i + 96);
        class_w[96 + i].b01 = 10;
        class_w[96 + i].b02 = 0;
        class_w[96 + i].w38 = 0x01000000;
        class_w[96 + i].b03 = 4;
        class_w[96 + i].w0C = shougatu_omikuji_res_img0;
        class_w[96 + i].w10 = shougatu_omikuji_res_anim_tbl0;
        class_w[96 + i].w34 = shougatu_omikuji_res;
        class_w[96 + i].h14 = shougatu_omikuji_res_aux0;
        class_w[96 + i].h16 = 0;
        class_w[96 + i].h18 = 0;
        class_w[96 + i].b06 = 1;
        class_w[96 + i].b43 = 17;
        class_w[96 + i].h26 = -160;
        class_w[96 + i].h2A = -120;
        class_w[96 + i].b07 = 128;
        class_w[96 + i].b05 = 0;
        class_w[96 + i].b04 = 0;
    }

    (*(Rec68 *)&((CLASS_W *)class_w)[96]).h26 = 0;
    (*(Rec68 *)&((CLASS_W *)class_w)[97]).h16 = 3;
    (*(Rec68 *)&((CLASS_W *)class_w)[98]).h16 = 4;

    palette_upload_enqueue(shougatu_omikuji_res_pal1, 18, 0, 0, 2);
    class_clear_n(99);

    (*(Rec68 *)&((CLASS_W *)class_w)[99]).b01 = 13;
    (*(Rec68 *)&((CLASS_W *)class_w)[99]).b02 = 0;
    (*(Rec68 *)&((CLASS_W *)class_w)[99]).w38 = 0;
    (*(Rec68 *)&((CLASS_W *)class_w)[99]).b03 = 4;
    (*(Rec68 *)&((CLASS_W *)class_w)[99]).w0C = shougatu_omikuji_res_img1;
    (*(Rec68 *)&((CLASS_W *)class_w)[99]).w10 = shougatu_omikuji_res_anim_tbl1;
    (*(Rec68 *)&((CLASS_W *)class_w)[99]).w34 = shougatu_omikuji_res_hdr1;
    (*(Rec68 *)&((CLASS_W *)class_w)[99]).h14 = shougatu_omikuji_res_aux1;
    (*(Rec68 *)&((CLASS_W *)class_w)[99]).h16 = 0;
    (*(Rec68 *)&((CLASS_W *)class_w)[99]).h18 = 0;
    (*(Rec68 *)&((CLASS_W *)class_w)[99]).b06 = 1;
    (*(Rec68 *)&((CLASS_W *)class_w)[99]).b43 = 0;
    (*(Rec68 *)&((CLASS_W *)class_w)[99]).h26 = -160;
    (*(Rec68 *)&((CLASS_W *)class_w)[99]).h2A = -120;
    (*(Rec68 *)&((CLASS_W *)class_w)[99]).b07 = 128;
    (*(Rec68 *)&((CLASS_W *)class_w)[99]).b05 = 129;
    (*(Rec68 *)&((CLASS_W *)class_w)[99]).b04 = 0;

    sub_sub_step_up();
}
