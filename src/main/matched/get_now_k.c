/* Address: 0x8004F118 */
/* Size: 288 bytes (72 instructions) */
/* Region: Game */
/* @SymbolSize: 288 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8 pad[5];
    u8 delta;
    u8 pad2[2];
} AnimEntry;

extern AnimEntry disp_string_tbl0[];
extern s16 check_end_k_tbl1;

u8 get_now_k(void) {
    s32 sum;
    s32 i;

    sum = 0;
    for (i = 0; i < check_end_k_tbl1; i++) {
        sum += disp_string_tbl0[i].delta;
        if (sum >= check_end_k_tbl1) {
            return i;
        }
    }
    return 0xFF;
}
