/* @PostProc: patch_stack_complete --old 128 --new 152 */
/* @PostProc: remap_stack_slots --from 0 --to 20 */
/* @SymbolStart: 0x8013C51C */
/* @SymbolSize: 80 */
/* geko_sub_sub_dispatch32 -- GEKO overlay, Address: 0x8013C520, Size: 76 bytes (19 insns) */
#include "types.h"

typedef struct {
    s32 w[32];
} Block128;

extern Block128 geko_sub_sub_dispatch32_tbl;

void geko_sub_sub_dispatch32(void)
{
    Block128 local;

    local = geko_sub_sub_dispatch32_tbl;
}
