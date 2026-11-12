/* Function: config_screen_init */
/* Address: 0x8006A378 */
/* Size: 3064 bytes (766 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x934 --offset2 0x93C */
/* @PostProc: swap_instructions --offset1 0x984 --offset2 0x98C */
/* @PostProc: swap_instructions --offset1 0xB08 --offset2 0xB0C */
/* @PostProc: swap_instructions --offset1 0xB38 --offset2 0xB3C */
/* @PostProc: patch_insn_reg_field 0x8c0:rt:t2:t4 0x8f0:rt:t2:t4 0x900:rt:t2:t4 0x908:rs:t2:t4 0x914:rt:t6:t5 0x91c:rt:a0:t2 0x920:rs:t6:t5 0x920:rt:t7:t6 0x924:rt:a1:t5 0x928:rt:t6:t8 0x92c:rt:a2:a0 0x930:rt:t5:t7 0x934:rs:a0:t2 0x938:rt:t7:t6 0x93c:rt:t5:t7 0x944:rt:t5:t7 0x948:rt:t8:t2 0x94c:rs:a1:t5 0x94c:rt:t4:t6 0x950:rs:t6:t8 0x950:rt:t7:t9 0x95c:rt:a2:a0 0x960:rt:t4:t6 0x964:rt:t5:t7 0x968:rt:a2:a0 0x96c:rt:t7:t9 0x970:rt:t8:t2 0x974:rt:a2:a0 0x978:rt:t9:t3 0x97c:rs:t9:t3 0x97c:rt:t9:t3 0x990:rd:t3:t5 0x994:rt:t3:t5 0x994:rd:t4:t6 0x998:rs:t9:t3 0x998:rt:t4:t6 0x998:rd:t5:t7 0x99c:rs:t5:t7 0x9b8:rt:t7:t9 0x9bc:rs:t7:t9 0x9c0:rt:t6:t8 0x9c4:rs:t6:t8 0x9c4:rt:t6:t8 0x9d4:rd:t8:t2 0x9d8:rt:t8:t2 0x9d8:rd:t2:t4 0x9dc:rs:t6:t8 0x9dc:rt:t2:t4 0x9dc:rd:t3:t5 0x9e0:rs:t3:t5 0x9f4:rt:t4:t6 0x9f8:rs:t4:t6 0x9fc:rt:t9:t3 0xa00:rs:t9:t3 0xa00:rt:t9:t3 0xa10:rd:t5:t7 0xa14:rt:t5:t7 0xa14:rd:t7:t9 0xa18:rs:t9:t3 0xa18:rt:t7:t9 0xa18:rd:t8:t2 0xa1c:rs:t8:t2 0xa30:rt:t6:t8 0xa38:rt:t6:t8 0xa60:rt:t3:t5 0xa64:rd:t2:t4 0xa68:rs:t2:t4 0xa68:rd:t2:t4 0xa6c:rs:t3:t5 0xa6c:rt:t3:t5 0xa70:rs:t2:t4 0xa70:rt:t3:t5 0xa74:rs:t2:t4 0xa78:rt:t2:t4 0xa90:rt:t4:t6 0xa98:rt:t4:t6 0xab4:rt:t5:t7 0xab8:rt:t5:t7 0xac8:rt:t9:t3 0xacc:rs:t9:t3 0xacc:rt:t9:t3 0xad0:rt:t9:t3 0xae8:rt:t7:t9 0xaf0:rs:t7:t9 0xafc:rs:t7:t9 0xafc:rt:t8:t2 0xb00:rs:t8:t2 0xb00:rt:t6:t8 0xb04:rt:t2:t4 0xb08:rt:t2:t4 0xb0c:rt:t6:t8 0xb18:rt:t3:t5 0xb20:rs:t3:t5 0xb2c:rs:t3:t5 0xb2c:rt:t4:t6 0xb30:rs:t4:t6 0xb30:rt:t5:t7 0xb34:rt:t9:t3 0xb38:rt:t9:t3 0xb3c:rt:t5:t7 0xb44:rt:t7:t9 0xb48:rt:t6:t8 0xb4c:rt:t7:t9 0xb4c:rd:t8:t2 0xb50:rs:t8:t2 0xb50:rt:t7:t9 0xb50:rd:t8:t2 0xb54:rt:t8:t2 0xb54:rd:t8:t2 0xb58:rs:t8:t2 0xb58:rt:t7:t9 0xb58:rd:t8:t2 0xb5c:rt:t8:t2 0xb5c:rd:t8:t2 0xb60:rs:t6:t8 0xb60:rt:t6:t8 0xb64:rs:t8:t2 0xb64:rt:t6:t8 0xbc8:rt:t2:t4 0xbd0:rt:t2:t4 0xbd0:rd:t3:t5 0xbd4:rt:t3:t5 */
#include "types.h"

