/* @SymbolSize: 156 */
/* Address: 0x80133928 */
/* Size: 156 bytes (39 insns) */
#include "types.h"

extern s32 taiiku_pad_accel(s32 a0);

typedef struct {
    s8  pad[38];
    s16 x;
    s16 unk0;
    s16 y;
    s8  tail[24];

} S801271B0;

extern S801271B0 class_w[];

void taiiku_pad_move(s32 a0) {
    S801271B0 *p;

    taiiku_pad_accel(a0);
    p = &class_w[a0];
    if (p->x >= 144) p->x = 144;
    if (p->x < -159) p->x = -160;
    if (p->y >= 104) p->y = 104;
    if (p->y < -103) p->y = -104;
}
