/* Address: 0x800A24FC */
/* Size: 432 bytes (108 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 (assembled by ASPSX in retail) */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_NO_JUMP_MERGE=all IDO52_FORCE_WEB_COLOR=31:v1 IDO52_AS1_KEEP_DEAD_RELOAD=0 IDO52_AS1_TIE_REJECT=39,40,41,42,45,47 IDO52_AS1_NO_COPY_PROP=1 IDO52_AS1_NO_JUMP_CHAIN=10 IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_FTF_BACKWARD=occ:0 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_stack_complete --old 24 --new 32 */
/* @PostProc: remap_stack_slots --map '20:28,24:32' --keep-frame */
/* @PostProc: convert_or_to_addu */
#include "types.h"

typedef struct {
    void (*fn)(s32, s32);
    s32 a0;
    s32 a1;
    u8 pad[84];
} GpuQEnt;

extern volatile u32 *libgpu_gp0_reg_ptr;
extern volatile u32 *GPU_status;
extern volatile u32 *libgpu_dma2_chcr_ptr;
extern volatile u32 *libgpu_dma_dpcr_reg_ptr;
extern volatile s32 _qin;
extern volatile s32 _qout;
extern GpuQEnt _que[];
extern u8 libgpu_gp1_shadow_tbl[];
extern s32 libgpu_saved_intr_mask;

extern s32 SetIntrMask(s32);
extern u32 _param(s32);
extern void memset(u8 *, s32, s32);

s32 _reset(s32 mode)
{
    volatile u32 *stat;
    volatile u32 *chcr;
    volatile u32 *gp1;
    volatile u32 *gp0;
    s32 i;
    s32 r;

    libgpu_saved_intr_mask = SetIntrMask(0);
    _qout = 0;
    _qin = _qout;
    for (i = 0; i < 64; i++) {
        _que[i].fn = 0;
    }
    goto test;

case0:
    stat = libgpu_dma_dpcr_reg_ptr;
    chcr = libgpu_dma2_chcr_ptr;
    gp1 = GPU_status;
    *chcr = 1025;
    *stat |= 0x800;
    *gp1 = 0;
    memset(libgpu_gp1_shadow_tbl, 0, 256);
    goto done;

case1:
    stat = libgpu_dma_dpcr_reg_ptr;
    chcr = libgpu_dma2_chcr_ptr;
    gp1 = GPU_status;
    *chcr = 1025;
    *stat |= 0x800;
    *gp1 = 0x02000000;
    *gp1 = 0x01000000;
    goto done;

test:
    switch (mode) {
    case 0:
        goto case0;
    case 1:
        goto case1;
    }
done:

    SetIntrMask(libgpu_saved_intr_mask);
    if (mode == 0) {
        if (_param(7) == 2) {
            return 3;
        }
        gp1 = GPU_status;
        gp0 = libgpu_gp0_reg_ptr;
        if (1) {
            *gp0 = (*gp1 & 0x3FFF) | 0xE1001000;
            *gp0;
            if (*gp1 & 0x1000) {
                r = 1;
            } else {
                r = 0;
            }
            return r;
        }
    }
    return 0;
}