typedef union {
    s32 l;
    struct { u16 lo; s16 hi; } w;
} Fix;

typedef struct {
    u8 b0;
    u8 layer;
    u8 f2;
    u8 f3;
    u8 f4;
    u8 f5;
    u8 rate;
    u8 bright;
    s16 tick;
    s16 pad0a;
    u32 *cels;
    u32 *anims;
    s16 pad14;
    s16 anim;
    s16 frame;
    s16 sx;
    s16 sy;
    s16 pad1e;
    s32 timing;
    Fix x;
    Fix y;
    s32 vx;
    s32 vy;
    u32 *hdr;
    s32 base;
    s32 pad3c;
    u8 r, g, b;
    u8 clut;
} Slot;

extern Slot class_w[];
extern u8 now_year;
extern u8 now_month;
extern u8 basyo_flag1;
extern u8 basyo_flag3;
extern u8 basyo_flag5;
extern u8 basyo_flag6;
extern u8 basyo_flag7;
extern u8 basyo_flaga;
extern u8 basyo_flagb;
extern u8 basyo_flagd;
extern u32 basyo_init_tbl1;
extern u32 basyo_init_tbl2;
extern u32 _sys_default_tpage_set_tbl2;
extern s16 basyo_init_tbl3;
extern u8 config_screen_init_str2[];
extern u8 config_screen_init_str3[];
extern u8 config_screen_init_str4[];
extern u8 config_screen_init_str5[];
extern u8 config_screen_init_str6[];
extern u8 config_screen_init_str7[];
extern u8 config_screen_init_tbl0[];
extern u8 **card_file_save_tbl0;
extern u8 config_screen_init_tbl2[];
extern u8 config_screen_init_tbl1[];
extern u8 config_screen_init_str1[];
extern u8 config_screen_init_str0[];
extern u8 config_screen_init_str8[];
extern u8 *config_screen_init_tbl3[];
extern u8 p_sei[];

