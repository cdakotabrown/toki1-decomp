/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u8 pad[19];
    u8 flag;
} Slot;

extern Slot taco_effect_buf[16];
extern Slot taco_effect2_buf[16];

void taco_sprite_flag_clear(sel)
s32 sel;
{
    s32 i;

    if (sel == 0) {
        for (i = 0; i < 16; i++) { taco_effect_buf[i].flag &= ~0x20; }
    } else {
        for (i = 0; i < 16; i++) { taco_effect2_buf[i].flag &= ~0x20; }
    }
}
