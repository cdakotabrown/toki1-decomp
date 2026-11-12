/* @SymbolStart: 0x801460B8 */
/* @SymbolSize: 200 */
/* @PostProc: patch_insn_reg_field 0x18:rt:v0:v1 0x1c:rs:v0:v1 0x1c:rt:v0:v1 0x30:rs:v0:v1 0x3c:rs:v0:v1 0x64:rt:v1:v0 0x68:rs:v1:v0 0x68:rt:v1:v0 0x70:rs:v1:v0 0x80:rt:v1:v0 */
/* etc_white_light_main - ETC overlay, Address: 0x801460C8, Size: 184 bytes (46 insns) */
#include "types.h"

extern s32 sub_step_up();
extern s32 white_light();
extern s32 etc_white_light_step();
extern s32 etc_xa_load();
extern s32 pad_trig;
extern s32 step_state;
extern s32 step_counter;
extern u8 now_sub_sub_step;

void etc_white_light_main(void)
{
    s32 t6 = step_counter;
    s32 a0 = now_sub_sub_step;
    s32 v0;
    s32 t9;
    s32 t1;

    step_counter = t6 + 1;

    switch (a0) {
    case 0:
        etc_white_light_step();
        break;
    case 1:
        etc_xa_load();
        break;
    }

    v0 = step_state;
    if ((u32)v0 < 255U) {
        white_light(255 - v0);
    }

    t9 = pad_trig;
    if ((t9 & 0x860) != 0) {
        sub_step_up();
        return;
    }

    t1 = step_counter;
    if ((u32)t1 < 1801U) {
        return;
    }
    sub_step_up();
}
