/* @SymbolSize: 92 */
/* ending_kanji_clear - ENDING overlay, Address: 0x8013B670, Size: 92 bytes (23 insns) */
#include "types.h"

extern s32 get_now_k(void);
extern u8 *get_k_work(s32 a0);

void ending_kanji_clear(void)
{
    u32 s1;
    s16 s0;
    u8 *v0;

    v0 = (u8*)get_now_k();
    s1 = (u32)v0 & 0xFF;
    get_k_work((s32)v0);

    if (s1 != 0) {
        s0 = 0;
        do {
            v0 = get_k_work(s0);
            s0 += 1;
            v0[7] = 0;
        } while ((u32)s0 < s1);
    }
}
