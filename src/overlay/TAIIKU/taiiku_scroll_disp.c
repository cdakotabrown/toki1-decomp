/* @SymbolStart: 0x80136EA0 */
/* @SymbolSize: 220 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=0:v1,2:a0,6:a1 */
#include "types.h"
#include "tokimeki/taiiku_kari_run.h"

extern void scroll_poly_disp(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);

extern s32 taiiku_kari_vmax;
typedef union { s32 w; struct { u16 lo; s16 hi; } h; } FX;
extern FX taiiku_kari_scrl;
extern s32 taiiku_kari_scrv;
extern TAIIKU_KARI_RUN taiiku_kari_run[];

void taiiku_scroll_disp(void)
{
    taiiku_kari_scrv += taiiku_kari_run[0].accel;
    if (taiiku_kari_vmax < taiiku_kari_scrv) {
        taiiku_kari_scrv = taiiku_kari_vmax;
    }
    if (taiiku_kari_scrv < 0 || (taiiku_kari_run[0].accel < 0 && taiiku_kari_run[0].pos.half[1] < -159)) {
        taiiku_kari_scrv = 0;
    }
    taiiku_kari_scrl.w += taiiku_kari_scrv;
    scroll_poly_disp((taiiku_kari_scrl.h.hi >> 1) % 640, 15, 0, 112, 640);
    scroll_poly_disp(taiiku_kari_scrl.h.hi & 0x7F, 15, 112, 128, 128);
}
