/* option_value_step - OPTION overlay, Address: 0x8013A570, Size: 180 bytes (45 insns) */
/* @SymbolSize: 180 */
#include "types.h"

extern u32 pad_trig;
extern u32 pad_data;

s32 option_value_step(s32 cur, s32 max, s32 min)
{
    u32 pressed;
    u32 held;

    pressed = pad_trig;
    if (pressed & 0x80) {
        cur++;
        if (max < cur) {
            cur = min;
        }
        held = pad_data;
    } else {
        held = pad_data;
        if (held & 0x80) {
            if (held & 5) {
                cur++;
                if (max < cur) {
                    cur = min;
                }
            }
        }
    }

    if (pressed & 0x10) {
        cur--;
        if (cur < min) {
            cur = max;
        }
    } else {
        if (held & 0x10) {
            if (held & 5) {
                cur--;
                if (cur < min) {
                    cur = max;
                }
            }
        }
    }

    return cur;
}
