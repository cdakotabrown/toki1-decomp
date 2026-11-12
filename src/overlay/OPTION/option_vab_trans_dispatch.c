/* @SymbolStart: 0x8013A080 */
/* @SymbolSize: 60 */
/* Address: 0x8013A088, Size: 52 bytes (13 instructions) */
#include "types.h"

extern void option_main_vab_trans();
extern void option_sub_vab_trans();
extern u8 option_main_vab_wk;

void option_vab_trans_dispatch(void) {
    s32 hidden_t6;

    hidden_t6 = option_main_vab_wk;
    if (hidden_t6 != 0) {
        option_main_vab_trans();
        return;
    }
    option_sub_vab_trans();
}