void config_screen_init()
{
    s32 c0;
    s32 i;
    s32 n;
    s32 c1;
    s32 sel;

    class_clear_f();
    class_clear_n(96);
    class_w[96].layer = 5;
    class_w[96].f2 = 64;
    class_w[96].base = 0x40000000;
    class_w[96].f3 = 132;
    class_w[96].cels = &basyo_init_tbl1;
    class_w[96].anims = &basyo_init_tbl2;
    class_w[96].hdr = &_sys_default_tpage_set_tbl2;
    class_w[96].pad14 = basyo_init_tbl3;
    class_w[96].f4 = 8;
    class_w[96].f5 = 1;
    class_w[96].frame = 118;
    class_w[96].x.w.hi = 0;
    class_w[96].y.w.hi = -40;
    for (i = 0; i < 32; i++) {
        class_clear_n(i + 97);
        class_w[i + 97].layer = 4;
        class_w[i + 97].f2 = 64;
        class_w[i + 97].base = 0;
        class_w[i + 97].f3 = 132;
        class_w[i + 97].cels = &basyo_init_tbl1;
        class_w[i + 97].anims = &basyo_init_tbl2;
        class_w[i + 97].hdr = &_sys_default_tpage_set_tbl2;
        class_w[i + 97].pad14 = basyo_init_tbl3;
        class_w[i + 97].f4 = 8;
        class_w[i + 97].f5 = 1;
        class_w[i + 97].rate = 8;
        class_w[i + 97].frame = 0;
        class_w[i + 97].x.w.hi = 256;
        class_w[i + 97].y.w.hi = 256;
        class_w[i + 97].bright = 128;
    }
    n = 97;
    switch (now_month) {
    case 2:
    case 3:
    case 4:
        if (now_year == 95) {
            sel = 0;
            class_w[n].frame = 121;
            class_w[n].x.w.hi = 0;
            class_w[n].y.w.hi = -40;
            n++;
            set_kanji_string(-158, -60, 3, config_screen_init_str2, 0);
            set_kanji_string(-158, -44, 3, config_screen_init_str3, 0);
        }
        if (now_year == 96) {
            sel = 4;
            if (now_month != 2) {
                basyo_flagd = 1;
            }
            class_w[n].frame = 122;
            class_w[n].x.w.hi = 0;
            class_w[n].y.w.hi = -40;
            n++;
            class_w[n].frame = 129;
            class_w[n].x.w.hi = 8;
            class_w[n].y.w.hi = -40;
            n++;
        }
        if (now_year == 97) {
            sel = 8;
            if (now_month != 2) {
                basyo_flagb = 1;
            }
            class_w[n].frame = 122;
            class_w[n].x.w.hi = 0;
            class_w[n].y.w.hi = -40;
            n++;
            class_w[n].frame = 132; class_w[n].x.w.hi = 8; class_w[n].y.w.hi = -40;
            n++;
        }
        if (now_year == 98) {
            sel = 12;
            set_kanji_string(-158, -60, 3, config_screen_init_str6, 0);
            set_kanji_string(-158, -44, 3, config_screen_init_str7, 0);
        }
        break;
    case 5:
    case 6:
    case 7:
        class_w[n].frame = 123;
        class_w[n].x.w.hi = 0;
        class_w[n].y.w.hi = -40;
        n++;
        if (now_year == 95) {
            sel = 1;
            if (now_month != 5) {
                basyo_flag1 = 1;
            }
            class_w[n].frame = 126;
            class_w[n].x.w.hi = 8;
            class_w[n].y.w.hi = -40;
            n++;
        }
        if (now_year == 96) {
            sel = 5;
            if (now_month != 5) {
                basyo_flag5 = 1;
            }
            class_w[n].frame = 130; class_w[n].x.w.hi = 8; class_w[n].y.w.hi = -40;
            n++;
        }
        if (now_year == 97) {
            sel = 9;
            set_kanji_string(-158, -60, 3, config_screen_init_str4, 0);
            set_kanji_string(-158, -44, 3, config_screen_init_str5, 0);
        }
        break;
    case 8:
    case 9:
    case 10:
        class_w[n].frame = 124;
        class_w[n].x.w.hi = 0;
        class_w[n].y.w.hi = -40;
        n++;
        if (now_year == 95) {
            sel = 2;
            if ((now_month | 0) != 8) {
                basyo_flag7 = 1;
            }
            class_w[n].frame = 127;
            class_w[n].x.w.hi = 8;
            class_w[n].y.w.hi = -40;
            n++;
        }
        if (now_year == 96) {
            sel = 6;
            if ((now_month | 0) != 8) {
                basyo_flaga = 1;
            }
            class_w[n].frame = 131; class_w[n].x.w.hi = 8; class_w[n].y.w.hi = -40;
            n++;
        }
        if (now_year == 97) {
            sel = 10;
            set_kanji_string(-158, -60, 3, config_screen_init_str4, 0);
            set_kanji_string(-158, -44, 3, config_screen_init_str5, 0);
        }
        break;
    case 11:
    case 12:
        class_w[n].frame = 125;
        class_w[n].x.w.hi = 0;
        class_w[n].y.w.hi = -40;
        n++;
        if (now_year == 95) {
            sel = 3;
            if (now_month == 12) {
                basyo_flag3 = 1;
            }
            class_w[n].frame = 128;
            class_w[n].x.w.hi = 8;
            class_w[n].y.w.hi = -40;
            n++;
        }
        if (now_year == 96) {
            sel = 7;
            if (now_month == 12) {
                basyo_flag6 = 1;
            }
            class_w[n].frame = 133; class_w[n].x.w.hi = 8; class_w[n].y.w.hi = -40;
            n++;
        }
        if (now_year == 97) {
            sel = 11;
            set_kanji_string(-158, -60, 3, config_screen_init_str6, 0);
            set_kanji_string(-158, -44, 3, config_screen_init_str7, 0);
        }
        break;
    case 1:
        class_w[n].frame = 125;
        class_w[n].x.w.hi = 0;
        class_w[n].y.w.hi = -40;
        n++;
        if (now_year == 96) {
            sel = 3;
            basyo_flag3 = 1;
            class_w[n].frame = 128;
            class_w[n].x.w.hi = 8;
            class_w[n].y.w.hi = -40;
            n++;
        }
        if (now_year == 97) {
            sel = 7;
            basyo_flag6 = 1;
            class_w[n].frame = 133; class_w[n].x.w.hi = 8; class_w[n].y.w.hi = -40;
            n++;
        }
        if (now_year == 98) {
            sel = 11;
            set_kanji_string(-158, -60, 3, config_screen_init_str6, 0);
            set_kanji_string(-158, -44, 3, config_screen_init_str7, 0);
        }
        break;
    }
    if (now_year != 95 || now_month >= 6) {
        class_w[n].frame = 119;
        class_w[n].x.w.hi = 0;
        class_w[n].y.w.hi = -40;
        n++;
    }
    if (now_year == 95) {
        class_w[n].x.w.hi = 0;
        class_w[n].frame = sel + 151;
        class_w[n].y.w.hi = -40;
        n++;
    } else if (now_year == 96 && now_month < 2) {
        class_w[n].x.w.hi = 0;
        class_w[n].frame = sel + 151;
        class_w[n].y.w.hi = -40;
        n++;
    } else {
        class_w[n].frame = 120;
        class_w[n].layer++;
        c0 = config_screen_init_tbl0[sel * 3];
        c1 = config_screen_init_tbl0[sel * 3 + 1];
        class_w[n + 1].y.w.hi = 24;
        class_w[n + 1].frame = c0 + 144;
        class_w[n].x.w.hi = 0;
        class_w[n].y.w.hi = -40;
        class_w[n + 1].x.w.hi = 272;
        class_w[n + 2].frame = c1 + 144;
        class_w[n + 2].y.w.hi = 40;
        class_w[n + 2].x.w.hi = 272;
        class_w[n + 3].frame = config_screen_init_tbl0[sel * 3 + 2] + 144;
        class_w[n + 3].y.w.hi = 56;
        class_w[n + 3].x.w.hi = 272;
        n += 4;
        set_kanji_string(256, 16, 6, card_file_save_tbl0[(sel * 3 + 2) % 12], 0);
        set_kanji_string(256, 32, 6, card_file_save_tbl0[(sel * 3 + 3) % 12], 0);
        set_kanji_string(256, 48, 6, card_file_save_tbl0[(sel * 3 + 4) % 12], 0);
    }
    if (config_screen_init_tbl2[sel] != 255) {
        class_w[n].frame = config_screen_init_tbl2[sel];
        class_w[n].x.w.hi = 0;
        class_w[n].y.w.hi = -40;
        n++;
    }
    set_kanji_string(32, -94, 7, &config_screen_init_str1[sel * 17], 0);
    if (config_screen_init_tbl1[sel] != 255) {
        class_w[n].y.w.hi = -40;
        class_w[n].frame = config_screen_init_tbl1[sel];
        class_w[n].x.w.hi = 0;
        n++;
    }
    set_kanji_string(32, -62, 8, &config_screen_init_str0[sel * 17], 0);
    if (sel < 4) {
        class_w[n].frame = (sel & 3) + 151;
        class_w[n].y.w.hi = -40;
        class_w[n].x.w.hi = 0;
        n++;
    }
    if (sel >= 4) {
        class_w[n].frame = (sel & 3) + 147;
        class_w[n].y.w.hi = -40;
        class_w[n].x.w.hi = 0;
    }
    set_kanji_string(-143, -14, 6, &config_screen_init_str8[sel * 108], 0);
    set_kanji_string(-143, 2, 6, &config_screen_init_str8[sel * 108 + 36], 0);
    set_kanji_string(-143, 18, 6, &config_screen_init_str8[sel * 108 + 72], 0);
    k_sub_reset_point_set();
    k_sub_disp_start(0);
    gnsx(p_sei);
    sndisp(config_screen_init_tbl3[sel], 0, 31);
}
