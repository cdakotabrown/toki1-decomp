/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x80146BB0 */
/* @SymbolSize: 164 */
/* etc_week_bustup_load - ETC overlay, Address: 0x80146BB8 (head at 0x80146BB0). */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 get_CD_State2();
extern s32 cd_read_start2();
extern s32 get_nf();
extern s32 bustup_addr_set3();
extern u8 step_mode;
extern s32 etc_week_bust_lba[];

s32 etc_week_bustup_load()
{
    s32 ret;
    s32 v;

    ret = step_mode;
    switch (step_mode) {
    case 0:
        v = get_nf();
        ret = cd_read_start2(9, 0x801E0000, etc_week_bust_lba[0 - v]);
        step_mode = 1;
        break;
    case 1:
        ret = get_CD_State2();
        if ((ret & 1) != 0) {
            v = get_nf();
            bustup_addr_set3(1 - v);
            ret = sub_sub_step_up();
        }
        break;
    }
    return ret;
}
