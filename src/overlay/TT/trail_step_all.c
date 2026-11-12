/* Address: 0x801480AC, Size: 152 bytes (38 insns) */
#include "types.h"

extern u8 *tt_player_wk;
extern u8 *tt_trail_buf;
extern u8 *tt_cel_buf;

extern void trail_hist_push(u8 *ptr);

void trail_step_all(void)
{
    u8 *s3;
    u8 *s0;
    u8 *s1;
    s32 s2;
    s32 s4;

    s3 = tt_player_wk;
    if (s3[83] == 0) {
        return;
    }

    s1 = tt_trail_buf;
    s2 = 0;
    s0 = tt_cel_buf + 192;
    s4 = 3;

    do {
        trail_hist_push(s1);
        s0[62] = s3[90] + 56;
        s1 += 164;
        s0 += 192;
        s0[-34] = s3[91] + 61;
        s2++;
    } while (s2 != s4);
}
