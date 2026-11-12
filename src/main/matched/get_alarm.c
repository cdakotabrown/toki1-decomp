/* Function: get_alarm */
/* Address: 0x800A28B0 */
/* Size: 196 bytes (49 instructions) */
/* Region: Library (0x80086000-0x800BFFFF) */
/* Compiler: IDO 5.2 (t6..t9 temporaries, `b` not `j`) */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=all IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_UGEN_TEMP_REG=2:v0 IDO52_UGEN_TEMP_REG_INPLACE=2 IDO52_AS1_NO_BRANCH_FOLD=1 IDO52_AS1_TIE_TAKE=33 IDO52_AS1_TIE_REJECT=22,27,38,47,58,59 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x74 */
/* Also refuted 2026-09-22: `@Compiler: ido52-gas` (55 words, 17/49) and */
#include "common.h"

extern s32 libgpu_alarm_deadline_vcount;
extern s32 libgpu_alarm_spin_count;

extern s32 * volatile GPU_status;
extern s32 _qin;
extern s32 * volatile libgpu_dma2_chcr_ptr;
extern s32 _qout;
extern s32 *libgpu_dma2_madr_ptr;
extern s32 libgpu_timeout_fmt[1];

extern s32 VSync(s32 arg0);
extern void printf(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void _reset(s32 arg0);

s32 get_alarm(void)
{
    s32 t;
    s32 count;
    s32 flag;
    volatile s32 *ptr;

    t = VSync(-1);

    if (libgpu_alarm_deadline_vcount < t) {
        goto do_work;
    }

    count = libgpu_alarm_spin_count;
    flag = (count < 0x780001) ^ 1;
    libgpu_alarm_spin_count = count + 1;
    if (!flag) {
        goto return_0;
    }

do_work:
    ptr = GPU_status;
    *ptr;

    printf(libgpu_timeout_fmt, (_qin - _qout) & 0x3F, *ptr,
                  *libgpu_dma2_chcr_ptr, *libgpu_dma2_madr_ptr);
    _reset(1);
    return -1;

return_0:
    return 0;
}
