/* Size: 312 bytes */
/* @SymbolSize: 312 */
#include "types.h"

extern s32 taiiku_tama_fall;
extern s16 cal_cw_y;
extern s16 cal_cw_x;
extern s16 taiiku_tama_sy;
extern s16 taiiku_tama_sx;

s32 get_taiiku_zone(void) {
    if (taiiku_tama_fall != 0) {
        return 7;
    }

    if (cal_cw_y < 49 &&
        cal_cw_y >= (taiiku_tama_sy >> 3) + 16 &&
        cal_cw_x <= (taiiku_tama_sx >> 3) + 80 &&
        cal_cw_x >= (taiiku_tama_sx >> 3) - 80) {
        return 1;
    }

    if (cal_cw_y < (taiiku_tama_sy >> 3) + 16 &&
        cal_cw_x > (taiiku_tama_sx >> 3) + 80) {
        return 2;
    }
    if (cal_cw_y < (taiiku_tama_sy >> 3) + 16 &&
        cal_cw_x < (taiiku_tama_sx >> 3) - 80) {
        return 3;
    }
    if (cal_cw_y >= (taiiku_tama_sy >> 3) + 16 &&
        cal_cw_x < (taiiku_tama_sx >> 3) - 80) {
        return 5;
    }
    if (cal_cw_y >= (taiiku_tama_sy >> 3) + 16 &&
        cal_cw_x > (taiiku_tama_sx >> 3) + 80) {
        return 6;
    }
    return 0;
}
