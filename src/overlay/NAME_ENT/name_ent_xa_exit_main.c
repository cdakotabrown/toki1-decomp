/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x80140E18 */
/* @SymbolSize: 84 */
/* @CompileFlags: -cckr */
#include "types.h"

extern s32 name_ent_xa_step();
extern s32 name_ent_exit();
extern u8 now_sub_sub_step;

s32 name_ent_xa_exit_main()
{
    s32 ret = now_sub_sub_step;

    switch (ret) {
    case 0:
        ret = name_ent_xa_step();
        break;
    case 1:
        ret = name_ent_exit();
        break;
    }
    return ret;
}
