/* Address: 0x8006B670 (true start 0x8006B668: the ent[0] read is hoisted */
/* Size: 132 bytes (33 instructions) over the corrected window */
/* Region: Game (IDO 5.2) */
/* @SymbolStart: 0x8006B668 */
/* @SymbolSize: 132 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u32 id : 5;
    u32 pad : 20;
    u32 grp : 4;
    u32 on : 1;
    u32 rest : 2;
} Ent;

typedef struct {
    u8 pad0[1356];
    Ent ent[8];
} Save;

extern Save screen_w;

void reserved_date_add(e)
    Ent e;
{
    s32 i;

    i = 0;
    while (screen_w.ent[i].on && i != 8) {
        i++;
    }
    screen_w.ent[i] = e;
    reserved_date_sort();
}
