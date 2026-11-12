/* Address: 0x801453D0 */
/* Size: 180 bytes (45 insns) */
/* @SymbolSize: 180 */
#include "types.h"

extern s32 taiiku_girl6_dobj[];

void taiiku_tpage_init(void) {
    taiiku_girl6_dobj[0] = 0x801B523C;
    taiiku_girl6_dobj[1] = 0x801B518C;
    taiiku_girl6_dobj[2] = 0x801B51DC;
    (*(s16 *)&taiiku_girl6_dobj[3]) = *(s16 *)0x801B5240;
    taiiku_girl6_dobj[4] = 0x80197000;
    taiiku_girl6_dobj[5] = 0x80199000;
    taiiku_girl6_dobj[6] = 0x8019D000;
    taiiku_girl6_dobj[7] = 0x801A1000;
    taiiku_girl6_dobj[8] = 0x801A5000;
    taiiku_girl6_dobj[9] = 0x801A9000;
    taiiku_girl6_dobj[10] = 0x801AD000;
}
