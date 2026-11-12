/* Address: 0x801387A0 */
/* Size: 128 bytes (32 insns) */
#include "types.h"

extern u8 taco_load_no;
extern u8 taco_load_state;
extern u8 taco_load_res;

extern s32 device_load_poll2();
extern void taco_scene_step2();

void taco_load_poll(void)
{
    s32 result;

    taco_scene_step2();

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
