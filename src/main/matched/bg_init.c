/* Address: 0x8004180C */
/* Size: 500 bytes (125 instructions) */
/* Region: Game (IDO 5.2) */
/* `jr ra ; nop` at 0x800419FC.  @SymbolSize: 500.  The registry entry */
/* @SymbolSize: 500 */
/* @CompileFlags: -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u32 tag;
    s16 a[6];
    u8 b[3];
    u8 pad13;
    u32 c;
    s16 d[4];
    u32 e;
} PrimRec;

typedef struct {
    u8 pad0[4332];
    u8 flag[1];
} Save;

extern PrimRec Bg[];
extern Save screen_w;

void bg_init(start, end)
    s32 start;
    s32 end;
{
    s32 i;
    Save *sv;

    sv = &screen_w;
    for (i = start; i < end; i++) {
        Bg[i].tag = 0x80000000;
        Bg[i].a[0] = 0;
        Bg[i].a[1] = 0;
        Bg[i].a[2] = 0;
        Bg[i].a[3] = 0;
        Bg[i].a[4] = 0;
        Bg[i].a[5] = 0;
        Bg[i].b[0] = 0;
        Bg[i].b[1] = 0;
        Bg[i].b[2] = 0;
        Bg[i].c = 0;
        Bg[i].d[0] = 0;
        Bg[i].d[1] = 0;
        Bg[i].d[2] = 0;
        Bg[i].d[3] = 0;
        Bg[i].e = 0;
        sv->flag[i] = 0x80;
    }
}
