/* Address: 0x80138594 */
/* Size: 136 bytes (34 insns) */
/* @SymbolSize: 128 */
#include "types.h"

extern u8 taco_load_no;
extern u8 taco_load_state;
extern u8 taco_load_res;

extern s32 device_load_poll2();
extern void taco_scene_step();

void taco_load_poll2(void)
{
    s32 result;

    taco_scene_step();

    if (taco_load_no == 0 && taco_load_state == 0) {
        taco_load_state = 1;
    }

    if (taco_load_state == 1) {
        result = device_load_poll2();
        taco_load_res = result;
        if ((result & 0xFF) == 1) {
            taco_load_state = 2;
        }
    }
}
