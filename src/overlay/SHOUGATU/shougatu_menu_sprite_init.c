/* @CompileFlags: -Wc,-pic0 */
/* Address: 0x8013F570 (true start 0x8013F564: the three u8 flag clears at */
/* Size: 1328 bytes (332 instructions) including the absorbed head. */
/* @SymbolStart: 0x8013F564 */
/* @SymbolSize: 1328 */
/* @CompileEnv: IDO52_AS1_TIE_REJECT=1343,1340 */
/* @PostProc: swap_instructions --offset1 0x2b8 --offset2 0x2c4 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void sub_sub_step_up(void);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void class_clear_n(s32);
extern s32 rand(void);
extern void shougatu_bank_init2(void);
extern void shougatu_bank_init2b(void);

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


extern s32 shougatu_menu_ss_serifu;
extern s32 shougatu_menu_ss_serifu_kind;
extern s32 shougatu_menu_sss_sound;
extern s16 shougatu_menu_now_serifu;
extern s32 shougatu_menu_res[];
extern s32 shougatu_menu_sub_res[];
extern u8 shougatu_menu_sprite_flag;
extern u8 shougatu_pair_disp_on;
extern u8 shougatu_phase_on;
extern CLASS_W class_w[];
extern s32 shougatu_menu_serifu_tbl[];
extern s32 shougatu_menu_serifu_kind_tbl[];
extern s32 shougatu_menu_sound_tbl[];

void shougatu_menu_sprite_init(void)
{
    s32 padA[2];
    Rec68 b;
    Rec68 a;
    s32 padB[2];
    s16 i;

    shougatu_menu_sprite_flag = 0;
    shougatu_pair_disp_on = 0;
    shougatu_phase_on = 0;
    shougatu_bank_init2();

    for (i = 0; i < 9; i++) {
        palette_upload_enqueue(shougatu_menu_res[4], (u8)(i + 16), 1, 2, i);
    }
    class_clear_n(96);

    (*(Rec68 *)&class_w[96]).b01 = 8;
    (*(Rec68 *)&class_w[96]).b02 = 64;
    (*(Rec68 *)&class_w[96]).w38 = 0x01000000;
    (*(Rec68 *)&class_w[96]).b03 = 4;
    (*(Rec68 *)&class_w[96]).w0C = shougatu_menu_res[1];
    (*(Rec68 *)&class_w[96]).w10 = shougatu_menu_res[2];
    (*(Rec68 *)&class_w[96]).w34 = shougatu_menu_res[0];
    (*(Rec68 *)&class_w[96]).h14 = (*(s16 *)&shougatu_menu_res[3]);
    (*(Rec68 *)&class_w[96]).h16 = 0;
    (*(Rec68 *)&class_w[96]).h18 = 0;
    (*(Rec68 *)&class_w[96]).b06 = 1;
    (*(Rec68 *)&class_w[96]).b43 = 0;
    (*(Rec68 *)&class_w[96]).h26 = 0;
    (*(Rec68 *)&class_w[96]).h2A = 8;
    (*(Rec68 *)&class_w[96]).b07 = 128;
    (*(Rec68 *)&class_w[96]).b05 = 128;
    (*(Rec68 *)&class_w[96]).b04 = 0;

    (*(Rec68 *)&class_w[97]) = (*(Rec68 *)&class_w[96]);
    shougatu_bank_init2b();
    palette_upload_enqueue(shougatu_menu_sub_res[4], 31, 1, 2, 0);
    (*(Rec68 *)&class_w[98]) = (*(Rec68 *)&class_w[96]);

    (*(Rec68 *)&class_w[98]).b01 = 11;
    (*(Rec68 *)&class_w[98]).b04 = 8;
    (*(Rec68 *)&class_w[98]).b02 = 1;
    (*(Rec68 *)&class_w[98]).w0C = shougatu_menu_sub_res[1];
    (*(Rec68 *)&class_w[98]).w10 = shougatu_menu_sub_res[2];
    (*(Rec68 *)&class_w[98]).w34 = shougatu_menu_sub_res[0];
    (*(Rec68 *)&class_w[98]).h14 = (*(s16 *)&shougatu_menu_sub_res[3]);

    (*(Rec68 *)&class_w[98]).h16 = rand() % 3;
    (*(Rec68 *)&class_w[98]).h18 = 0;
    (*(Rec68 *)&class_w[98]).h26 = -160;
    (*(Rec68 *)&class_w[98]).h2A = -120;
    (*(Rec68 *)&class_w[98]).b43 = 31;

    a = (*(Rec68 *)&class_w[98]);
    ((Rec68 *)&class_w[99].active)[1] = a;
    b = a;
    ((Rec68 *)&class_w[99].active)[0] = b;
    (*(Rec68 *)&class_w[101]) = b;

    ((Rec68 *)&class_w[99].active)[0].h26 = ((Rec68 *)&class_w[99].active)[0].h26 + 72;
    ((Rec68 *)&class_w[99].active)[0].h16 = rand() % 3;
    ((Rec68 *)&class_w[99].active)[0].h18 = 4;
    ((Rec68 *)&class_w[99].active)[1].h26 = ((Rec68 *)&class_w[99].active)[1].h26 + 144;
    ((Rec68 *)&class_w[99].active)[1].h16 = rand() % 3;
    ((Rec68 *)&class_w[99].active)[1].h18 = 10;
    (*(Rec68 *)&class_w[101]).b01 = 10;
    (*(Rec68 *)&class_w[101]).b02 = 64;
    (*(Rec68 *)&class_w[101]).h16 = 0;
    shougatu_menu_now_serifu = 0;
    shougatu_menu_ss_serifu = shougatu_menu_serifu_tbl[10];
    shougatu_menu_ss_serifu_kind = shougatu_menu_serifu_kind_tbl[10];
    shougatu_menu_sss_sound = shougatu_menu_sound_tbl[10];
    sub_sub_step_up();
}
