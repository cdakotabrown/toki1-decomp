/* Function: chr_flag_clear (chr_flag_clear) */
/* Address: 0x80071340 */
/* Size: 172 bytes (43 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Likely Symbol: chr_flag_clear */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: scheduler_normalizer */
#include "types.h"

extern u8 now_month;
extern u8 now_day;
extern u8 girl0_flag;
extern u8 girl1_flag;
extern u8 girl2_flag;
extern u8 girl_blk;
extern u8 girl_blk_end;

void chr_flag_clear(void) {
    u8 *ptr;
    u8 *end;

    if (now_month != 5) {
        return;
    }
    if (now_day != 6) {
        return;
    }

    girl0_flag = girl0_flag & 0xFFEF;
    girl1_flag = girl1_flag & 0xFFEF;
    girl2_flag = girl2_flag & 0xFFEF;

    end = &girl_blk_end;
    ptr = &girl_blk;
    do {
        *(ptr + 456) = *(ptr + 456) & 0xFFEF;
        *(ptr + 512) = *(ptr + 512) & 0xFFEF;
        *(ptr + 568) = *(ptr + 568) & 0xFFEF;
        *(ptr + 624) = *(ptr + 624) & 0xFFEF;
        ptr += 224;
    } while (ptr != end);
}
