/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x801365D4 */
/* @SymbolSize: 108 */
/* Address: 0x801365DC, Size: 108 bytes (27 instructions incl. head) */
#include "types.h"

extern s32 option_csr_speed_init(void);
extern s32 option_csr_speed_main(void);
extern s32 option_csr_speed_exit(void);
extern u8 now_sub_sub_step;

s32 option_csr_speed_step(void)
{
    s32 ret;

    ret = now_sub_sub_step;
    switch (now_sub_sub_step) {
        case 0:
            ret = option_csr_speed_init();
            break;
        case 1:
            ret = option_csr_speed_main();
            break;
        case 2:
            ret = option_csr_speed_exit();
            break;
    }
    return ret;
}
