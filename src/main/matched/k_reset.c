/* Address: 0x8004EE1C */
/* Size: 164 bytes (41 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8 pad0[5];
    u8 f5;
    u8 pad6[2];
} Rec8;

extern Rec8 disp_string_tbl0[];
struct {
    s16 f60;
    s16 f62;
    s16 f64;
    s16 f66;
    s16 f68;
} disp_string_state = { 0, 0, 0, 0, 0 };

void k_reset(s32 n) {
    s16 sum = 0;
    s32 i;

    for (i = 0; i < n; i++) {
        sum += disp_string_tbl0[i].f5;
    }
    disp_string_state.f60 = n;
    disp_string_state.f62 = sum;
    disp_string_state.f68 = sum;
}
