/* Address: 0x80067F04 */
/* Size: 40 bytes (10 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* `@CompileEnv: IDO52_PRECOLOR_WEB=v0:0` (Hook 18, colour-only pin, no */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0x0:rt:v1:v0 0x4:rs:v1:v0 */
#include "types.h"

extern u8 cw97_mode;

u8 bustup2_flag_on(void) {
    u8 v = cw97_mode;

    if ((v & 1) == 0) {
        v = v | 3;
        cw97_mode = v;
    }
    return v;
}
