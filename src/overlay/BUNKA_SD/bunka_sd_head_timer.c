/* Address: 0x801390C4 (true symbol start 0x801390BC, cross-prologue head) */
/* Size: 324 bytes (81 insns incl. the 2 hoisted head words) */
/* @SymbolStart: 0x801390BC */
/* @SymbolSize: 324 */
#include "types.h"

extern u32 check_head_position(void);
extern void device_cmd_send(s32);

typedef struct {
    u32 base;
    u32 step;
} StepTimer;

extern StepTimer step_state;
extern u8 now_year;

s32 bunka_sd_head_timer(void)
{
    u32 step;

    step = step_state.step;
    if (step == 188) {
        step_state.base = check_head_position() - 4096;
        step = step_state.step;
    }
    if (step == 190) {
        device_cmd_send(768);
        step = step_state.step;
    }
    if (step < 192) {
        return;
    }
    if (step >= 0x70000000) {
        return;
    }

    switch (now_year) {
    case 95:
        if ((check_head_position() - step_state.base) >= 9737) {
            step_state.step = 0x70000000;
        }
        break;
    case 96:
        if ((check_head_position() - step_state.base) >= 10993) {
            step_state.step = 0x70000000;
        }
        break;
    case 97:
    default:
        if ((check_head_position() - step_state.base) >= 10217) {
            step_state.step = 0x70000000;
        }
        break;
    }
}
