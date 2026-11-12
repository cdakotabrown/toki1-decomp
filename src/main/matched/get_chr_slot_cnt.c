/* Address: 0x8006B0C0 */
/* Size: 180 bytes (45 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=1:t7,2:t8,3:t9,4:t1,5:t2,6:t3,7:t5,8:t6,9:t7,10:t9,11:t0,12:t1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u32 screen_w[];

s32 get_chr_slot_cnt(s32 kind) {
    s32 n = 0;
    s32 i;

    for (i = 0; i < 8; i++) {
        if ((s32)(screen_w[339 + i] << 2) < 0) {
            if ((screen_w[339 + i] << 3) >> 28 == kind) {
                n++;
            }
        }
    }
    return n;
}
