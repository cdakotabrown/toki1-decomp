/* Address: 0x80139AF4 */
/* Size: 124 bytes (31 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[39];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable geko_sub_sub_dispatch39_tbl;

extern void geko_particle_move(void);

void geko_sub_sub_dispatch39(void)
{
    u8 idx;
    JumpTable table;

    table = geko_sub_sub_dispatch39_tbl;

    idx = now_sub_sub_step;
    table.ptrs[idx](128);

    geko_particle_move();
}
