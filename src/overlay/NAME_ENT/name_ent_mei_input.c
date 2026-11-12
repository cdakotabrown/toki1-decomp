/* @PostProc: scheduler_normalizer --const-fp --store-fp --block-aware */
/* @PostProc: patch_insn_reg_field 0x88:rt:v1:v0 0x90:rs:v1:v0 0x98:rt:v1:v0 0xa0:rt:v1:v0 0xa4:rs:v1:v0 0xac:rs:v1:v0 0xc0:rt:t7:t6 0xc4:rt:t0:v0 0xc8:rs:t9:v0 0xc8:rt:t9:v0 0xd0:rs:t9:v0 0xdc:rs:t0:t9 0xdc:rt:t0:t9 0xe0:rt:t1:t0 0xe4:rs:t0:t9 0xec:rs:t1:t0 0xec:rt:t1:t0 0xf4:rs:t1:t0 0x164:rt:t2:t1 0x168:rs:t2:t1 0x168:rt:t2:t1 0x170:rs:t2:t1 0x194:rt:t3:t2 0x198:rs:t3:t2 0x198:rt:t3:t2 0x1a0:rs:t3:t2 0x1cc:rt:t4:t3 0x1d0:rs:t4:t3 0x1d0:rt:t4:t3 0x1d8:rs:t4:t3 0x204:rt:t5:t4 0x208:rs:t5:t4 0x208:rt:t5:t4 0x210:rs:t5:t4 0x238:rt:t6:t5 0x23c:rs:t6:t5 0x23c:rt:t6:t5 0x244:rs:t6:t5 0x27c:rs:t7:t6 */
/* name_ent_mei_input - NAME_ENT overlay, Address: 0x80135C64, Size: 676 bytes (169 insns) */
#include "types.h"

extern s32 name_ent_tab_no;
extern u8  name_ent_cell_no;
extern u32 pad_trig;
extern s32 name_ent_upper_col;
extern s32 name_ent_upper_row;
extern s32 name_ent_tab_hit;
extern u8  name_ent_mei_ask_str[];

extern void step_directional_scroll_once(void);
extern void name_ent_cell_set(void);
extern void name_ent_tab_index_set(void);
extern void name_ent_hilite_set(void);
extern void name_ent_key_cursor_set(s32);
extern void name_ent_lower_cell_set(void);
extern void name_ent_index_set(void);
extern void name_ent_arrow_step(void);
extern void name_ent_glyph_write(s32);
extern void name_ent_names_disp(void);
extern void name_ent_desc_disp(s32);
extern void k_sub_reset_point_set(void);
extern void name_ent_caption_disp(u8 *);
extern void name_ent_hira_disp(u8 *);
extern void name_ent_kata_disp(u8 *);
extern void name_ent_default_set(s32);
extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);

void name_ent_mei_input(void)
{
    u32 input;
    s32 sub;

    step_directional_scroll_once();
    name_ent_cell_set();
    name_ent_tab_index_set();
    name_ent_hilite_set();
    name_ent_key_cursor_set(1);

    if (name_ent_tab_no == 3) {
        name_ent_lower_cell_set();
        name_ent_index_set();
    }
    name_ent_arrow_step();

    input = pad_trig;
    if (input & 0x0040) {
        sub = name_ent_cell_no;
        if (sub == 12) {
            sub = 6;
        }
        name_ent_cell_no = sub;
        if ((u32)sub < 4) {
            return;
        }
        name_ent_cell_no = sub - 1;
        return;
    }

    if (input & 0x0020) {
        if ((u32)name_ent_cell_no < 6 && name_ent_upper_col >= 0 && name_ent_upper_row >= 0) {
            name_ent_glyph_write(1);
            name_ent_names_disp();
        }

        if (name_ent_tab_no == 3) {
            name_ent_desc_disp(2);
            k_sub_reset_point_set();
            name_ent_names_disp();
        } else if (name_ent_tab_no == 2) {
            name_ent_caption_disp(name_ent_mei_ask_str);
            k_sub_reset_point_set();
            name_ent_names_disp();
        }

        if (name_ent_tab_hit == 0) {
            name_ent_tab_no = 0;
            name_ent_hira_disp(name_ent_mei_ask_str);
            k_sub_reset_point_set();
            name_ent_names_disp();
        }
        if (name_ent_tab_hit == 1) {
            name_ent_tab_no = 1;
            name_ent_kata_disp(name_ent_mei_ask_str);
            k_sub_reset_point_set();
            name_ent_names_disp();
        }
        if (name_ent_tab_hit == 2) {
            name_ent_tab_no = 2;
            name_ent_caption_disp(name_ent_mei_ask_str);
            k_sub_reset_point_set();
            name_ent_names_disp();
        }
        if (name_ent_tab_hit == 3) {
            name_ent_tab_no = 3;
            name_ent_desc_disp(2);
            k_sub_reset_point_set();
            name_ent_names_disp();
        }
        if (name_ent_tab_hit == 4) {
            name_ent_tab_no = 4;
            name_ent_default_set(1);
            name_ent_names_disp();
            device_cmd_send(1281);
            sub_sub_step_up();
        }
    } else {
        if (input & 0x0800) {
            name_ent_default_set(1);
            name_ent_names_disp();
            device_cmd_send(1281);
            sub_sub_step_up();
        }
    }
}
