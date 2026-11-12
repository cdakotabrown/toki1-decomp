/* @SymbolSize: 88 */
/* Address: 0x8013B2FC */
/* Size: 80 bytes (20 insns) */
#include "types.h"

extern void taiiku_event_main(void);
extern void taiiku_cw97_frame_set(void);
extern void taiiku_cw97_recover_step(void);
extern u16 taiiku_tama_ghold;
extern s32 taiiku_tama_fall;

void taiiku_cw97_main(void) {
    s32 t6 = taiiku_tama_fall;

    if (t6 == 0) {
        taiiku_event_main();
    } else {
        taiiku_cw97_recover_step();
    }
    if (taiiku_tama_ghold == 0) {
        taiiku_cw97_frame_set();
    }
}
