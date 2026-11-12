/* @SymbolStart: 0x8013C59C */
/* @SymbolSize: 96 */
/* geko_serifuev_girl_set -- GEKO overlay, Address: 0x8013C5A4, Size: 88 bytes (22 insns) */
#include "types.h"

extern u16 now_serifuev_in;

extern void geko_serifuev_run(void);

extern s8 now_girl;

void geko_serifuev_girl_set(void)
{
    if (now_serifuev_in == 0) {
        now_girl = 14;
    } else if (now_serifuev_in == 1) {
        now_girl = 13;
    } else {
        now_girl = 3;
    }
    geko_serifuev_run();
}
