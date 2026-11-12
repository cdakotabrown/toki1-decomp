/* geko_sprite_fade_step -- GEKO overlay, Address: 0x80139530, Size: 68 bytes (17 insns), Matched */
#include "types.h"

extern void sprite_fade_main(void);
extern void device_cmd_send(s32 a0);

extern u8 step_mode;

void geko_sprite_fade_step(void)
{
    u8 tmp;
    s32 cond;
    sprite_fade_main();
    tmp = step_mode;
    cond = (u32)tmp < 1U;
    step_mode = tmp + 1;
    if (cond == 0) {
        return;
    }
    device_cmd_send(1281);
}
