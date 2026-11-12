/* Function: bustup_sprite_init */
/* Address: 0x80062F5C */
/* Size: 1856 bytes (464 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_func_80062F5C.py */
#include "types.h"

extern u8 bustup2_cw_prio;
extern u8 bustup2_cw_mode;
extern u8 bustup2_cw_disp;
extern u8 bustup2_cw_rate;
extern u8 bustup2_cw_bright;
extern s32 bustup2_cw_img;
extern s32 bustup2_cw_anim_tbl;
extern s16 bustup2_cw_anim;
extern s16 bustup2_cw_x;
extern s16 bustup2_cw_y;
extern s32 bustup2_cw_hdr;
extern u32 bustup2_cw_attr;
extern u8 bustup2_cw_clut;

extern u8 bustup_cw_prio;
extern u8 bustup_cw_mode;
extern u8 bustup_cw_disp;
extern u8 bustup_cw_rate;
extern u8 bustup_cw_bright;
extern s32 bustup_cw_img;
extern s32 bustup_cw_anim_tbl;
extern s16 bustup_cw_anim;
extern s16 bustup_cw_y;
extern s32 bustup_cw_hdr;
extern u32 bustup_cw_attr;
extern u8 bustup_cw_clut;

extern u32 Sprites;
extern s16 sprite0_x;
extern s16 D_80129CB6;
extern s16 D_80129CB8;
extern s16 D_80129CBA;
extern u8 D_80129CBE;
extern u8 D_80129CBF;
extern s16 D_80129CC0;
extern s16 D_80129CC2;
extern u8 sprite0_r;
extern u8 sprite0_g;
extern u8 sprite0_b;
extern s16 D_80129CC8;
extern s16 D_80129CCA;
extern s16 D_80129CCC;
extern s16 D_80129CCE;
extern s32 D_80129CD0;

extern u32 sprite1_attribute;
extern s16 D_80129CD8;
extern s16 sprite1_y;
extern s16 D_80129CDC;
extern s16 D_80129CDE;
extern u8 D_80129CE2;
extern u8 D_80129CE3;
extern s16 D_80129CE4;
extern s16 D_80129CE6;
extern u8 sprite1_r;
extern u8 sprite1_g;
extern u8 sprite1_b;
extern s16 D_80129CEC;
extern s16 D_80129CEE;
extern s16 D_80129CF0;
extern s16 D_80129CF2;
extern s32 D_80129CF4;

extern u32 sprite2_attribute;
extern s16 sprite2_x;
extern s16 D_80129CFE;
extern s16 D_80129D00;
extern s16 D_80129D02;
extern u8 D_80129D06;
extern u8 D_80129D07;
extern s16 D_80129D08;
extern s16 D_80129D0A;
extern u8 sprite2_r;
extern u8 sprite2_g;
extern u8 sprite2_b;
extern s16 D_80129D10;
extern s16 D_80129D12;
extern s16 D_80129D14;
extern s16 D_80129D16;
extern s32 D_80129D18;

extern u32 sprite3_attribute;
extern s16 D_80129D20;
extern s16 D_80129D22;
extern s16 D_80129D24;
extern s16 D_80129D26;
extern u8 D_80129D2A;
extern u8 D_80129D2B;
extern s16 D_80129D2C;
extern s16 D_80129D2E;
extern u8 D_80129D30;
extern u8 D_80129D31;
extern u8 D_80129D32;
extern s16 D_80129D34;
extern s16 D_80129D36;
extern s16 D_80129D38;
extern s16 D_80129D3A;
extern s32 D_80129D3C;

extern u32 D_80129D40;
extern s16 D_80129D44;
extern s16 sprite4_y;
extern s16 D_80129D48;
extern s16 sprite4_h;
extern u8 sprite4_u;
extern u8 sprite4_v;
extern s16 D_80129D50;
extern s16 D_80129D52;
extern u8 sprite4_r;
extern u8 sprite4_g;
extern u8 sprite4_b;
extern s16 D_80129D58;
extern s16 D_80129D5A;
extern s16 D_80129D5C;
extern s16 D_80129D5E;
extern s32 D_80129D60;

extern u32 D_80129D64;
extern s16 D_80129D68;
extern s16 sprite5_y;
extern s16 D_80129D6C;
extern s16 sprite5_h;
extern u8 sprite5_u;
extern u8 sprite5_v;
extern s16 D_80129D74;
extern s16 D_80129D76;
extern u8 sprite5_r;
extern u8 sprite5_g;
extern u8 sprite5_b;
extern s16 D_80129D7C;
extern s16 D_80129D7E;
extern s16 D_80129D80;
extern s16 D_80129D82;
extern s32 D_80129D84;

extern u8 sprite_state;
extern u8 sprite_state1;
extern u8 sprite_state2;
extern u8 sprite_state3;
extern u8 sprite_state4;
extern u8 sprite_state5;

extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void load_tpage_buf_lock(s32, s32, s32);
extern void class_clear_n(s32);
extern void bustup_face_set(s32);
extern void bustup_timer_clamp(s32);

