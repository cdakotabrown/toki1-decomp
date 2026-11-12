/* @SymbolSize: 60 */
/* Address: 0x801486C8, Size: 60 bytes (15 insns) */
#include "types.h"

extern s32 memcpy();
extern s32 *tt_str_ptr_tbl[];

void tt_name_set(s32 a0, s32 idx)
{
    memcpy(tt_str_ptr_tbl[idx], a0, 3);
}
