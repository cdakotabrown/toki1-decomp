/* @CompileEnv: IDO52_AS1_NO_LA_PULL=3 IDO52_AS1_NO_FALLTHRU_FILL=0 */
/* name_ent_sei_input - NAME_ENT overlay, Address: 0x801358A4, Size: 676 bytes (169 insns) */
/* @PostProc: patch_insn_reg_field 0x80:rt:t9:t8 0x98:rt:t7:v0 0xa4:rt:t7:v0 0xac:rt:t8:t7 0xb8:rt:t8:t7 0xbc:rs:t9:t8 0xc0:rt:t7:t6 0xcc:rt:t0:t9 0xdc:rs:t0:t9 0xdc:rt:t0:t9 0xe0:rt:t1:t0 0xe4:rs:t0:t9 0xec:rs:t1:t0 0xec:rt:t1:t0 0xf4:rs:t1:t0 0x164:rt:t2:t1 0x168:rs:t2:t1 0x168:rt:t2:t1 0x170:rs:t2:t1 0x194:rt:t3:t2 0x198:rs:t3:t2 0x198:rt:t3:t2 0x1a0:rs:t3:t2 0x1cc:rt:t4:t3 0x1d0:rs:t4:t3 0x1d0:rt:t4:t3 0x1d8:rs:t4:t3 0x204:rt:t5:t4 0x208:rs:t5:t4 0x208:rt:t5:t4 0x210:rs:t5:t4 0x238:rt:t6:t5 0x23c:rs:t6:t5 0x23c:rt:t6:t5 0x244:rs:t6:t5 0x27c:rs:t7:t6 */
#include "types.h"

extern u32 name_ent_tab_no;
extern volatile u8 name_ent_cell_no;
extern u32 pad_trig;
extern s32 name_ent_upper_col;
extern s32 name_ent_upper_row;
extern u32 name_ent_tab_hit;
extern u32 name_ent_sei_ask_str[];

extern void step_directional_scroll_once();
extern void name_ent_cell_set();
extern void name_ent_tab_index_set();
extern void name_ent_hilite_set();
extern void name_ent_key_cursor_set();
extern void name_ent_lower_cell_set();
extern void name_ent_index_set();
extern void name_ent_arrow_step();
extern void name_ent_glyph_write();
extern void name_ent_names_disp();
extern void name_ent_desc_disp();
extern void name_ent_caption_disp();
extern void name_ent_hira_disp();
extern void name_ent_kata_disp();
extern void name_ent_default_set();
extern void k_sub_reset_point_set();
extern void device_cmd_send();
extern void sub_sub_step_up();

void name_ent_sei_input(void)
{
    u32 input;
    u32 subState;

    step_directional_scroll_once();
    name_ent_cell_set();
    name_ent_tab_index_set();
    name_ent_hilite_set();
    name_ent_key_cursor_set(0);

    if (name_ent_tab_no == 3) {
        name_ent_lower_cell_set();
        name_ent_index_set();
    }

    name_ent_arrow_step();

    input = pad_trig;

    if (input & 0x0040) {
        subState = name_ent_cell_no;
        if (subState == 12) {

            subState = 2;
            name_ent_cell_no = subState;
            return;
        }
        if (subState != 0) {
            name_ent_cell_no = subState - 1;
        }
        return;
    }

    if (input & 0x0020) {
        subState = name_ent_cell_no;
        if ((u32)subState < 3) {
            if (name_ent_upper_col >= 0 && name_ent_upper_row >= 0) {
                name_ent_glyph_write(0);
                name_ent_names_disp();
            }
        }

        if (name_ent_tab_no == 3) {
            name_ent_desc_disp(1);
            k_sub_reset_point_set();
            name_ent_names_disp();
        } else if (name_ent_tab_no == 2) {
            name_ent_caption_disp(name_ent_sei_ask_str);
            k_sub_reset_point_set();
            name_ent_names_disp();
        }

        if (name_ent_tab_hit == 0) {
            name_ent_tab_no = 0;
            name_ent_hira_disp(name_ent_sei_ask_str);
            k_sub_reset_point_set();
            name_ent_names_disp();
        }
        if (name_ent_tab_hit == 1) {
            name_ent_tab_no = 1;
            name_ent_kata_disp(name_ent_sei_ask_str);
            k_sub_reset_point_set();
            name_ent_names_disp();
        }
        if (name_ent_tab_hit == 2) {
            name_ent_tab_no = 2;
            name_ent_caption_disp(name_ent_sei_ask_str);
            k_sub_reset_point_set();
            name_ent_names_disp();
        }
        if (name_ent_tab_hit == 3) {
            name_ent_tab_no = 3;
            name_ent_desc_disp(1);
            k_sub_reset_point_set();
            name_ent_names_disp();
        }
        if (name_ent_tab_hit == 4) {
            name_ent_tab_no = 4;
            name_ent_default_set(0);
            name_ent_names_disp();
            device_cmd_send(1281);
            sub_sub_step_up();
        }
        return;
    }

    if (input & 0x0800) {
        name_ent_default_set(0);
        name_ent_names_disp();
        device_cmd_send(1281);
        sub_sub_step_up();
    }
}
