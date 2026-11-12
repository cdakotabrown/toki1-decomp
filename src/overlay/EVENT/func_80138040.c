/* @CompileFlags: -cckr -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=2,5 IDO52_AS1_NO_FALLTHRU_FILL=0 */
#include "types.h"

typedef struct { u32 w[42]; } Tbl42;
extern Tbl42 D_801212B8;
extern u8 D_800B1AF6;
extern u8 D_801212B4;
extern u8 D_800EB06F;
extern u8 D_800EB073;
extern u8 D_800EB06E;
extern u8 D_800F19AB;
extern s16 D_800EB084;
extern s32 ev_flag14;

typedef void (*handler_t)();

extern s32 func_80015A64();
extern s32 func_800458E0();

void func_80138040()
{
    int r;
    int sub;
    Tbl42 tbl;

    tbl = D_801212B8;
    ((handler_t)tbl.w[D_800B1AF6])(128);
    if (ev_flag14 != 0) {
        D_800EB06F |= 0x80;
        D_800EB073 = D_800F19AB;
        if (D_801212B4 == 0) {
            D_800EB06E = 3;
        }
        if (D_800EB084 == 6) {
            if (D_801212B4 == 0) {
                D_800EB06E = 0;
            }
        }
        if (func_80015A64() & 4) {
            r = func_800458E0();
        } else {
            r = 0;
        }
        if (r != 0) {
            sub = D_801212B4;
            if (sub == 0) {
                D_800EB06E = 3;
                D_801212B4 = 1;
            }
        }
    } else {
        D_800EB06F &= 0xFF7F;
    }
}
