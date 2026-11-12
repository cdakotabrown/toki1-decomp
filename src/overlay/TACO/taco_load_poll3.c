/* @SymbolSize: 128 */
/* Address: 0x8013836C */
#include "types.h"

extern u8 taco_load_no;
extern u8 taco_load_state;
extern s8 taco_load_res;

extern s32 device_load_poll2(void);
extern void taco_phase_main(void);

void taco_load_poll3(void)
{
    s32 v;

    taco_phase_main();
    if (taco_load_no == 0 && taco_load_state == 0) {
        taco_load_state = 1;
    }
    if (taco_load_state == 1) {
        v = device_load_poll2();
        taco_load_res = (s8)v;
        if ((v & 0xFF) == 1) {
            taco_load_state = 2;
        }
    }
}
