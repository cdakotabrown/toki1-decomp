/* Address: 0x800A1E40 */
/* Size: 1060 bytes (265 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 (multu-free, `b` never `j`), */
/* @CompileFlags: -Wc,-pic0 -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG_INPLACE=occ:20 IDO52_UGEN_TEMP_REG=20:v0 IDO52_FORCE_WEB_SPILL=31 IDO52_NO_CSE_SITE=mpy:2-9 IDO52_CONST_NOINREG_VALUES=96 IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_NO_BRANCH_FOLD=all IDO52_UGEN_DEST_TEMP=16 IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_NO_FINDINDUCT_STRIDE=1 IDO52_AS1_DIV_POW2=aspsx IDO52_FORCE_WEB_COLOR=134:a1 IDO52_CONST_DEF_EDGE=134:25:j,134:26:j IDO52_CONST_DEF_EDGE_FORCE=1 IDO52_CONST_DEF_BB=134:39:-,134:32:- IDO52_AS1_TIE_TAKE=220,258 IDO52_AS1_TIE_REJECT=221 IDO52_AS1_KEEP_IDENTICAL_REWRITE=all */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

typedef struct {
    void (*fn)();
    u32 *param;
    u32 arg;
    u32 data[21];
} GpuQEnt;

extern volatile u32 *GPU_status;
extern volatile u32 *libgpu_dma2_chcr_ptr;
extern volatile s32 _qin;
extern volatile s32 _qout;
extern s32 libgpu_queue_full_count;
typedef struct {
    u8 pad0;
    u8 b61;
    u8 pad2[6];
    s32 w68;
    s32 w6C;
} Gpu60;
extern Gpu60 graph_env;
extern s32 libgpu_addque_saved_mask;
extern volatile GpuQEnt _que[];

extern void set_alarm();
extern s32 get_alarm();
extern s32 _exeque();
extern s32 SetIntrMask(s32);
extern void _sync(s32);
extern void DMACallback(s32, s32 (*)());

s32 _addque2(void (*func)(), u32 *param, s32 len, volatile u32 arg)
{
    s32 i;
    s32 rem;
    volatile u32 *stat;

    set_alarm();
    while (_qout == ((_qin + 1) & 0x3F)) {
        libgpu_queue_full_count++;
        if (get_alarm()) {
            return -1;
        }
        _exeque();
    }
    libgpu_addque_saved_mask = SetIntrMask(0);
    graph_env.w68 = 1;
    if (graph_env.b61 == 0) {
        _sync(0);
        func(param, arg);
        goto end;
    }
    if (_qin == _qout && !(*libgpu_dma2_chcr_ptr & 0x1000000) && graph_env.w6C == 0) {
        stat = GPU_status;
        while (!(*stat & 0x10000000)) {
        }
        func(param, arg);
        goto end;
    }
    DMACallback(2, _exeque);
    if (len != 0) {
        i = 0;
        if (len / 4 > 0) {
            rem = (len / 4) & 3;
            if (rem != 0) {
                do {
                    _que[_qin].data[i] = *(u32 *)((u8 *)param + i * 2 * 2);
                    goto L1;
                L1:
                    i++;
                } while (rem != i);
                if (i == len / 4) {
                    goto post;
                }
            }
            do {
                _que[_qin].data[i] = *(u32 *)((u8 *)param + i * 2 * 2);
                goto L2;
            L2:
                _que[_qin].data[i + 1] = *(u32 *)((u8 *)param + (i + 1) * 2 * 2);
                goto L3;
            L3:
                _que[_qin].data[i + 2] = *(u32 *)((u8 *)param + (i + 2) * 2 * 2);
                goto L4;
            L4:
                _que[_qin].data[i + 3] = *(u32 *)((u8 *)param + (i + 3) * 2 * 2);
                goto L5;
            L5:
                i += 4;
            } while (i != len / 4);
        }
    post:
        _que[_qin].param = (u32 *)((u32)_que[0].data + _qin * 96);
    } else {
        _que[_qin].param = param;
    }
    _que[_qin].arg = arg;
    _que[_qin].fn = func;
    _qin = (_qin + 1) & 0x3F;
    SetIntrMask(libgpu_addque_saved_mask);
    _exeque();
    return (_qin - _qout) & 0x3F;
end:
    SetIntrMask(libgpu_addque_saved_mask);
    return 0;
}
