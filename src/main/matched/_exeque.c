/* Address: 0x800A2264 */
/* Size: 664 bytes */
/* Region: Library (0x80086000+), IDO 5.2 -- but see the -O1 finding below. */
/* @SymbolStart: 0x800A2264 */
/* @SymbolSize: 664 */
/* @CompileFlags: -Wc,-pic0 -Wo,-do_opt_saved_regs */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=96 IDO52_AS1_NO_BRANCH_FOLD=all IDO52_AS1_NO_FALLTHRU_FILL=1,2,3,5,6 IDO52_AS1_NO_LA_PULL=all IDO52_COPYPROP_SITE=lod:1:deny IDO52_UGEN_DEST_TEMP=39 IDO52_AS1_TIE_REJECT=17 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

typedef struct {
    void (*fn)(s32, s32);
    s32 a0;
    s32 a1;
    u8 pad[84];
} GpuQEnt;

extern volatile u32 *libgpu_dma2_chcr_ptr;
extern volatile u32 *GPU_status;
extern volatile s32 _qin;
extern volatile s32 _qout;
extern s32 libgpu_null_func_count;
extern char libgpu_exeque_null_msg[];
extern volatile GpuQEnt _que[];
extern s32 libgpu_drawsync_cb_pending;
extern void (*libgpu_drawsync_callback)(void);
extern s32 libgpu_exeque_saved_mask;

extern s32 SetIntrMask(s32);
extern s32 DMACallback(s32, s32);
extern void _reset(s32);
extern void printf(const char *, ...);

s32 _exeque(void)
{
    volatile u32 *stat;

    if (*libgpu_dma2_chcr_ptr & 0x1000000) {
        return 1;
    }
    libgpu_exeque_saved_mask = SetIntrMask(0);
    if (_qin == _qout) goto end;
    if (*libgpu_dma2_chcr_ptr & 0x1000000) goto end;
loop:
    if (_que[_qout].fn != 0) {
        if (_qin == ((_qout + 1) & 0x3F)) {
            if (libgpu_drawsync_callback == 0) {
                DMACallback(2, 0);
            }
        }
        stat = GPU_status;
        while (!(*stat & 0x10000000)) {
        }
        _que[_qout].fn(_que[_qout].a0, _que[_qout].a1);
        _que[_qout].fn = 0;
        _qout = (_qout + 1) & 0x3F;
    } else {
        _reset(1);
        printf(libgpu_exeque_null_msg);
        libgpu_null_func_count++;
        goto end;
    }
    if (_qin != _qout && !(*libgpu_dma2_chcr_ptr & 0x1000000)) goto loop;
end:
    SetIntrMask(libgpu_exeque_saved_mask);
    if (libgpu_drawsync_cb_pending != 0) {
        if (libgpu_drawsync_callback != 0) {
            libgpu_drawsync_cb_pending = 0;
            libgpu_drawsync_callback();
        }
    }
    return 0;
}
