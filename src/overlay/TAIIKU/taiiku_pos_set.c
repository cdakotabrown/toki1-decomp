/* Address: 0x80136C14 */
/* Size: 156 bytes (39 insns) */
/* @SymbolSize: 156 */
#include "types.h"
#include "tokimeki/taiiku_kari_run.h"

extern s16 taiiku_kari_scrl_hi;
extern s32 taiiku_kari_scrv;
extern u8 taiiku_kari_run[];
extern s16 cal_cw_x;

void taiiku_pos_set(void) {
    s16 y;

    if (((TAIIKU_KARI_RUN *)taiiku_kari_run)[0].accel < 0 || taiiku_kari_scrl_hi == 0) {
        ((TAIIKU_KARI_RUN *)taiiku_kari_run)[0].pos.fix = ((TAIIKU_KARI_RUN *)taiiku_kari_run)[0].pos.fix - taiiku_kari_scrv;
    } else {
        ((TAIIKU_KARI_RUN *)taiiku_kari_run)[0].pos.fix = ((TAIIKU_KARI_RUN *)taiiku_kari_run)[0].pos.fix + 0x8000;
    }

    y = *(volatile s16 *)(taiiku_kari_run + 34);
    if (y >= -31) {
        *(volatile s16 *)(taiiku_kari_run + 34) = -32;
        y = *(volatile s16 *)(taiiku_kari_run + 34);
    }

    if (y < -159) {
        *(volatile s16 *)(taiiku_kari_run + 34) = -160;
        y = *(volatile s16 *)(taiiku_kari_run + 34);
    }

    cal_cw_x = y;
}
