/* @CompileEnv: IDO52_FORM_SITE=lda:0:142:0,lda:0:155:0,lda:0:168:0 */
/* @PostProc: swap_instructions --offset1 0x1b8 --offset2 0x1bc */
#include "types.h"

extern u8 space_str[];

extern u8 name_ent_init_sei_fmt[];
extern u8 name_ent_init_mei_fmt[];
extern u8 name_ent_init_adana_fmt[];
extern u8 name_ent_init_str0[];
extern u8 name_ent_init_str1[];
extern u8 name_ent_init_str2[];
extern u8 name_ent_init_end_sei_fmt[];
extern u8 name_ent_init_end_mei_fmt[];
extern u8 name_ent_init_end_adana_fmt[];

u8 name_ent_sei[8] = {0};
u8 name_ent_mei[8] = {0};
extern u8 name_ent_adana[];
extern u8 name_ent_sei_6[];
extern u8 name_ent_mei_6[];
extern u8 name_ent_adana_10[];

extern u8 name_ent_p_mon;
extern u8 name_ent_p_day;
extern u8 name_ent_p_blood;
extern u8 name_ent_fuji_mon;
extern u8 name_ent_fuji_day;
extern u8 name_ent_fuji_blood;

extern u8 name_ent_keep_sei[];
extern u8 name_ent_keep_mei[];
extern u8 name_ent_keep_adana[];
extern u8 name_ent_keep_p_mon;
extern u8 name_ent_keep_p_day;
extern u8 name_ent_keep_p_blood;
extern u8 name_ent_keep_fuji_mon;
extern u8 name_ent_keep_fuji_day;
extern u8 name_ent_keep_fuji_blood;
extern u8 name_ent_entry_mode;

extern u8 name_ent_cell_no;
extern s32 name_ent_sei_xa;
extern s32 name_ent_sei_xa_1;
extern s32 name_ent_yomi_no;
extern s32 name_ent_lower_col;
extern s32 name_ent_lower_row;
extern s32 name_ent_tab_no;
extern u8 name_ent_sei_ask_str[];

extern void memcpy();
extern void printf();
extern void strcpy(u8 *, u8 *);
extern void name_ent_names_disp(void);
extern void name_ent_names_select(void);
extern void name_ent_layout_init(void);
extern void name_ent_hira_disp();
extern void sub_sub_step_up(void);
extern void device_cmd_send();
extern void set_XAsector();
extern void k_sub_reset_point_set(void);

void name_ent_init(void)
{
    u32 mode;
    u8 *p;

    printf(name_ent_init_sei_fmt, name_ent_sei);
    printf(name_ent_init_mei_fmt, name_ent_mei);
    printf(name_ent_init_adana_fmt, name_ent_adana);

    mode = name_ent_entry_mode;
    if (mode >= 2) {
        printf(name_ent_init_str0);
    } else if (mode == 1) {
        printf(name_ent_init_str1);
        memcpy(name_ent_sei, name_ent_keep_sei, 8);
        memcpy(name_ent_mei, name_ent_keep_mei, 8);
        memcpy(name_ent_adana, name_ent_keep_adana, 12);
        name_ent_p_mon = (u32)name_ent_keep_p_mon % 12;
        name_ent_p_day = (u32)name_ent_keep_p_day % 31;
        name_ent_p_blood = name_ent_keep_p_blood & 3;
        name_ent_fuji_mon = (u32)name_ent_keep_fuji_mon % 12;
        name_ent_fuji_day = (u32)name_ent_keep_fuji_day % 31;
        name_ent_fuji_blood = name_ent_keep_fuji_blood & 3;
    } else {
        printf(name_ent_init_str2);
        p = name_ent_sei;
        do {
            strcpy(p, space_str);
            p += 2;
        } while (p < name_ent_sei_6);
        p = name_ent_mei;
        do {
            strcpy(p, space_str);
            p += 2;
        } while (p < name_ent_mei_6);
        p = name_ent_adana;
        do {
            strcpy(p, space_str);
            p += 2;
        } while (p != name_ent_adana_10);
        name_ent_adana[10] = 0;
        name_ent_mei[7] = 0;
        name_ent_mei[6] = 0;
        name_ent_sei[7] = 0;
        name_ent_sei[6] = 0;
        name_ent_p_mon = 255;
        name_ent_p_day = 255;
        name_ent_p_blood = 255;
        name_ent_fuji_mon = 255;
        name_ent_fuji_day = 255;
        name_ent_fuji_blood = 255;
        name_ent_adana[11] = 0;
        name_ent_names_select();
    }

    name_ent_cell_no = 0;
    name_ent_tab_no = 0;
    name_ent_hira_disp(name_ent_sei_ask_str);
    name_ent_yomi_no = 0;
    name_ent_lower_col = -1;
    name_ent_lower_row = -1;
    name_ent_layout_init();
    set_XAsector(name_ent_sei_xa, name_ent_sei_xa_1, 0);
    device_cmd_send(768);
    k_sub_reset_point_set();
    name_ent_names_disp();
    name_ent_entry_mode = 2;
    printf(name_ent_init_end_sei_fmt, name_ent_sei);
    printf(name_ent_init_end_mei_fmt, name_ent_mei);
    printf(name_ent_init_end_adana_fmt, name_ent_adana);
    sub_sub_step_up();
}
