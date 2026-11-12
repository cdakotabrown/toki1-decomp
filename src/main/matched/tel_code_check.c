/* Function: tel_code_check */
/* Address: 0x80075D38  (true start; 0x80075D40 is the PROLOGUE) */
/* Size: 796 bytes (199 instructions) over the corrected window */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=17:v1 IDO52_UGEN_DEST_TEMP=3,13,23,33 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG_INPLACE=1,6,12,18 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=1:a0,6:a0,12:a0,18:a0 */
/* @SymbolStart: 0x80075D38 */
/* @SymbolSize: 796 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 pad_trig;
extern u8 holiday_screen_init_tbl0[];
extern volatile u8 tel_code_check_tbl0;
extern s32 work_fuku;
extern u8 now_girl;
extern u8 now_icon;
extern u8 step_mode;
extern u16 byouki_state;
extern s16 para_str;
extern u8 holiday_tel_exit_tbl0[];
extern s32 screen_w[];

extern void device_cmd_send(s32);
extern s32 rand(void);
extern void goto_sub_step(s32);
extern void goto_sub_sub_step(s32);
extern void text_sys_init(void);
extern void icon_disp_switch(s32);
extern void mes_window_switch(s32);
extern void telephone_choice_menu_init(void);
extern void k_sub_reset_point_set(void);
extern s32 k_sub_disp_start(s32);

s32 tel_code_check(void) {
    s32 v1;
    u8 *t4;
    s32 v0;
    u8 *t5;
    s32 a2;
    u8 *a0ptr;

    if (cursor_cw_x >= -64) {
        goto fail;
    }
    if (cursor_cw_x < -79) {
        goto fail;
    }

    v1 = cursor_cw_y;
    if (v1 >= 24) {
        goto fail;
    }
    if (v1 < -15) {
        goto fail;
    }

    v1 = pad_trig & 0x70;
    t4 = holiday_screen_init_tbl0;

    switch (v1) {
    case 32:
        t4[tel_code_check_tbl0] = 0;
        tel_code_check_tbl0 = tel_code_check_tbl0 + 1;
        device_cmd_send(1285);
        v1 = pad_trig & 0x70;
        t4 = holiday_screen_init_tbl0;
        break;
    case 64:
        t4[tel_code_check_tbl0] = 1;
        tel_code_check_tbl0 = tel_code_check_tbl0 + 1;
        device_cmd_send(1286);
        v1 = pad_trig & 0x70;
        t4 = holiday_screen_init_tbl0;
        break;
    case 16:
        t4[tel_code_check_tbl0] = 2;
        tel_code_check_tbl0 = tel_code_check_tbl0 + 1;
        device_cmd_send(1287);
        v1 = pad_trig & 0x70;
        t4 = holiday_screen_init_tbl0;
        break;
    }

    if (tel_code_check_tbl0 >= 7U) {
        tel_code_check_tbl0 = 0;
    }

    if (v1 == 0) {
        goto fail;
    }

    a2 = 0;
    t5 = holiday_tel_exit_tbl0;
    do {
        v1 = (u32)tel_code_check_tbl0 + 7;
        v0 = 0;
        a0ptr = t5;
        do {
            if (t4[(u32)v1 % 7] != a0ptr[0]) {
                break;
            }
            if (v0 == 6) {
                if ((((u32)(*(s32 *)((u8 *)screen_w + a2 * 56
                        + 456))) << 28) >> 31) {
                    if (a2 != 11) {
                        if (a2 == 12) {
                            work_fuku = 11;
                            now_girl = 11;
                            now_icon = 12;
                        } else {
                            work_fuku = a2;
                            now_girl = (u8)a2;
                            now_icon = (u8)(a2 + 1);
                        }
                    }
                    *(s32 *)((u8 *)screen_w + a2 * 56 + 444) +=
                        rand() & 0x7fff;
                    if (byouki_state & 1) {
                        para_str += 32;
                    }
                    goto_sub_step(16);
                    goto_sub_sub_step(2);
                    step_mode = 11;
                    text_sys_init();
                    icon_disp_switch(0);
                    mes_window_switch(0);
                    telephone_choice_menu_init();
                    k_sub_reset_point_set();
                    k_sub_disp_start(0);
                    return 1;
                }
            }
            v1++;
            v0++;
            a0ptr++;
        } while (v0 != 7);
        a2++;
        t5 += 7;
    } while (a2 != 13);

fail:
    return 0;
}
