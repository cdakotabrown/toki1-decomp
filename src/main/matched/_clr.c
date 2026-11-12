/* Address: 0x800A15DC, Size: 544 bytes (136 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=all */
/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_instructions --offset1 0x148 --offset2 0x14c */
#include "types.h"

typedef struct { s16 x, y, w, h; } RECT_;

typedef union {
    RECT_ r;
    struct { u32 xy, wh; } p;
} RECTU_;

extern s16 D_800EBD64;
extern s16 D_800EBD66;
u32 libgpu_clr_packet_buf[12];
extern u32 libgpu_clr_restore_packet[];

extern u32 _param(s32 which);
extern void _cwc(u32 *packet);

s32 _clr(RECT_ *rect, u32 color)
{
    volatile RECTU_ u;

    u.r.x = rect->x;
    u.r.y = rect->y;
    u.r.w = rect->w;
    u.r.h = rect->h;

    if (((RECT_ *)&u)->w < 0) {
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
            s32 v;
            if (D_800EBD66 < u.r.h) {
                v = D_800EBD66;
            } else {
                v = u.r.h;
            }
            u.r.h = v;
        }
    }

    if ((u.r.x & 0x3f) || (u.r.w & 0x3f)) {
        libgpu_clr_packet_buf[0] = ((u32)libgpu_clr_restore_packet & 0xffffff) | 0x07000000;
        libgpu_clr_packet_buf[1] = 0xe3000000;
        libgpu_clr_packet_buf[2] = 0xe4ffffff;
        libgpu_clr_packet_buf[3] = 0xe5000000;
        libgpu_clr_packet_buf[4] = 0xe6000000;
        libgpu_clr_packet_buf[5] = (color & 0xffffff) | 0x60000000;
        libgpu_clr_packet_buf[6] = u.p.xy;
        libgpu_clr_packet_buf[7] = u.p.wh;
        libgpu_clr_packet_buf[8] = 0x03ffffff;
        libgpu_clr_packet_buf[9] = _param(3) | 0xe3000000;
        libgpu_clr_packet_buf[10] = _param(4) | 0xe4000000;
        libgpu_clr_packet_buf[11] = _param(5) | 0xe5000000;
    } else {
        libgpu_clr_packet_buf[0] = 0x04ffffff;
        libgpu_clr_packet_buf[1] = 0xe6000000;
        libgpu_clr_packet_buf[2] = (color & 0xffffff) | 0x02000000;
        libgpu_clr_packet_buf[3] = u.p.xy;
        libgpu_clr_packet_buf[4] = u.p.wh;
    }
    _cwc(libgpu_clr_packet_buf);
    return 0;
}
