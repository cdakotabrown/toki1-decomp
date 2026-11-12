/* Address: 0x80142350 */
/* Size: 180 bytes (45 insns) */
/* @SymbolSize: 180 */
#include "types.h"

extern s32 taiiku_girl1_dobj[];

void taiiku_tex_ptrs_set(void) {
    taiiku_girl1_dobj[0] = 0x801B93F0;
    taiiku_girl1_dobj[1] = 0x801B9360;
    taiiku_girl1_dobj[2] = 0x801B93D8;
    (*(s16 *)&taiiku_girl1_dobj[3]) = *(s16 *)0x801B93F8;
    taiiku_girl1_dobj[4] = 0x80197000;
    taiiku_girl1_dobj[5] = 0x80199000;
    taiiku_girl1_dobj[6] = 0x8019D000;
    taiiku_girl1_dobj[7] = 0x801A1000;
    taiiku_girl1_dobj[8] = 0x801A5000;
    taiiku_girl1_dobj[9] = 0x801A9000;
    taiiku_girl1_dobj[10] = 0x801AD000;
}
