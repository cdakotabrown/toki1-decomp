/* Likely Symbol: gnsx (from o.bin, offset +40 bytes from 0x800511F4) */
/* Address: 0x8005121C */
/* Debug Symbol: gnsx (from o.bin, high match) */
/* Size: 144 bytes (36 instructions) */
/* Region: GCC (< 0x80086000, OR encoding) */
/* Compiler: IDO 5.2 via ido-static-recomp */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 strlen(void);
extern void set_kanji_string(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);

void gnsx(s32 arg0) {
    s32 result;

    result = strlen();

    switch (result) {
    case 2:
        set_kanji_string(-114, 50, 0, arg0, 0);
        break;
    case 4:
        set_kanji_string(-121, 50, 0, arg0, 0);
        break;
    case 6:
    default:
        set_kanji_string(-128, 50, 0, arg0, 0);
        break;
    }
}
