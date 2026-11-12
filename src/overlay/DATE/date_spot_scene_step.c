/* @RodataBase: 0x8015B0A0 */
/* @CompileFlags: -cckr */
/* Address: 0x80144D98, Size: 3192 bytes (798 insns) */
/* @SymbolSize: 3192 */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct { FuncPtr ptrs[18]; } Tbl1;
typedef struct { FuncPtr ptrs[35]; } Tbl2;
typedef struct { FuncPtr ptrs[48]; } Tbl3;
typedef struct { FuncPtr ptrs[32]; } Tbl4;
typedef struct { FuncPtr ptrs[35]; } Tbl5;
typedef struct { FuncPtr ptrs[34]; } Tbl6;
typedef struct { FuncPtr ptrs[36]; } Tbl7;
typedef struct { FuncPtr ptrs[37]; } Tbl8;
typedef struct { FuncPtr ptrs[50]; } Tbl9;
typedef struct { FuncPtr ptrs[39]; } Tbl10;
typedef struct { FuncPtr ptrs[32]; } Tbl11;
typedef struct { FuncPtr ptrs[34]; } Tbl12;
typedef struct { FuncPtr ptrs[104]; } Tbl13;
typedef struct { FuncPtr ptrs[29]; } Tbl14;
typedef struct { FuncPtr ptrs[45]; } Tbl15;
typedef struct { FuncPtr ptrs[41]; } Tbl16;
typedef struct { FuncPtr ptrs[35]; } Tbl17;
typedef struct { FuncPtr ptrs[76]; } Tbl18;
typedef struct { FuncPtr ptrs[74]; } Tbl19;
typedef struct { FuncPtr ptrs[31]; } Tbl20;
typedef struct { FuncPtr ptrs[31]; } Tbl21;

extern Tbl1 date_main1_tbl0;
extern Tbl2 date_main1_tbl1;
extern Tbl3 date_main1_tbl2;
extern Tbl4 date_main1_tbl3;
extern Tbl5 date_main1_tbl4;
extern Tbl6 date_main1_tbl5;
extern Tbl7 date_main1_tbl6;
extern Tbl8 date_main1_tbl7;
extern Tbl9 date_main1_tbl8;
extern Tbl10 date_main1_tbl9;
extern Tbl11 date_main1_tbl10;
extern Tbl12 date_main1_tbl11;
extern Tbl13 date_main1_tbl12;
extern Tbl14 date_main1_tbl13;
extern Tbl15 date_main1_tbl14;
extern Tbl16 date_main1_tbl15;
extern Tbl17 date_main1_tbl16;
extern Tbl18 date_main1_tbl17;
extern Tbl19 date_main1_tbl18;
extern Tbl20 date_main1_tbl19;
extern Tbl21 date_main1_tbl20;

extern u8 date2_menu_select_tbl1;
extern u8 now_basyo;
extern u8 date_pattern;
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u8 now_sub_sub_step;
extern u8 step_mode;
extern s32 date_effect_on;
extern s32 date_pal_on;
extern s32 date_disp_flag;
extern u8 fade_bri;
extern u8 cal_cw_disp;
extern u8 date_light_on;
extern u8 date_last_light;

extern void device_cmd_send(s32);
extern void date_light_effect(void);
extern void sprite_fade_main(s32);
extern void date_effect_move(void);
extern void date_spring_clear(void);
extern void date_spring(void);
extern void date_palette_cycle7(void);
extern void date_ending_dispatch(s32);

