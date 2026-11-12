/* Function: _drs */
/* Address: 0x800A1A50 */
/* Size: 712 bytes (178 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* - `@CompileFlags: -Wc,-pic1` -- retail's frame is 72, ours was 56.  The */
/* the as1 walls above are pipeline artefacts.  BUT `@Compiler: ido52-gas` */
/* @SymbolStart: 0x800A1A50 */
/* @SymbolSize: 712 */
/* @CompileFlags: -Wo,-do_opt_saved_regs */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=all IDO52_AS1_DIV_POW2=aspsx IDO52_AS1_NO_BRANCH_FOLD=all IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_UGEN_FORCE_TARGET_SAFE_DIV=1 IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_NO_COPY_PROP=all IDO52_AS1_NO_PASS1_PROP=all IDO52_UGEN_TEMP_REG=29:a0,14:a2,17:a2,26:a2 IDO52_UGEN_TEMP_REG_INPLACE=29,14,17,26 IDO52_FORCE_WEB_COLOR=2:a0,66:a1,57:a2,24:v1 IDO52_AS1_NO_BOPT=occ:2 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

typedef struct { s16 x, y, w, h; } GpuRect;
typedef union { GpuRect r; struct { u32 xy, wh; } p; } GpuRectU;

extern s16 D_800EBD64;
extern s16 D_800EBD66;
extern volatile u32 *libgpu_gp0_reg_ptr;
extern volatile u32 *GPU_status;
extern u32 *libgpu_dma2_madr_ptr;
extern u32 *libgpu_dma2_bcr_ptr;
extern u32 *libgpu_dma2_chcr_ptr;
extern void set_alarm(void);
extern s32 get_alarm(void);

s32 _drs(GpuRect *rect, u32 *dst)
{
    volatile GpuRectU u; s32 n; s32 cnt; volatile u32 *gp1; volatile u32 *port;

    set_alarm();

    u.r.x = rect->x;
    u.r.y = rect->y;
    u.r.w = rect->w;
    ((GpuRect *)&u)->h = rect->h;

    u.r.w = (u.r.w < 0) ? 0 : ((u.r.w > D_800EBD64) ? D_800EBD64 : u.r.w);
    if (u.r.h < 0) {
        u.r.h = 0;
    } else {
        if (D_800EBD66 < u.r.h) {
            cnt = D_800EBD66;
        } else {
            cnt = u.r.h;
        }
        u.r.h = cnt;
    }

    n = (u.r.w * u.r.h + 1) / 2;
    if (n <= 0) {
        return -1;
    }
    cnt = n % 16;

    gp1 = GPU_status;
    while (!(*gp1 & 0x04000000)) {
        if (get_alarm()) {
            return -1;
        }
        gp1 = GPU_status;
    }
    port = libgpu_gp0_reg_ptr;
    *gp1 = 0x04000000;
    *port = 0x01000000;
    *port = 0xC0000000;
    *port = u.p.xy;
    *port = u.p.wh;

    if (!(*gp1 & 0x08000000)) {
        do {
            if (get_alarm()) {
                return -1;
            }
            gp1 = GPU_status;
        } while (!(*gp1 & 0x08000000));
        port = libgpu_gp0_reg_ptr;
    }

    while (cnt--) {
        *dst++ = *port;
    }

    cnt = n / 16;
    if (cnt != 0) {
        *gp1 = 0x04000003;
        *libgpu_dma2_madr_ptr = (u32)dst;
        *libgpu_dma2_bcr_ptr = (cnt << 16) | 0x10;
        *libgpu_dma2_chcr_ptr = 0x01000200;
    }
    return 0;
}