void bustup_sprite_init(s32 a0, s32 *a1, volatile s32 a2, volatile s32 a3, s32 a4) {
    s16 *v0;
    s16 a3val;
    s16 t3val, t4val;
    u32 tagC1;
    u32 tag81;
    s32 local16;

    palette_upload_enqueue(a0, 16, 1, 2, 0);

    load_tpage_buf_lock(5, a1[0], 1);
    load_tpage_buf_lock(6, a1[1], 1);
    load_tpage_buf_lock(7, a1[2], 1);
    load_tpage_buf_lock(8, a1[3], 1);
    load_tpage_buf_lock(9, a1[4], 1);

    class_clear_n(31);
    bustup2_cw_prio = 12;
    bustup2_cw_mode = 0;
    bustup2_cw_attr = 0xC1000000;
    bustup2_cw_disp = 140;
    bustup2_cw_img = a2;
    bustup2_cw_anim_tbl = a3;
    bustup2_cw_hdr = (s32)a1;
    bustup2_cw_anim = 0;
    bustup2_cw_rate = 2;
    bustup2_cw_clut = 16;
    bustup2_cw_y = -32;
    bustup2_cw_bright = 0;

    class_clear_n(30);
    bustup_cw_prio = 12;
    bustup_cw_mode = 0;
    tagC1 = 0xC1000000;
    bustup_cw_attr = tagC1;
    bustup_cw_disp = 140;
    bustup_cw_img = a2;
    bustup_cw_anim_tbl = a3;
    bustup_cw_hdr = (s32)a1;
    bustup_cw_anim = 1;
    bustup_cw_rate = 2;
    bustup_cw_clut = 16;
    bustup_cw_y = -32;
    bustup_cw_bright = 0;

    v0 = *(s16 **)a2;
    a3val = v0[0];

    if (a3val >= 6) {
        t3val = v0[1];
        t4val = v0[2];
        tag81 = 0x81000000;

        Sprites = tag81;
        D_80129CB8 = 16;
        D_80129CBA = 16;
        D_80129CBE = 48;
        D_80129CBF = 0;
        D_80129CC0 = 0;
        D_80129CC2 = 496;
        sprite0_r = 128;
        sprite0_g = 128;
        sprite0_b = 128;
        D_80129CC8 = 0;
        D_80129CCA = 0;
        D_80129CCC = 4096;
        D_80129CCE = 4096;
        D_80129CD0 = 0;
        sprite0_x = bustup2_cw_x + v0[39] - t3val;
        D_80129CB6 = bustup2_cw_y + v0[40] - t4val;
        sprite_state = 11;

        sprite1_attribute = tag81;
        D_80129CDC = 48;
        D_80129CDE = 32;
        D_80129CE2 = 0;
        D_80129CE3 = 0;
        D_80129CE4 = 0;
        D_80129CE6 = 496;
        sprite1_r = 128;
        sprite1_g = 128;
        sprite1_b = 128;
        D_80129CEC = 0;
        D_80129CEE = 0;
        D_80129CF0 = 4096;
        D_80129CF2 = 4096;
        D_80129CF4 = 0;
        D_80129CD8 = bustup2_cw_x + v0[32] - t3val;
        sprite1_y = bustup2_cw_y + v0[33] - t4val;
        sprite_state1 = 11;
    }

    a3val = v0[0];
    t3val = v0[1];
    t4val = v0[2];

    if (a3val >= 4) {
        sprite2_attribute = tagC1;
        D_80129D00 = 16;
        D_80129D02 = v0[23];
        D_80129D06 = (u8)v0[24];
        D_80129D07 = 0;
        D_80129D08 = 0;
        D_80129D0A = 496;
        sprite2_r = 128;
        sprite2_g = 128;
        sprite2_b = 128;
        D_80129D10 = 0;
        D_80129D12 = 0;
        D_80129D14 = 4096;
        D_80129D16 = 4096;
        D_80129D18 = 0;
        sprite2_x = bustup2_cw_x + v0[18] - t3val;
        D_80129CFE = bustup2_cw_y + v0[19] - t4val;
        sprite_state2 = 139;

        sprite3_attribute = tagC1;
        D_80129D24 = v0[30];
        D_80129D26 = v0[31];
        D_80129D2A = 0;
        D_80129D2B = 0;
        D_80129D2C = 0;
        D_80129D2E = 496;
        D_80129D30 = 128;
        D_80129D31 = 128;
        D_80129D32 = 128;
        D_80129D34 = 0;
        D_80129D36 = 0;
        D_80129D38 = 4096;
        D_80129D3A = 4096;
        D_80129D3C = 0;
        D_80129D20 = bustup2_cw_x + v0[25] - t3val;
        D_80129D22 = bustup2_cw_y + v0[26] - t4val;
        sprite_state3 = 139;
    }

    a3val = v0[0];

    if (a3val >= 7) {
        D_80129D40 = tagC1;
        D_80129D48 = 16;
        sprite4_h = v0[51];
        sprite4_u = (u8)v0[52];
        sprite4_v = 0;
        D_80129D50 = 0;
        D_80129D52 = 496;
        sprite4_r = 128;
        sprite4_g = 128;
        sprite4_b = 128;
        D_80129D58 = 0;
        D_80129D5A = 0;
        D_80129D5C = 4096;
        D_80129D5E = 4096;
        D_80129D60 = 0;
        D_80129D44 = bustup2_cw_x + v0[46] - t3val;
        sprite4_y = bustup2_cw_y + v0[47] - t4val;
        sprite_state4 = 139;
    }

    a3val = v0[0];

    if (a3val == 8) {
        D_80129D64 = tagC1;
        D_80129D6C = v0[58];
        sprite5_h = v0[59];
        sprite5_u = 0;
        sprite5_v = 0;
        D_80129D74 = 0;
        D_80129D76 = 496;
        sprite5_r = 128;
        sprite5_g = 128;
        sprite5_b = 128;
        D_80129D7C = 0;
        D_80129D7E = 0;
        D_80129D80 = 4096;
        D_80129D82 = 4096;
        D_80129D84 = 0;
        D_80129D68 = bustup2_cw_x + v0[53] - t3val;
        sprite5_y = bustup2_cw_y + v0[54] - t4val;
        sprite_state5 = 139;
    }

    bustup_face_set(a4);
    bustup_timer_clamp(a4);
}
