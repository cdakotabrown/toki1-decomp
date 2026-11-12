/* @SymbolStart: 0x80133AF4 */
/* @RodataBase: 0x8015D564 */
/* @SymbolSize: 276 */
#include "types.h"

extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32  device_load_poll2(void);
extern void sub_sub_step_up(void);

extern u8 step_mode;

void device_pair_load(void)
{
    u8 sel = step_mode;

    switch (sel) {
    case 0:
        device_load_start(0, 0xC57B, 0xC55B, 0xD31E, 0xD2D0, 0xD2AB);
        step_mode++;
        break;
    case 1:
        if (device_load_poll2() == 1)
            step_mode++;
        break;
    case 2:
        device_load_start(1, 0xC559, 0xC527, 0xC7C9, 0xC785, 0xC774);
        step_mode++;
        break;
    case 3:
        if (device_load_poll2() == 1)
            step_mode++;
        break;
    case 4:
        sub_sub_step_up();
        break;
    }
}
