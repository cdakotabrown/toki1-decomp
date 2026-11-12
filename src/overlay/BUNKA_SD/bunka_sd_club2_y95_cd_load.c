/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:2 */
/* @SymbolStart: 0x80136270 */
/* @SymbolSize: 124 */
/* bunka_sd_club2_y95_cd_load - BUNKA_SD overlay, Address: 0x80136270, Size: 124 bytes (31 insns) */
#include "types.h"

extern s32 cd_read_start2(s32, s32, s32);
extern s32 get_CD_State2(void);
extern void sub_sub_step_up(void);

extern u8 step_mode;

s32 bunka_sd_club2_y95_cd_load(void)
{
    u8 sel;

    sel = step_mode;
    if (sel == 0) {
        cd_read_start2(111, 0x80180000, 19545);
        return step_mode = step_mode + 1;
    }

    if (sel == 1) {
        if (get_CD_State2() & 1) {
            sub_sub_step_up();
        }
    }

}
