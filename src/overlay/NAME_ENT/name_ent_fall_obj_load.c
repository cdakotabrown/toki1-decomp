/* @SymbolStart: 0x8014569C */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x5c:rt:t7:v1 0x60:rs:t7:v1 0x60:rt:t7:v1 0x68:rs:t7:v1 0x68:rt:t8:t7 0x70:rt:t8:t7 0x7c:rt:t9:t8 0x80:rs:t9:t8 */
/* @SymbolSize: 232 */
/* name_ent_fall_obj_load - NAME_ENT overlay, Address: 0x801456A4, Size: 224 bytes (56 insns) */
#include "types.h"

extern void hizuke_show(void);
extern void cd_read_start2(s32, s32, s32);
extern s32 get_CD_State2(void);
extern void class_clear_n(s32);
extern void hizuke_disp_switch(s32);
extern void name_ent_fall_obj_init(void);
extern void decdct(s32, s32, s32, s32);
extern void move_255_line(s32, s32, s32);
extern void sub_sub_step_up(void);
extern void dec_bg_show(s32);

extern volatile u8 srn_vram_set_tbl0;
extern volatile u8 step_mode;
extern volatile s32 name_ent_step_cnt;

void name_ent_fall_obj_load(void)
{
    s32 buttons;

    switch (step_mode) {
    case 0:
        srn_vram_set_tbl0 = 1;
        hizuke_show();
        cd_read_start2(9, 0x80162000, 18772);
        step_mode = step_mode + 1;
        break;
    case 1:
        buttons = get_CD_State2();
        if (buttons & 0x0001) {
            class_clear_n(96);
            hizuke_disp_switch(0);
            name_ent_fall_obj_init();
            decdct(320, 0, 640, 160);
            move_255_line(5, 14, 15);
            name_ent_step_cnt = 0;
            sub_sub_step_up();
        } else {
            hizuke_show();
        }
        break;
    }
    dec_bg_show(1);
}
