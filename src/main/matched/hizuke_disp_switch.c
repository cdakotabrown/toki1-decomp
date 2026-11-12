/* Address: 0x80064ACC */
/* Size: 168 bytes (42 instructions) */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_AS1_TIE_TAKE=0 IDO52_AS1_TIE_REJECT=31,32,33,34,35,36,69,70,71,72,73,74 IDO52_AS1_XBB_SKIP_CAND=0 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 class_w[];
extern u8 cw8_rec[];

void hizuke_disp_switch(s32 a0) {
    u8 *v1;
    u8 *v0end;

    if (a0 == 1) {
        v0end = cw8_rec;
        v1 = class_w;
        do {
            v1[1091] |= 0x80;
            v1[1159] |= 0x80;
            v1[1227] |= 0x80;
            v1[1295] |= 0x80;
            v1 += 272;
        } while (v1 != v0end);
    } else {
        v0end = cw8_rec;
        v1 = class_w;
        do {
            v1[1091] &= 0x7F;
            v1[1159] &= 0x7F;
            v1[1227] &= 0x7F;
            v1[1295] &= 0x7F;
            v1 += 272;
        } while (v1 != v0end);
    }
}
