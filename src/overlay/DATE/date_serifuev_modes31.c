/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x8014FD90  Size: 132 bytes (33 instructions) */
#include "types.h"

extern s32 device_cmd_send(s32);
extern void bustup_modes31_step(void);
extern s32 func_8014C530(void);
extern u16 now_serifuev_in;
extern s32 step_counter;
extern u8 now_sub_sub_step;

s32 date_serifuev_modes31(void) {
    s32 v0;
    v0 = func_8014C530();
    if (now_serifuev_in == 2) {
        v0 = step_counter;
        bustup_modes31_step();
        now_sub_sub_step = now_sub_sub_step - 1;
        step_counter = v0 + 1;
        if (v0 == 1) {
            device_cmd_send(513);
            v0 = device_cmd_send(1282);
        }
    }
    return v0;
}
