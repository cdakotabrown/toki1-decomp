/* @SymbolStart: 0x8013C9C8 */
/* @SymbolSize: 504 */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: patch_stack_complete --old 64 --new 72 */
/* @PostProc: remap_stack_slots --from 40 --to 48 --from 16 --to 24 */
#include "types.h"

typedef struct { u16 h[11]; } RecU;
typedef struct { s16 h[11]; } RecS;

extern RecU taiiku_cw97_frame_set_tbl0;
extern RecS taiiku_cw97_frame_set_tbl1;
extern s32 taiiku_tama_girl;
extern s32 taiiku_tama_spd;
extern s32 taiiku_tama_fall;
extern volatile s32 taiiku_tama_gfrm;
extern s32 taiiku_tama_gstep;
extern u16 taiiku_tama_gfrm_tick;
extern s16 cw97_frame;
extern u8 cw97_disp;

void taiiku_cw97_frame_set(void) {
    RecU buf_a;
    RecS buf_b;
    s32 v;
    s32 c;
    s32 a1v;

    v = taiiku_tama_girl;
    buf_a = taiiku_cw97_frame_set_tbl0;
    buf_b = taiiku_cw97_frame_set_tbl1;

    if ((u32)buf_a.h[v] < taiiku_tama_gfrm_tick) {
        if (v != 2 || (v == 2 && taiiku_tama_spd > 0) || taiiku_tama_fall != 0) {
            c = taiiku_tama_gfrm;
            if (c >= 3 && taiiku_tama_fall != 0 &&
                (taiiku_tama_gstep == 2 || taiiku_tama_gstep == 4 || taiiku_tama_gstep == 6)) {
                c = 0;
            } else if (c >= 5) {
                c = 0;
            } else {
                c = c + 1;
            }
            taiiku_tama_gfrm_tick = 0;
            taiiku_tama_gfrm = c;
        }
    }

    c = taiiku_tama_gfrm;
    cw97_frame = (s16)(v * 6 + c + 6);
    if (taiiku_tama_fall != 0) {
        a1v = taiiku_tama_gstep;
        if (a1v != 2 && a1v != 4) {
            taiiku_tama_gfrm = c;
            if (a1v != 6) {
                return;
            }
        }
        if (c >= 3) {
            c = 0;
        }
        cw97_frame = (s16)(buf_b.h[v] + c);
        taiiku_tama_gfrm = c;
        if (v == 2 && cw97_disp == 165) {
            cw97_frame = (s16)(cw97_frame + 4);
        }
    }
}
