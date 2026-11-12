/* Function: _schedule_main38 */
/* Address: 0x80072A04 */
/* Size: 84 bytes (21 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=0 IDO52_AS1_FORCE_LA_PULL=0 IDO52_AS1_TIE_REJECT=2 IDO52_AS1_TIE_TAKE=4 */
/* @PostProc: strip_leading_instructions --count auto */

#include "types.h"

extern u8 now_sub_sub_step;
extern s32 season_load_step(void);
extern s32 device_load_wait(void);

s32 _schedule_main38(void) {
    s32 val;
    s32 ret;

    val = now_sub_sub_step;
    ret = val;
    if (val == 0) goto c0;
    if (val == 1) goto c1;
    return ret;
c0:
    return season_load_step();
c1:
    return device_load_wait();
}
