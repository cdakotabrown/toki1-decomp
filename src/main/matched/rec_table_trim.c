/* Address: 0x8006CA30 (NEW symbol: it lies inside the recorded 356-byte */
/* Size: 272 bytes (68 instructions) */
/* Region: Game (IDO 5.2 -O2, -Wc,-pic1, leaf) */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=9:t0,49:t0 */
/* @SymbolStart: 0x8006CA30 */
/* @SymbolSize: 272 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8 kind;
    u8 b1;
    u8 flag;
    u8 b3;
    u8 b4;
    u8 b5;
    u8 val;
    u8 b7;
} Rec8;

extern u8 screen_w[];

s32 rec_table_trim(u8 *outLast, u8 *outPrev, s32 unused)
{
    Rec8 *r;
    s32 i;
    s32 n;
    u32 last;
    u32 prev;
    u32 kind;
    u32 x;

    last = 255;
    prev = 0;
    n = 0;
    r = (Rec8 *)screen_w;
    i = 0;
    do {
        i++;
        if (r[236].flag == 0) {
            break;
        }
        kind = r[236].kind;
        n++;
        if (kind != 0 && kind != 1 && kind != 2 && kind != 15) {
            x = r[235].val;
        } else {
            x = r[235].val;
            last = x;
        }
        r++;
        prev = x;
    } while (i < 256);
    for (i = n; i < 256; i++) {
        ((Rec8 *)screen_w)[i + 236].flag = 0;
    }
    *outLast = last;
    *outPrev = prev;
    return n;
}
