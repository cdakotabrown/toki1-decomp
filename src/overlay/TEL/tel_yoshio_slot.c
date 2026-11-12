/* @SymbolStart: 0x80134598 */
/* @RodataBase: 0x8013F328 */
/* @SymbolSize: 1396 */
#include "types.h"

extern u8 step_mode;
extern s8 menu_csr;
extern s16 cal_cw_frame;

extern u8 p_sei[];

extern u8 tel_yoshio_slot_str0[];
extern u8 tel_yoshio_slot_str1[];
extern u8 tel_yoshio_slot_str2[];
extern u8 tel_yoshio_slot_str3[];
extern u8 tel_yoshio_slot_str4[];
extern u8 tel_yoshio_slot_str5[];
extern u8 tel_yoshio_slot_str6[];
extern u8 tel_yoshio_slot_str7[];
extern u8 tel_yoshio_slot_str8[];
extern u8 tel_yoshio_slot_str9[];
extern u8 tel_yoshio_slot_str10[];
extern u8 tel_yoshio_slot_str11[];
extern u8 tel_yoshio_slot_str12[];
extern u8 tel_yoshio_slot_str13[];
extern u8 tel_yoshio_slot_str14[];
extern s32 work_fuku[];
extern s32 get_g_name_tbl0[];

extern void cd_read_start2(s32 a0, s32 a1, u32 a2);
extern s32 get_CD_State2(void);
extern void class_clear_f(void);
extern void text_sys_init(void);
extern void k_sub_reset_point_set(void);
extern void k_sub_reset(void);
extern void k_sub_disp_start(s32 a0);
extern void gnsx(s32 a0);
extern void sndi(u8 *a0, s32 a1, s32 a2);
extern void xa_wait(void);
extern void xa_set_m(s32 a0, s32 a1, s32 a2, s32 a3);
extern void xa_wait_m(void);
extern void set_dec_bri(s32 a0);
extern void cal_base_show(void);
extern void appraisal_base_show(void);
extern void return_schedule(s32 a0);
extern void tel_slot_disp(s32 a0, u8 *a1);

void tel_yoshio_slot(void)
{
    switch ((u32)step_mode) {
    case 0:
        cd_read_start2(38, 0x801A0000, 0x9D5F);
        step_mode += 1;
        break;
    case 1:
        if (get_CD_State2() & 1) {
            step_mode += 1;
        }
        break;
    case 2:
        text_sys_init();
        tel_slot_disp(menu_csr, ((u8 *)&work_fuku[1]));
        k_sub_reset_point_set();
        k_sub_disp_start(0);
        if (menu_csr == 10) {
            step_mode = 32;
        }
        step_mode += 1;
        break;
    case 3:
        step_mode = step_mode + 1;
        break;
    case 4:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndi(tel_yoshio_slot_str0, 0, 13);
        xa_set_m(0x1E00005A, 0x1100005D, -1, 13);
        k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 5:
        xa_wait_m();
        break;
    case 6:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndi(tel_yoshio_slot_str1, 0, 15);
        sndi(tel_yoshio_slot_str2, 1, 15);
        k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 7:
        xa_wait();
        break;
    case 8:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndi(tel_yoshio_slot_str3, 0, 13);
        sndi(tel_yoshio_slot_str4, 1, 13);
        sndi(tel_yoshio_slot_str5, 2, 13);
        xa_set_m(0x140000BE, 0x0E0000CF, -1, 13);
        k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 9:
        xa_wait_m();
        break;
    case 10:
        text_sys_init();
        class_clear_f();
        set_dec_bri(0);
        return_schedule(0);
        break;
    case 32:
        xa_wait();
        break;
    case 33:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndi(tel_yoshio_slot_str6, 0, 13);
        xa_set_m(0x1E00005A, 0x1100005D, -1, 13);
        k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 34:
        xa_wait_m();
        break;
    case 35:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndi(tel_yoshio_slot_str7, 0, 15);
        k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 36:
        xa_wait();
        break;
    case 37:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndi(tel_yoshio_slot_str8, 0, 13);
        sndi(tel_yoshio_slot_str9, 1, 13);
        sndi(tel_yoshio_slot_str10, 2, 13);
        xa_set_m(0x1900007B, 0x0C000084, -1, 13);
        k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 38:
        xa_wait_m();
        break;
    case 39:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndi(tel_yoshio_slot_str11, 0, 15);
        sndi(tel_yoshio_slot_str12, 1, 15);
        k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 40:
        xa_wait();
        break;
    case 41:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndi(tel_yoshio_slot_str13, 0, 13);
        sndi(tel_yoshio_slot_str14, 1, 13);
        xa_set_m(0x0A0000E3, 0x0E0000EA, -1, 13);
        k_sub_disp_start(0);
        step_mode += 1;
        break;
    case 42:
        xa_wait_m();
        break;
    case 43:
        text_sys_init();
        class_clear_f();
        set_dec_bri(0);
        return_schedule(0);
        break;
    }
    if (cal_cw_frame == 63) {
        cal_base_show();
    } else {
        appraisal_base_show();
    }
}