void date_spot_scene_step(void)
{
    s32 pad0;
    Tbl1 buf1;
    Tbl2 buf2;
    Tbl3 buf3;
    Tbl4 buf4;
    Tbl5 buf5;
    Tbl6 buf6;
    Tbl7 buf7;
    Tbl8 buf8;
    Tbl9 buf9;
    Tbl10 buf10;
    Tbl11 buf11;
    Tbl12 buf12;
    Tbl13 buf13;
    Tbl14 buf14;
    Tbl15 buf15;
    Tbl16 buf16;
    Tbl17 buf17;
    Tbl18 buf18;
    Tbl19 buf19;
    Tbl20 buf20;
    Tbl21 buf21;
    FuncPtr *sel;
    s32 pad1;

    buf1 = date_main1_tbl0;
    buf2 = date_main1_tbl1;
    buf3 = date_main1_tbl2;
    buf4 = date_main1_tbl3;
    buf5 = date_main1_tbl4;
    buf6 = date_main1_tbl5;
    buf7 = date_main1_tbl6;
    buf8 = date_main1_tbl7;
    buf9 = date_main1_tbl8;
    buf10 = date_main1_tbl9;
    buf11 = date_main1_tbl10;
    buf12 = date_main1_tbl11;
    buf13 = date_main1_tbl12;
    buf14 = date_main1_tbl13;
    buf15 = date_main1_tbl14;
    buf16 = date_main1_tbl15;
    buf17 = date_main1_tbl16;
    buf18 = date_main1_tbl17;
    buf19 = date_main1_tbl18;
    buf20 = date_main1_tbl19;
    buf21 = date_main1_tbl20;

    switch (now_basyo) {
    case 0:
        sel = buf1.ptrs;
        break;
    case 1:
        sel = buf2.ptrs;
        break;
    case 2:
        sel = buf3.ptrs;
        break;
    case 3:
        sel = buf4.ptrs;
        break;
    case 4:
        sel = buf5.ptrs;
        break;
    case 5:
        sel = buf6.ptrs;
        break;
    case 6:
        sel = buf7.ptrs;
        break;
    case 7:
        sel = buf8.ptrs;
        break;
    case 8:
        sel = buf9.ptrs;
        break;
    case 9:
        sel = buf10.ptrs;
        break;
    case 10:
        sel = buf11.ptrs;
        break;
    case 11:
        sel = buf12.ptrs;
        break;
    case 12:
        sel = buf13.ptrs;
        break;
    case 13:
        sel = buf14.ptrs;
        break;
    case 14:
        sel = buf15.ptrs;
        break;
    case 15:
        sel = buf16.ptrs;
        break;
    case 16:
        sel = buf17.ptrs;
        break;
    case 17:
        sel = buf18.ptrs;
        break;
    case 18:
        sel = buf19.ptrs;
        break;
    case 19:
        sel = buf20.ptrs;
        break;
    case 20:
        sel = buf21.ptrs;
        break;
    default:
        sel = buf1.ptrs;
        break;
    }

    if (sel[now_sub_sub_step] == sprite_fade_main) {
        if (step_mode == 0) {
            date_last_light = date_light_on;
        }
        if (now_basyo != 1 && now_basyo != 4 && now_basyo != 12) {
            if ((step_mode == 0 && date_last_light == 1)
                || (step_mode == 14 && date_last_light == 0)) {
                date_light_on = 1 - date_last_light;
            }
        }
        step_mode++;
    }

    if (((now_year == 95 && (now_month | 0) == 7 && now_day == 23)
         || (now_year == 96 && (now_month | 0) == 7 && now_day == 28)
         || (now_year == 97 && (now_month | 0) == 7 && now_day == 27))
        && fade_bri == 128 && date_light_on != 0
        && sel[now_sub_sub_step] != date_ending_dispatch) {
        date_light_effect();
        date_disp_flag = 1;
    } else if (now_basyo == 1 && date_pattern == 0
               && now_sub_sub_step >= 12 && now_sub_sub_step < 21
               && now_month >= 3 && now_month < 9 && fade_bri == 128) {
        date_light_effect();
        date_disp_flag = 1;
    } else if (now_basyo == 20 && now_sub_sub_step >= 7 && now_sub_sub_step < 17
               && fade_bri == 128) {
        date_light_effect();
        date_disp_flag = 1;
    } else if (now_basyo == 17 && now_sub_sub_step < 8
               && fade_bri == 128) {
        date_light_effect();
        date_disp_flag = 1;
    } else if (now_basyo == 17 && now_sub_sub_step >= 9 && now_sub_sub_step < 30
               && fade_bri == 128) {
        date_light_effect();
        date_disp_flag = 1;
    } else if (now_basyo == 17 && now_sub_sub_step >= 31 && now_sub_sub_step < 44
               && fade_bri == 128) {
        date_light_effect();
        date_disp_flag = 1;
    } else if (now_basyo == 17 && now_sub_sub_step >= 45 && now_sub_sub_step < 49
               && fade_bri == 128) {
        date_light_effect();
        date_disp_flag = 1;
    } else if (now_basyo == 17 && now_sub_sub_step >= 62 && now_sub_sub_step < 73
               && fade_bri == 128) {
        date_light_effect();
        date_disp_flag = 1;
    } else if (now_basyo == 16 && now_sub_sub_step < 8
               && fade_bri == 128) {
        date_light_effect();
        date_disp_flag = 1;
    } else if (now_basyo == 16 && now_sub_sub_step >= 9 && now_sub_sub_step < 19
               && fade_bri == 128) {
        date_light_effect();
        date_disp_flag = 1;
    } else if (now_basyo == 16 && now_sub_sub_step >= 20 && now_sub_sub_step < 34
               && fade_bri == 128) {
        date_light_effect();
        date_disp_flag = 1;
    }

    sel[now_sub_sub_step](128);

    if (now_basyo == 12 && now_sub_sub_step >= 69 && now_sub_sub_step < 91
        && date_pal_on != 0) {
        cal_cw_disp |= 0x80;
        date_palette_cycle7();
    } else if (now_basyo == 12 && now_sub_sub_step >= 91) {
        cal_cw_disp &= 0x7F;
    }
    if (date_effect_on != 0) {
        date_effect_move();
    }
    if (now_basyo == 1 && now_sub_sub_step >= 12 && now_sub_sub_step < 21) {
        date_spring();
    }
    if (now_basyo == 1 && now_sub_sub_step == 21) {
        date_spring_clear();
    }
    if (now_basyo == 11 && now_sub_sub_step < 9 && date2_menu_select_tbl1 == 1
        && date_pattern != 2) {
        date_pattern = 2;
        device_cmd_send(0x601);
    }
}
