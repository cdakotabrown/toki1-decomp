/* Address: 0x8004ECAC */
/* Size: 184 bytes (46 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileFlags: -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8 pad[5];
    u8 delta;
    u8 pad2[2];
} AnimEntry;

extern AnimEntry disp_string_tbl0[];
extern s16 check_end_k_tbl1;

void k_disp_start(n)
    s32 n;
{
    s32 i;

    check_end_k_tbl1 = 0;
    for (i = 0; i < n; i++) {
        check_end_k_tbl1 += disp_string_tbl0[i].delta;
    }
}
