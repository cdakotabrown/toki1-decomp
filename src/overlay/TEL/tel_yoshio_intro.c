/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
/* @SymbolStart: 0x8013226C */
/* Address: 0x80132274 */
/* Size: 3408 bytes (852 insns) */
/* @SymbolSize: 3416 */
/* @RodataBase: 0x8013EF7C */
/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic0 */
/* @PostProc: remap_stack_slots --from 48 --to 60 --from 32 --to 44 */
#include "types.h"

extern s32  reserved_date_check(u8, u8, u8);
extern s32  get_g_zyotai_h(s32);
extern void k_sub_reset(void);
extern void gnsx(s32);
extern void sndi(char *, s32, s32);
extern void xa_set_m(s32, s32, s32, s32);
extern void k_sub_disp_start(s32);
extern void xa_wait(void);
extern void xa_wait_m(void);
extern void get_p_name(char *, s32);
extern void x_taku_string_set(char *, char *, s32, s32);
extern void x_taku_menu_set(s32, s32);
extern void menu_check(s32, s32, s32);
extern void menu_bar_show(s32);
extern void menu_cursor_move(s32);
extern void goto_sub_sub_step(s32);
extern void strcpy(char *, s32);
extern void strcat(char *, s32);
extern void cal_base_show(void);

extern volatile u8 girl13_tel_cnt;
extern u8  now_year;
extern u8  now_month;
extern u8  now_day;
extern u32 reserved_date;
extern u8  step_mode;
extern u32 pad_trig;
extern s8  menu_csr;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern u8  p_sei[];

extern char tel_yoshio_intro_str0[];
extern char tel_yoshio_intro_str1[];
extern char tel_yoshio_intro_str2[];
extern char tel_yoshio_intro_str3[];
extern char tel_yoshio_intro_str4[];
extern char tel_yoshio_intro_str5[];
extern char tel_yoshio_intro_str6[];
extern char tel_yoshio_intro_str7[];
extern char tel_yoshio_intro_str8[];
extern char tel_yoshio_intro_str9[];
extern char tel_yoshio_intro_str10[];
extern char tel_yoshio_intro_str11[];
extern char tel_yoshio_intro_str12[];
extern char tel_yoshio_intro_str13[];
extern char tel_yoshio_intro_str14[];
extern char tel_yoshio_intro_str15[];
extern char tel_yoshio_intro_str16[];
extern char tel_yoshio_intro_str17[];
extern char tel_yoshio_intro_str18[];
extern char tel_yoshio_intro_str19[];
extern char tel_yoshio_intro_str20[];
extern char tel_yoshio_intro_str21[];
extern char tel_yoshio_intro_str22[];
extern char tel_yoshio_intro_str23[];
extern char tel_yoshio_intro_str24[];
extern char tel_yoshio_intro_str25[];
extern char tel_yoshio_intro_str26[];
extern char tel_yoshio_intro_str27[];
extern char tel_yoshio_intro_str28[];
extern char tel_yoshio_intro_str29[];
extern char tel_yoshio_intro_str30[];
extern char tel_yoshio_intro_str31[];
extern char tel_yoshio_intro_str32[];
extern char tel_yoshio_intro_str33[];
extern char tel_yoshio_intro_str34[];
extern char tel_yoshio_intro_str35[];
extern char tel_yoshio_intro_str36[];
extern char tel_yoshio_intro_str37[];
extern char tel_yoshio_intro_str38[];
extern char tel_yoshio_intro_str39[];
extern char tel_yoshio_intro_str40[];
extern char tel_yoshio_intro_str41[];
extern char tel_yoshio_intro_str42[];
extern char tel_yoshio_intro_str43[];
extern char tel_yoshio_intro_str44[];
extern char tel_yoshio_intro_str45[];
extern char tel_yoshio_aisatu_xa_tbl[];
extern char tel_yoshio_aisatu_xa_tbl2[];
extern s32 get_g_name_tbl0[];
extern u32 date_name_set_tbl0[];

