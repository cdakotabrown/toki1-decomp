/* @CompileEnv: IDO52_COPYPROP_VAR=-12 IDO52_CMOT_KILL_AVAIL=269 IDO52_AS1_TIE_REJECT=16 IDO52_COPYPROP_SITE=lod:2:deny,lod:11:deny IDO52_NO_CSE_SITE=35:1,35:2,35:3,35:5,35:6 IDO52_FORCE_WEB_COLOR=11:v1,50:a3,76:a0,167:a2,193:a0 */
/* @SymbolStart: 0x8013FBF8 */
/* @SymbolSize: 1152 */
/* Address: 0x8013FC00 (the window opens at the absorbed head 0x8013FBF8) */
/* Size: 1144 bytes (286 insns) + an 8-byte cross-prologue head = 288 insns */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);
extern void k_reset(u8);
extern s32 check_end_k(void);
extern void menu_check(s32, s32, s32);
extern void menu_cursor_move(s32);

extern u8 now_year;
extern s16 para_bun;
extern s16 para_gei;
extern u16 bukatu_pt5;
extern s32 pad_trig;
extern u8 menu0_hit0;
extern u8 menu0_hit1;
extern u8 menu0_hit2;
extern u8 menu0_hit3;
extern u8 menu_mode;
extern u32 frame_counter;
extern u8 cursor_cw_disp;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern u8 cw97_disp;
extern s16 cw97_frame;
extern s16 cw97_scaley;
extern s16 cw97_x;
extern s16 cw97_y;
extern s32 menu_on;
extern s16 shougatu_menu_now_serifu;
extern s16 shougatu_menu_now_serifu_in;
extern u8 shougatu_menu_girl1;
extern u8 shougatu_sel_girl_no;
extern s8 shougatu_menu_slot;
extern s8 shougatu_pair_disp_on;
u8 shougatu_menu_girl_tbl[4] = {0};

s32 shougatu_menu_select(void)
{
    u32 g;
    s32 i;
    s32 j;
    s16 n;
    s32 c0;
    s32 c1;
    s32 c2;
    u8 c3;
    u8 p;
    s32 slot;
    s32 flag;
    u8 ch;

    g = frame_counter;
    c0 = (u8)(g & 3);

    i = 1;
    for (;;) {
        c1 = (u8)((g * 2 + i) & 3);
        j = i;
        if (c0 != c1) {
            break;
        }
        i = (s16)(i + 1);
    }
    for (;;) {
        c2 = (u8)((g / 10 + j) & 3);
        if (c0 != c2 && c1 != c2) {
            break;
        }
        i = (s16)(i + 1);
        j = i;
    }
    for (;;) {
        c3 = (u8)(i % 4);
        if (c0 != c3 && c1 != c3 && (shougatu_menu_girl_tbl[3] = c3, c2 != c3)) {
            shougatu_menu_girl_tbl[0] = c0;
            shougatu_menu_girl_tbl[1] = c1;
            shougatu_menu_girl_tbl[2] = c2;
            break;
        }
        i = (s16)(i + 1);
    }

    for (n = 0; n < 4; n = (s16)(n + 1)) {
        if (shougatu_menu_girl_tbl[n] == 1) {
            shougatu_menu_girl_tbl[n] = 11;
        } else if (shougatu_menu_girl_tbl[n] == 2) {
            shougatu_menu_girl_tbl[n] = shougatu_menu_girl1;
        } else if (shougatu_menu_girl_tbl[n] == 3) {
            shougatu_menu_girl_tbl[n] = shougatu_sel_girl_no;
        }
    }

    menu_check(0, cursor_cw_x, cursor_cw_y);
    menu_cursor_move(0);

    if ((pad_trig & 0xF000) != 0) {
        p = menu_mode;
        if ((p & 3) == 1 || (p & 3) == 3) {
            device_cmd_send(1282);
        }
    }
    if ((pad_trig & 0x00200020) == 0) {
        return;
    }
    if (check_end_k() == 0) {
        return;
    }
    if (menu0_hit0 != 0) {
        shougatu_menu_slot = 0;
    } else if (menu0_hit1 != 0) {
        shougatu_menu_slot = 1;
    } else if (menu0_hit2 != 0) {
        shougatu_menu_slot = 2;
    } else if (menu0_hit3 != 0) {
        shougatu_menu_slot = 3;
    } else {
        return 0;
    }

    cursor_cw_disp = (u8)(cursor_cw_disp & 0xFF7F);
    menu_on = 0;
    k_reset(1);
    cw97_disp = (u8)(cw97_disp | 0x80);

    slot = *(u8 *)&shougatu_menu_slot;
    flag = (shougatu_menu_girl_tbl[slot] == 11U);
    if (flag != 0) {
        ch = now_year;
        flag = (ch == 95);
    }
    cw97_frame = (s16)(shougatu_menu_girl_tbl[slot] + (now_year == 96) * 11
                       + (now_year == 97) * 23 - flag + 1);
    cw97_x = (s16)(slot * 78 - slot - 117);
    cw97_y = -60;
    cw97_scaley = 0;
    shougatu_menu_now_serifu += shougatu_menu_girl_tbl[slot] * 3 + now_year - 95;
    shougatu_pair_disp_on = 1;
    shougatu_menu_slot = slot;

    if (now_year == 95 && shougatu_menu_girl_tbl[slot] == 0 && para_gei >= 100) {
        goto set;
    }
    if (((shougatu_menu_girl_tbl[slot] == 0) & (now_year == 95))
        && (u32)bukatu_pt5 >= 100U) {
        goto set;
    }
    if (((shougatu_menu_girl_tbl[slot] == 1) & (now_year == 95))
        && para_bun >= 70) {
        goto set;
    }
    if (((shougatu_menu_girl_tbl[slot] == 1) & (now_year == 97))
        && para_bun >= 100) {
        goto set;
    }
    if (((shougatu_menu_girl_tbl[slot] == 2) & (now_year == 96))
        && para_bun >= 100) {
        goto set;
    }
    goto done;
set:
    shougatu_menu_now_serifu_in = 1;
done:
    sub_sub_step_up();
}
