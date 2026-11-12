/* Address: 0x800A17FC */
/* Size: 596 bytes (149 instructions) */
/* @CompileFlags: -Wo,-loopunroll,0 -Wo,-do_opt_saved_regs */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=all IDO52_AS1_DIV_POW2=aspsx IDO52_AS1_NO_BRANCH_FOLD=all IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_UGEN_FORCE_TARGET_SAFE_DIV=1 IDO52_AS1_KEEP_DEAD_RELOAD=all IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_UGEN_TEMP_REG=11:a2,14:a2 IDO52_UGEN_TEMP_REG_INPLACE=occ:11,14 IDO52_COPYPROP_SITE=lod:10:deny IDO52_AS1_NO_COPY_PROP=all IDO52_AS1_NO_PASS1_PROP=all IDO52_FORCE_WEB_COLOR=55:a2,64:a1,82:s1,2:a0,24:v1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

typedef struct { s16 x, y, w, h; } RECT_;

typedef union {
    RECT_ r;
    struct { u32 xy, wh; } p;
} RECTU_;

extern s16 D_800EBD64;
extern s16 D_800EBD66;

extern volatile u32 *libgpu_gp0_reg_ptr;
extern volatile u32 *GPU_status;
extern u32 *libgpu_dma2_madr_ptr;
extern u32 *libgpu_dma2_bcr_ptr;
extern u32 *libgpu_dma2_chcr_ptr;

extern void set_alarm(void);
extern s32 get_alarm(void);

s32 _dws(RECT_ *rect, u32 *p)
{
    s32 n;
    s32 cnt;
    volatile u32 *gp1;
    volatile u32 *port;
    volatile RECTU_ u;

    set_alarm();

    u.r.x = rect->x;
    u.r.y = rect->y;
    u.r.w = rect->w;

    ((RECT_ *)&u)->h = rect->h;

    if (u.r.w < 0) {
        u.r.w = 0;
    } else {
        {
            s32 v;
            if (D_800EBD64 < u.r.w) {
                v = D_800EBD64;
            } else {
                v = u.r.w;
            }
            u.r.w = v;
        }
    }
    if (u.r.h < 0) {
        u.r.h = 0;
    } else {
        {
            if (D_800EBD66 < u.r.h) {
                cnt = D_800EBD66;
            } else {
                cnt = u.r.h;
            }
            u.r.h = cnt;
        }
    }

    n = (u.r.w * u.r.h + 1) / 2;
    if (n <= 0) {
        return -1;
    }

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
    *port = 0xa0000000;
    *port = u.p.xy;
    *port = u.p.wh;

    cnt = n % 16;
    while (cnt--) {
        *port = *p++;
    }

    cnt = n / 16;
    if (cnt != 0) {
        *gp1 = 0x04000002;
        *libgpu_dma2_madr_ptr = (u32)p;
        *libgpu_dma2_bcr_ptr = (cnt << 16) | 0x10;
        *libgpu_dma2_chcr_ptr = 0x01000201;
    }
    return 0;
}
