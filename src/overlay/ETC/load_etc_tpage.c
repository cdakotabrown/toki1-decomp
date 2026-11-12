/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x80143664 */
/* Size: 116 bytes (29 instructions) */
#include "types.h"

extern void load_tpage_buf_lock(s32 a0, s32 a1, s32 a2);

void load_etc_tpage(s32 a0)
{
    s32 v0;

    v0 = a0 << 15;
    load_tpage_buf_lock(5, 0x801E8000 - v0, 2);
    load_tpage_buf_lock(6, 0x801EA000 - v0, 2);
    load_tpage_buf_lock(7, 0x801EC000 - v0, 2);
}
