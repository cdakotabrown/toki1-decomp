/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:4 */
/* @SymbolStart: 0x80141F8C */
/* @SymbolSize: 128 */
#include "types.h"

extern s32 name_ent_maind0_0();
extern s32 name_ent_sysfile_remake();
extern s32 name_ent_sysfile_make();
extern s32 step_counter;
extern u8 now_sub_sub_step;

s32 name_ent_maind0()
{
    s32 ret;

    step_counter++;
    ret = now_sub_sub_step;
    switch (now_sub_sub_step) {
    case 0:
        ret = name_ent_maind0_0();
        break;
    case 1:
        ret = name_ent_sysfile_remake();
        break;
    case 2:
        ret = name_ent_sysfile_make();
        break;
    }
    return ret;
}
