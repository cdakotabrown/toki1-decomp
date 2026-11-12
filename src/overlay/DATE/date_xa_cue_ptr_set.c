#include "types.h"

extern u32 date_xa_cue_tbl;

void date_xa_cue_ptr_set(void)
{
    date_xa_cue_tbl = 0x801E4000;
}
