/* Address: 0x8004EF24 */
/* Size: 240 bytes (60 instructions) */
/* Region: Game */
/* @SymbolSize: 240 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8 pad[5];
    u8 delta;
    u8 pad2[2];
} AnimEntry;

extern AnimEntry disp_string_tbl0[];
extern s16 check_end_k_tbl1;

s32 k_disp_goto_line_end(void) {
    s32 acc;
    s32 i;

    acc = 0;
    for (i = 0; i < 52; i++) {
        acc += disp_string_tbl0[i].delta;
        if (acc >= check_end_k_tbl1) {
            check_end_k_tbl1 = acc;
            return (s16)acc;
        }
    }
    return -1;
}
