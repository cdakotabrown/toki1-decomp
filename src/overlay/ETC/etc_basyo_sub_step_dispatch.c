/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x30:rs:v0:v1 0x38:rs:v0:v1 */
/* @SymbolSize: 176 */
/* Address: 0x801320A0 */
/* Size: 176 bytes (44 instructions) */
#include "types.h"

extern void parameter_show(void);
extern void hizuke_show(void);
extern void message_window_show(void);
extern void basyo_show(void);
extern void update_anim_state(s32);
extern void step_directional_scroll_once(void);
extern void hizuke_basyo_step(void);
extern void etc_basyo_main1(void);
extern void xa_basyo_step(void);
extern void etc_basyo_main3(void);
extern u8 now_sub_step;

void etc_basyo_sub_step_dispatch(void)
{
    s32 v1 = now_sub_step;

    switch (v1) {
    case 0:
        hizuke_basyo_step();
        break;
    case 1:
        etc_basyo_main1();
        break;
    case 2:
        xa_basyo_step();
        break;
    case 3:
        etc_basyo_main3();
        break;
    }
    message_window_show();
    parameter_show();
    hizuke_show();
    update_anim_state(0);
    step_directional_scroll_once();
    basyo_show();
}
