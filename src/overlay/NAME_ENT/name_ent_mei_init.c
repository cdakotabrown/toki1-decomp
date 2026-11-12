/* name_ent_mei_init - NAME_ENT overlay, Address: 0x80135B48, Size: 284 bytes (71 insns) */
#include "types.h"

extern s32 name_ent_tab_no;
extern u8 name_ent_cell_no;
extern s16 cw97_frame;
extern s16 cw97_x;
extern s16 cw97_y;
extern s32 name_ent_mei_xa[];
extern u8 name_ent_mei_ask_str[];

extern void text_sys_init();
extern void name_ent_hira_disp();
extern void name_ent_kata_disp();
extern void name_ent_caption_disp();
extern void name_ent_desc_disp();
extern void set_XAsector();
extern void device_cmd_send();
extern void k_sub_reset_point_set();
extern void name_ent_names_disp();
extern void sub_sub_step_up();

void name_ent_mei_init(void)
{
    text_sys_init();

    if (name_ent_tab_no == 4) {
        name_ent_tab_no = 0;
    }

    switch (name_ent_tab_no) {
        case 0:
            name_ent_hira_disp(name_ent_mei_ask_str);
            break;
        case 1:
            name_ent_kata_disp(name_ent_mei_ask_str);
            break;
        case 2:
            name_ent_caption_disp(name_ent_mei_ask_str);
            break;
        case 3:
            name_ent_desc_disp(1);
            break;
    }

    name_ent_cell_no = 3;
    cw97_frame = 0;
    cw97_x = 0;
    cw97_y = 0;

    set_XAsector((*(u32 *)&name_ent_mei_xa[0]), (*(u32 *)&name_ent_mei_xa[1]), 0);
    device_cmd_send(768);
    k_sub_reset_point_set();
    name_ent_names_disp();
    sub_sub_step_up();
}
