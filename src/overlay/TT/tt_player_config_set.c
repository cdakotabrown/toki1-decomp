/* tt_player_config_set - TT overlay, Address: 0x80143570, true body 316 bytes */
/* @SymbolSize: 316 */
/* @PostProc: scheduler_normalizer */
#include "types.h"

extern s32 trail_init(void *a0);

extern u8 *tt_step_wk;
extern u8 *tt_player_wk;
extern u8 *tt_trail_buf;
extern u8 *tt_game_wk;

void tt_player_config_set(void)
{
    u8 *t6;
    u8 *v0;
    u8 *s1;
    u32 v1;
    u32 a1;
    u32 one;
    u8 t9;
    u8 t0;
    s32 i;

    t6 = tt_step_wk;
    v0 = tt_player_wk;
    s1 = tt_trail_buf;
    v1 = 8;
    if (t6[34] != 0) {
        one = 1;
        v1 = tt_game_wk[82];
        switch (v1) {
    case 1:
        a1 = 4;
        v1 = 8;
        v0[83] = 0;
        v0[84] = (u8)a1;
        v0[85] = (u8)v1;
        s1[146] = (u8)v1;
        break;
    case 2:
        a1 = 4;
        v1 = 9;
        v0[83] = 0;
        v0[84] = (u8)a1;
        v0[85] = (u8)v1;
        s1[146] = (u8)v1;
        break;
    case 3:
        a1 = 4;
        v1 = 9;
        v0[83] = 0;
        v0[84] = (u8)a1;
        v0[85] = (u8)v1;
        s1[146] = (u8)v1;
        break;
    case 4:
        t9 = 5;
        t0 = 10;
        v1 = 9;
        v0[83] = (u8)one;
        v0[84] = t9;
        v0[85] = t0;
        s1[146] = (u8)v1;
        i = 0;
        do {
            trail_init(s1);
            i++;
            s1 += 164;
        } while (i != 3);
        break;
        default:
            a1 = 4;
            v1 = 8;
            v0[83] = 0;
            v0[84] = (u8)a1;
            v0[85] = (u8)v1;
            s1[146] = (u8)v1;
            break;
        }
        return;
    }

    a1 = 4;
    v0[83] = 0;
    v0[84] = (u8)a1;
    v0[85] = (u8)v1;
    s1[146] = (u8)v1;
}
