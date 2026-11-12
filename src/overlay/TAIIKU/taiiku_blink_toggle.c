/* Address: 0x801452F4 */
/* Size: 72 bytes (18 insns) */
/* @SymbolSize: 72 */
/* @PostProc: scheduler_normalizer */
#include "types.h"

extern u8 taiiku_ninin_blink;
extern s16 cw97_frame;
extern s16 cw98_frame;

void taiiku_blink_toggle(void) {
    if ((u32)taiiku_ninin_blink >= 15) {
        cw97_frame = cw97_frame ^ 1;
        cw98_frame = cw98_frame ^ 1;
        taiiku_ninin_blink = 0;
    }
}
