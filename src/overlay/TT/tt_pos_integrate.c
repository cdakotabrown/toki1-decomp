/* Size: 172 bytes */
/* @SymbolSize: 172 */
#include "types.h"
extern u8 *tt_player_wk;
void tt_pos_integrate(void)
{
    u8 *p;
    s32 x;
    s32 y;

    p = tt_player_wk;
    *(s32 *)(p + 32) += *(s32 *)(p + 40);
    *(s32 *)(p + 36) += *(s32 *)(p + 44);
    if (p[86] == 0) {
        x = *(s32 *)(p + 32);
        if (x < -0x00780000) {
            *(s32 *)(p + 32) = -0x00780000;
        } else if (x > 0x00780000) {
            *(s32 *)(p + 32) = 0x00780000;
        }
        y = *(s32 *)(p + 36);
        if (y < 0x00380000) {
            *(s32 *)(p + 36) = 0x00380000;
        } else if (y > 0x00E00000) {
            *(s32 *)(p + 36) = 0x00E00000;
        }
    }
    *(s16 *)(p + 20) = *(s16 *)(p + 34);
    *(s16 *)(p + 22) = *(s16 *)(p + 38);
}
