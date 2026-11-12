/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=2:v0 IDO52_UGEN_TEMP_REG_INPLACE=occ:2 */
/* Address: 0x8014177C, Size: 268 bytes (67 insns) */
#include "types.h"

extern s32 device_cmd_send();
extern s32 k_reset();
extern s32 check_end_k();
extern s32 geko_serifuev_run();
extern s32 geko_serifuev_run2();

extern u16 now_serifuev_in;
extern s32 step_counter;
extern u8 step_mode;

s32 geko_serifuev_main2()
{
    if (check_end_k() != 0 && now_serifuev_in == 1) {
        if (step_mode++ < 64) {
            return 0;
        }
        step_mode = 0;
        k_reset(1);
    }

    if (now_serifuev_in == 0 || now_serifuev_in == 8) {
        geko_serifuev_run2();
    } else {
        geko_serifuev_run();
    }

    if (step_counter == 1 && now_serifuev_in == 1) {
        device_cmd_send(1545);
    }
    if (!(step_counter++ ^ 1) && now_serifuev_in == 9) {
        device_cmd_send(1547);
    }
}
