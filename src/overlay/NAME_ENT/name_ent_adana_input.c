/* @CompileEnv: IDO52_UGEN_TEMP_REG=2:v0 IDO52_UGEN_TEMP_REG_INPLACE=occ:2 */
/* name_ent_adana_input - NAME_ENT overlay, Address: 0x80136024, Size: 680 bytes (170 insns) */
/* @SymbolSize: 680 */
#include "types.h"

extern void step_directional_scroll_once(void);
extern void name_ent_cell_set(void);
extern void name_ent_tab_index_set(void);
extern void name_ent_hilite_set(void);
extern void name_ent_key_cursor_set(s32 a0);
extern void name_ent_lower_cell_set(void);
extern void name_ent_index_set(void);
extern void name_ent_arrow_step(void);
extern void name_ent_glyph_write(s32 a0);
extern void name_ent_names_disp(void);
extern void name_ent_desc_disp(s32 a0);
extern void k_sub_reset_point_set(void);
extern void name_ent_caption_disp(u8 *a0);
extern void name_ent_hira_disp(u8 *a0);
extern void name_ent_kata_disp(u8 *a0);
extern void name_ent_default_set(s32 a0);
extern void device_cmd_send(s32 a0);
extern void sub_sub_step_up(void);

extern volatile s32 pad_trig;
extern volatile u8 name_ent_cell_no;
extern s32 name_ent_upper_col;
extern s32 name_ent_upper_row;
extern s32 name_ent_tab_no;
extern s32 name_ent_tab_hit;
extern u8  name_ent_adana_ask_str[];

void name_ent_adana_input(void)
{
    u32 v;
    s32 cmd;

    step_directional_scroll_once();
    name_ent_cell_set();
    name_ent_tab_index_set();
    name_ent_hilite_set();
    name_ent_key_cursor_set(2);
    if (name_ent_tab_no == 3) {
        name_ent_lower_cell_set();
        name_ent_index_set();
    }
    name_ent_arrow_step();

    v = pad_trig;
    if (v & 0x40) {
        v = name_ent_cell_no;
        if (v == 12) {
            v = 10;
            name_ent_cell_no = v;
        } else if (v >= 7) {
            name_ent_cell_no = (u8)(v - 1);
        }
    } else if (v & 0x20) {
        v = name_ent_cell_no;
        if (v < 11) {
            if (name_ent_upper_col >= 0) {
                if (name_ent_upper_row >= 0) {
                    name_ent_glyph_write(2);
                    name_ent_names_disp();
                }
            }
        }

        v = name_ent_tab_no;
        if (v == 3) {
            name_ent_desc_disp(3);
            k_sub_reset_point_set();
            name_ent_names_disp();
        } else if (v == 2) {
            name_ent_caption_disp(name_ent_adana_ask_str);
            k_sub_reset_point_set();
            name_ent_names_disp();
        }

        cmd = name_ent_tab_hit;
        if (cmd == 0) {
            name_ent_tab_no = 0;
            name_ent_hira_disp(name_ent_adana_ask_str);
            k_sub_reset_point_set();
            name_ent_names_disp();
        }
        cmd = name_ent_tab_hit;
        if (cmd == 1) {
            name_ent_tab_no = 1;
            name_ent_kata_disp(name_ent_adana_ask_str);
            k_sub_reset_point_set();
            name_ent_names_disp();
        }
        cmd = name_ent_tab_hit;
        if (cmd == 2) {
            name_ent_tab_no = 2;
            name_ent_caption_disp(name_ent_adana_ask_str);
            k_sub_reset_point_set();
            name_ent_names_disp();
        }
        cmd = name_ent_tab_hit;
        if (cmd == 3) {
            name_ent_tab_no = 3;
            name_ent_desc_disp(3);
            k_sub_reset_point_set();
            name_ent_names_disp();
        }
        cmd = name_ent_tab_hit;
        if (cmd == 4) {
            name_ent_tab_no = 4;
            name_ent_default_set(2);
            name_ent_names_disp();
            device_cmd_send(1281);
            sub_sub_step_up();
        }
    } else if (v & 0x800) {
        name_ent_default_set(2);
        name_ent_names_disp();
        device_cmd_send(1281);
        sub_sub_step_up();
    }
}
