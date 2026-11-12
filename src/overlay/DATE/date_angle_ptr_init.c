/* @SymbolSize: 52 */
/* Address: 0x8014a120 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 date_move_src_tbl[];

void date_angle_ptr_init(void) {
    date_move_src_tbl[0] = 0x80195000;
    date_move_src_tbl[1] = 0x80195060;
    date_move_src_tbl[2] = 0x801950C0;
}
