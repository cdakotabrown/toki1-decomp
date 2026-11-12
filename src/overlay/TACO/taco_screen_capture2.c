/* @SymbolStart: 0x801435B4 */
/* @SymbolSize: 92 */
/* Address: 0x801435BC */
#include "types.h"

extern s32 activeBuff;

extern void DrawSync(s32);
extern void MoveImage(void *, s32, s32);

typedef struct {
    s16 a;
    s16 b;
    s16 c;
    s16 d;
} Quad16;

void taco_screen_capture2(void)
{
    s32 idx = activeBuff;

    Quad16 q;

    q.a = 0;
    q.b = (s16)(idx * 240);
    q.c = 512;
    q.d = 240;
    MoveImage(&q, 512, 0);
    DrawSync(0);
}
