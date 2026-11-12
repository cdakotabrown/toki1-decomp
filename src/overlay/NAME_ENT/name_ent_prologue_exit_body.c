/* @SymbolStart: 0x8014AD60 */
/* @SymbolSize: 208 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x90:rt:v0:v1 0x94:rs:v0:v1 0x94:rt:v0:v1 0x9c:rs:v0:v1 */
/* name_ent_prologue_exit_body - NAME_ENT overlay, Address: 0x8014AD60 (head absorbed) */
#include "types.h"

extern void CDQ_init(void);
extern void device_state_init(void);
extern void device_cmd_send(s32 a);
extern void device_load_start(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern void k_speed_set(s32 a);
extern void goto_step(s32 a);
extern void goto_sub_step(s32 a);

extern u8 update_anim_state_tbl4;
extern u8 screen_bri;
extern u8 step_mode;
extern u8 srn_vram_set_tbl0;
extern u8 menu_mode;

void name_ent_prologue_exit_body(void)
{
    u32 sel;

    sel = step_mode;
    switch (sel) {
    case 0:
        CDQ_init();
        update_anim_state_tbl4 = 15;
        screen_bri = 128;
        device_state_init();
        device_cmd_send(5);
        device_cmd_send(209);
        device_load_start(0, 0xBFA6, 0xBF87, 0xD3E3, 0xD395, 0xD38B);
        step_mode = (u8)(step_mode + 1);
        break;
    case 1:
    default:
        srn_vram_set_tbl0 = 0;
        k_speed_set(3);
        menu_mode = 1;
        goto_step(52);
        goto_sub_step(255);
        break;
    }
}
