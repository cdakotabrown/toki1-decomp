/* Address: 0x8009F970, Size: 44 bytes */
/* IDO `cc -S` stream (`@Compiler: ido52-gas`). In that stream ugen copies */
/* @Compiler: ido52-gas */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=0:v1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0x8 */
#include "types.h"
extern void GPU_cw(unsigned int arg);
void set_jtb(unsigned int a) {
    GPU_cw(a & 0x00FFFFFFu);
}
