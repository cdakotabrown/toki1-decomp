/* Address: 0x80097580 */
/* Size: 200 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 24 --new 32 --shift-ra */
/* Compiler: IDO 5.2 */
/* Region: IDO (0x80080000-0x800BFFFF) */
#include "types.h"

extern void ResetCallback(void);
extern void _spu_init(u32 a0);
extern void SpuStart(void);
extern void _spu_ioctl(u32 a0, s32 a1, void *a2);

extern u32 _spu_rev_flag;
extern u32 _spu_rev_reserve_wa;
extern u32 libspu_rev_mode;
extern s16 libspu_rev_depth_left;
extern s16 libspu_rev_depth_right;
extern u32 libspu_rev_delay;
extern u32 libspu_rev_feedback;
extern u32 _spu_rev_startaddr;
extern u32 _spu_rev_offsetaddr;
extern u32 _spu_fd;
extern u32 _spu_trans_mode;
extern u32 _spu_keystat;
extern u32 _spu_keyon_bit;
extern u32 _spu_keyoff_bit;

void _SpuInit(u32 param)
{
    ResetCallback();
    _spu_init(param);
    SpuStart();

    _spu_rev_flag = 0;
    _spu_rev_reserve_wa = 0;
    libspu_rev_mode = 0;
    libspu_rev_depth_left = 0;
    libspu_rev_depth_right = 0;
    libspu_rev_delay = 0;
    libspu_rev_feedback = 0;

    _spu_rev_offsetaddr = _spu_rev_startaddr;

    _spu_ioctl(_spu_fd, 97, &_spu_rev_offsetaddr);

    _spu_trans_mode = 0;

    _spu_ioctl(_spu_fd, 33, &_spu_trans_mode);

    _spu_keystat = 0;
    _spu_keyon_bit = 0;
    _spu_keyoff_bit = 0;
}
