/* @SymbolStart: 0x801452B4 */
/* @SymbolSize: 220 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x2c:rt:v1:a0 0x34:rs:v0:v1 0x38:rt:t9:t8 0x38:rs:v0:v1 0x40:rs:v0:v1 0x70:rt:t7:v1 0x74:rs:t7:v1 0x74:rt:t7:v1 0x7c:rs:t7:v1 0x7c:rt:t8:t7 0x84:rt:t8:t7 0x9c:rt:t9:t8 0xa8:rs:v1:a0 0xa8:rt:v1:a0 0xb0:rs:v1:a0 0xb4:rt:t0:v0 0xb8:rs:v1:a0 0xb8:rt:v1:a0 0xbc:rs:t0:v0 0xc0:rt:v1:a0 0xc4:rt:t1:t9 0xc8:rt:t1:t9 0xc8:rs:t1:t9 0xd0:rs:t1:t9 0xd0:rt:t2:t0 0xd8:rt:t2:t0 */
#include "types.h"

extern void back_clear_switch(s32);
extern void name_ent_serifu_set(void);
extern void name_ent_text_scroll(void);
extern void sub_step_up(void);

extern u8 screen_bri;
extern u8 name_ent_serifu_no;
extern u8 step_mode;
extern s32 step_counter;
extern u8 cal_cw_disp;

void name_ent_main0_2(void)
{
    s32 counter;
    s32 cond;

    switch (step_mode) {
    case 0:
        back_clear_switch(0);
        screen_bri = 128;
        name_ent_serifu_no = 0;
        name_ent_serifu_set();
        step_mode = step_mode + 1;
        break;
    case 1:
        name_ent_text_scroll();
        break;
    case 2:
        step_mode = step_mode + 1;
        step_counter = 0;

    case 3:
        counter = step_counter;
        cond = ((u32)counter < 61) ^ 1;
        counter = counter + 1;
        step_counter = counter;
        if (cond != 0) {
            cal_cw_disp = cal_cw_disp | 0x80;
            sub_step_up();
        }
        break;
    }
}
