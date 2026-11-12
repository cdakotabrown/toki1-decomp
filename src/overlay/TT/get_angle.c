/* @PostProc: convert_or_to_addu */
/* @CompileEnv: IDO52_AS1_DIV_MACRO=aspsx IDO52_AS1_DIV_MACRO_MUL_PASS=all IDO52_AS1_XBB_SKIP_CAND=0,1 IDO52_AS1_FORCE_BOPT=2,3 IDO52_AS1_NO_FALLTHRU_FILL=occ:4 */
/* Address: 0x8014D250, Size: 444 bytes (111 insns) */
#include "types.h"

extern s16 tt_atan_tbl[];

s32 get_angle(s32 x, s32 y)
{
    s32 negY;
    s32 negX;
    s32 r;

    negY = 0;
    negX = 0;
    if (y < 0) {
        negY = 1;
        y = -y;
    }
    if (x < 0) {
        negX = 1;
        x = -x;
    }
    if (y == 0 && x == 0) {
        return 0;
    }
    if (x < y) {
        if (x & 0x7FE00000) {
            r = tt_atan_tbl[x / (y >> 10)];
        } else {
            r = tt_atan_tbl[(x << 10) / y];
        }
    } else {
        if (y & 0x7FE00000) {
            r = 1024 - tt_atan_tbl[y / (x >> 10)];
        } else {
            r = 1024 - tt_atan_tbl[(y << 10) / x];
        }
    }
    if (negY) {
        r = 2048 - r;
    }
    if (negX) {
        r = -r;
    }
    return r;
}
