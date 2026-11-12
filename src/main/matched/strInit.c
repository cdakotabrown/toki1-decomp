/* Address: 0x80056784 */
/* Size: 136 bytes (34 instructions) */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 *movie_state;
extern u32 movie_main_tbl1;

extern void DecDCTReset(s32 arg0);
extern void DecDCToutCallback(s32 arg0);
extern void StSetRing(u8 *arg0, s32 arg1);
extern void StSetStream(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void strKickCD(s32 arg0);

void strInit(s32 arg0, s32 arg1) {
    DecDCTReset(0);

    *(s32*)(movie_state + 52) = 0;

    DecDCToutCallback(arg1);

    StSetRing(movie_state + 68, 32);

    StSetStream(movie_main_tbl1 & 1, 1, -1, 0, 0);

    strKickCD(arg0);
}
