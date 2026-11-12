/* Address: 0x80043280 */
/* Size: 176 bytes (44 instructions) */
/* Region: Game */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=6:a1,12:a0,18:a2 IDO52_AS1_TIE_TAKE=4,10 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 pad_type;
extern volatile u8 pad1_btn_lo;
extern volatile u8 pad1_btn_hi;
extern volatile u8 pad2_btn_lo;
extern volatile u8 pad2_btn_hi;

u32 pad_get(void)
{
    int a, b, c, v, w;

    if (pad_type != 0xFF) {
        if (pad_type != 0) {
            b = pad2_btn_hi;
            a = pad1_btn_lo;
            c = pad2_btn_lo;
            v = pad1_btn_hi;
            b = ~b;
            a = ~a;
            v = ~v;
            return ((~c) << 24) + ((u8)b << 16) + ((u8)a << 8) + (u8)v;
        }
        w = pad1_btn_hi;
        a = 0;
        w = ~w;
        w &= 0xff;
        if (w & 8) a = 32;
        if (w & 4) a |= 0x40;
        return a;
    }
}
