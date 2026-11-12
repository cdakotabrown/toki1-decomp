/* @PostProc: patch_stack_complete --old 40 --new 56 */
/* @PostProc: remap_stack_slots --from 32 --to 48 --from 36 --to 44 --from 28 --to 56 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0xb0 --offset2 0xb4 */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xbc */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xc8 */
/* @PostProc: patch_insn_reg_field 0x50:rt:v0:t8 0x58:rs:v0:t8 0x58:rt:v0:t8 0x64:rs:v0:t8 0x64:rt:t8:t9 0x68:rs:t8:t9 0x88:rt:t9:t0 0x8c:rs:t9:t0 0x8c:rt:t9:t0 0x94:rs:t9:t0 0x94:rt:t0:t1 0x98:rt:t0:t1 0x9c:rt:t1:t2 0xa0:rs:t1:t2 0xa0:rt:t1:t2 0xa8:rs:t1:t2 0xac:rt:t5:t7 0xb0:rt:t2:t3 0xb4:rt:v0:t5 0xb8:rs:t2:t3 0xb8:rt:t2:t3 0xbc:rs:v0:t5 0xbc:rt:v0:t5 0xc4:rs:t2:t3 0xc4:rt:t3:t4 0xc8:rs:v0:t5 0xc8:rt:t4:t6 0xcc:rs:t4:t6 0xd0:rt:t3:t4 0xf0:rs:t5:t7 0xf0:rt:t5:t7 0xf8:rs:t5:t7 0xf8:rt:t6:t8 0xfc:rt:t6:t8 */
/* name_ent_hizuke_button_step - NAME_ENT overlay, Address: 0x801376B8, Size: 256 bytes */
#include "types.h"

extern u8 cw107_disp;
extern u8 cw106_disp;
extern volatile u32 pad_trig;
extern volatile u8 name_ent_cell_no;

extern void name_ent_hizuke_rand();
extern void sub_sub_step_up();

void name_ent_hizuke_button_step(s32 buttonId)
{
    volatile s32 posYslot;
    volatile s32 posXslot;
    volatile s32 home;
    s32 posY;
    s32 posX;
    u32 hl;
    u32 input;

    posX = posXslot;
    home = buttonId;

    if (posX < -157) {
        goto clear;
    }
    if (posX >= -128) {
        goto clear;
    }
    posY = posYslot;
    if (posY < 81) {
        goto clear;
    }
    if (posY >= 96) {
        goto clear;
    }

    hl = cw107_disp;
    input = pad_trig;
    cw107_disp = hl | 0x80;
    if (input & 0x0020) {
        name_ent_hizuke_rand(buttonId);
        sub_sub_step_up();
    }
    goto substate;

clear:
    cw107_disp = cw107_disp & 0x7F;

substate:
    if (name_ent_cell_no == 3) {
        hl = cw106_disp;
        input = pad_trig;
        cw106_disp = hl | 0x80;
        if (input & 0x0020) {
            name_ent_hizuke_rand(home);
            sub_sub_step_up();
        }
    } else {
        cw106_disp = cw106_disp & 0x7F;
    }
}