void tel_yoshio_intro(void)
{
    s32 idx = step_mode;
    char buf[44];
    char tmp[16];

    switch (idx) {
    case 0:
        if (girl13_tel_cnt != 0)
            goto Laf70call;
        step_mode = 1;
        goto LincEC03;
    Laf70call:
        if (reserved_date_check(now_year, now_month, now_day) <= 0)
            goto Lcheck2;
        if (((reserved_date << 3) >> 28) != 10)
            goto Lcheck2;
        step_mode = 48;
        goto LincEC03;
    Lcheck2:
        if (get_g_zyotai_h(10) == 4) {
            step_mode = 48;
        } else if (girl13_tel_cnt & 1) {
            step_mode = 16;
        } else {
            step_mode = 48;
        }
    LincEC03:
        girl13_tel_cnt = girl13_tel_cnt + 1;
        goto Lexit;

    case 1:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndi(tel_yoshio_intro_str0, 0, 13);
        xa_set_m((s32)0x1E000048, (s32)0x0D00004D, -1, 13);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 2:
        xa_wait_m();
        goto Lexit;

    case 3:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndi(tel_yoshio_intro_str1, 0, 15);
        strcpy(buf, (s32)((u32)p_sei | 0));
        strcat(buf, tel_yoshio_intro_str2);
        sndi(buf, 1, 15);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 4:
        xa_wait();
        goto Lexit;

    case 5:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        strcpy(buf, tel_yoshio_intro_str3);
        get_p_name(tmp, 13);
        strcat(buf, (s32)tmp);
        strcat(buf, tel_yoshio_intro_str4);
        sndi(buf, 0, 13);
        sndi(tel_yoshio_intro_str5, 1, 13);
        xa_set_m((s32)tel_yoshio_aisatu_xa_tbl, (s32)tel_yoshio_aisatu_xa_tbl2, 0, 13);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 6:
        xa_wait_m();
        goto Lexit;

    case 7:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndi(tel_yoshio_intro_str6, 0, 15);
        sndi(tel_yoshio_intro_str7, 1, 15);
        sndi(tel_yoshio_intro_str8, 2, 15);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 8:
        xa_wait();
        goto Lexit;

    case 9:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndi(tel_yoshio_intro_str9, 0, 13);
        sndi(tel_yoshio_intro_str10, 1, 13);
        xa_set_m((s32)0x180000B8, (s32)0x140000C1, -1, 13);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 10:
        xa_wait_m();
        goto Lexit;

    case 11:
        step_mode = 32;
        goto Lexit;

    case 16:
        k_sub_reset();
        gnsx((*(s32 *)&date_name_set_tbl0[10]));
        sndi(tel_yoshio_intro_str11, 0, 10);
        xa_set_m((s32)0x01000143, (s32)0x1E000149, -1, 10);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 17:
        xa_wait_m();
        goto Lexit;

    case 18:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndi(tel_yoshio_intro_str12, 0, 15);
        sndi(tel_yoshio_intro_str13, 1, 15);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 19:
        xa_wait();
        goto Lexit;

    case 20:
        k_sub_reset();
        gnsx((*(s32 *)&date_name_set_tbl0[10]));
        sndi(tel_yoshio_intro_str14, 0, 10);
        sndi(tel_yoshio_intro_str15, 1, 10);
        sndi(tel_yoshio_intro_str16, 2, 10);
        xa_set_m((s32)0x020001A9, (s32)0x1F0001C4, -1, 10);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 21:
        xa_wait_m();
        goto Lexit;

    case 22:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndi(tel_yoshio_intro_str17, 0, 13);
        xa_set_m((s32)0x1E000048, (s32)0x0D00004D, -1, 13);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 23:
        xa_wait_m();
        goto Lexit;

    case 24:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndi(tel_yoshio_intro_str18, 0, 15);
        sndi((char *)p_sei, 1, 15);
        sndi(tel_yoshio_intro_str19, 2, 15);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 25:
        xa_wait();
        goto Lexit;

    case 26:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        strcpy(buf, tel_yoshio_intro_str20);
        strcat(buf, (s32)p_sei);
        strcat(buf, tel_yoshio_intro_str21);
        sndi(buf, 0, 13);
        xa_set_m((s32)0x1E000056, (s32)0x10000059, -1, 13);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 27:
        xa_wait_m();
        goto Lexit;

    case 28:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndi(tel_yoshio_intro_str22, 0, 15);
        sndi(tel_yoshio_intro_str23, 1, 15);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 29:
        xa_wait();
        goto Lexit;

    case 30:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndi(tel_yoshio_intro_str24, 0, 13);
        sndi(tel_yoshio_intro_str25, 1, 13);
        sndi(tel_yoshio_intro_str26, 2, 13);
        xa_set_m((s32)0x1400009A, (s32)0x0C0000AB, -1, 13);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 31:
        xa_wait_m();
        goto Lexit;

    case 32:
        k_sub_reset();
        x_taku_string_set(tel_yoshio_intro_str27, tel_yoshio_intro_str28, 0, 2);
        x_taku_menu_set(1, 2);
        k_sub_disp_start(2);
        step_mode += 1;
        goto Lexit;

    case 33:
        menu_check(1, cursor_cw_x, cursor_cw_y);
        menu_bar_show(1);
        menu_cursor_move(1);
        if (!(pad_trig & 0x20))
            goto Lexit;
        switch (menu_csr) {
        case 0:
            step_mode = 34;
            goto Lexit;
        case 1:
            goto_sub_sub_step(2);
            goto Lexit;
        }
        goto Lexit;

    case 34:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndi(tel_yoshio_intro_str29, 0, 15);
        sndi(tel_yoshio_intro_str30, 1, 15);
        sndi(tel_yoshio_intro_str31, 2, 15);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 35:
        xa_wait();
        goto Lexit;

    case 36:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndi(tel_yoshio_intro_str32, 0, 13);
        sndi(tel_yoshio_intro_str33, 1, 13);
        xa_set_m((s32)0x19000085, (s32)0x0D00008F, -1, 13);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 37:
        xa_wait_m();
        goto Lexit;

    case 38:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndi(tel_yoshio_intro_str34, 0, 13);
        sndi(tel_yoshio_intro_str35, 1, 13);
        xa_set_m((s32)0x19000090, (s32)0x0E00009C, -1, 13);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 39:
        xa_wait_m();
        goto Lexit;

    case 40:
        goto_sub_sub_step(3);
        goto Lexit;

    case 48:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndi(tel_yoshio_intro_str36, 0, 13);
        xa_set_m((s32)0x1E000048, (s32)0x0D00004D, -1, 13);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 49:
        xa_wait_m();
        goto Lexit;

    case 50:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndi(tel_yoshio_intro_str37, 0, 15);
        strcpy(buf, (s32)((u32)p_sei | 0));
        strcat(buf, tel_yoshio_intro_str38);
        sndi(buf, 1, 15);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 51:
        xa_wait();
        goto Lexit;

    case 52:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        strcpy(buf, tel_yoshio_intro_str39);
        strcat(buf, (s32)p_sei);
        strcat(buf, tel_yoshio_intro_str40);
        sndi(buf, 0, 13);
        xa_set_m((s32)0x1E000056, (s32)0x10000059, -1, 13);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 53:
        xa_wait_m();
        goto Lexit;

    case 54:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndi(tel_yoshio_intro_str41, 0, 15);
        sndi(tel_yoshio_intro_str42, 1, 15);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 55:
        xa_wait();
        goto Lexit;

    case 56:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndi(tel_yoshio_intro_str43, 0, 13);
        sndi(tel_yoshio_intro_str44, 1, 13);
        sndi(tel_yoshio_intro_str45, 2, 13);
        xa_set_m((s32)0x1400009A, (s32)0x0C0000AB, -1, 13);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 57:
        xa_wait_m();
        goto Lexit;

    case 58:
        step_mode = 32;

    default:
    Lexit:
        menu_bar_show(0);
        cal_base_show();
        break;
    }
}
