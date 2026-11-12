/* Address: 0x80136B24 */
/* Size: 136 bytes (34 insns) */
/* @SymbolSize: 136 */
#include "types.h"
#include "tokimeki/taiiku_kari_run.h"

extern s32 pad_trig;
extern s16 taiiku_kari_btn;
extern s32 taiiku_kari_gain;
extern TAIIKU_KARI_RUN taiiku_kari_run[];

void taiiku_accel_set(void) {
    if (pad_trig & 0x20) {
        taiiku_kari_btn = 1;
    }

    if (taiiku_kari_run[0].press_cnt < 16) {
        return;
    }

    if (taiiku_kari_btn != 0) {
        if (taiiku_kari_run[0].speed < 0) {
            taiiku_kari_run[0].speed = 0;
        }
        taiiku_kari_run[0].accel = taiiku_kari_gain;
    } else {
        taiiku_kari_run[0].accel = -768;
    }

    taiiku_kari_run[0].press_cnt = 0;
    taiiku_kari_btn = 0;
}
