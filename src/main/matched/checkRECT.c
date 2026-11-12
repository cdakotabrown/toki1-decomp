/* Address: 0x8009FF08 */
/* Size: 360 bytes (90 instructions) */
/* Region: Library, IDO 5.2 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=2:t0 IDO52_UGEN_DEST_TEMP=10,17,19,26 IDO52_UGEN_TEMP_REG=10:t7 IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_NO_FALLTHRU_FILL=occ:1,3 IDO52_AS1_FTF_BACKWARD=occ:0 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct { s16 x, y, w, h; } Rect16;

extern s16 D_800EBD64;
extern s16 D_800EBD66;
extern u8  D_800EBD62;
extern s32 (*GPU_printf)(const char *fmt, ...);
extern const char libgpu_badrect_fmt[];
extern const char libgpu_badrect_rect_fmt[];
extern const char libgpu_rectdump_name_fmt[];
extern const char libgpu_rectdump_rect_fmt[];

void checkRECT(s32 arg0, Rect16 *r) {
    goto dispatch;
case_1:
    if (D_800EBD64 < r->w || D_800EBD64 < r->w + r->x ||
        D_800EBD66 < r->y || D_800EBD66 < r->y + r->h ||
        r->w <= 0 || r->x < 0 || r->y < 0 || r->h <= 0) {
        GPU_printf(libgpu_badrect_fmt, arg0);
        GPU_printf(libgpu_badrect_rect_fmt, r->x, r->y, r->w, r->h);
    }
    return;
case_2:
    GPU_printf(libgpu_rectdump_name_fmt, arg0);
    GPU_printf(libgpu_rectdump_rect_fmt, r->x, r->y, r->w, r->h);
    return;
dispatch:
    if (D_800EBD62 == 1) goto case_1;
    if (D_800EBD62 == 2) goto case_2;
}
