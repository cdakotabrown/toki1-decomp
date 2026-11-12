/* Address: 0x80063A64 */
/* Size: 36 bytes (9 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=0:v1 IDO52_UGEN_TEMP_REG=2:t8 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 bustup_cw_mode;

void bustup_wink(void) {
    u32 v = bustup_cw_mode;

    if ((v & 1) == 0) {
        bustup_cw_mode = (u8)(v | 0x11);
    }
}
