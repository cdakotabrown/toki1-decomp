/* Address: 0x800867C8 */
/* Size: 60 bytes (15 instructions) */
/* Region: Library band (0x80086000+), but game-style IDO code (the */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=1:v1 IDO52_UGEN_TEMP_REG_INPLACE=occ:1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s16 cal_cw_tick;
extern s16 cal_cw_anim;
extern s16 cal_cw_frame;
extern s16 cw98_anim;
extern s16 cw98_frame;

void bustup_mode_swap(void) {
    s16 a = cw98_anim;
    s16 b = cal_cw_anim;

    cal_cw_anim = a;
    cw98_anim = b;
    cal_cw_frame = 0;
    cw98_frame = 0;
    cal_cw_tick = 0;
}
