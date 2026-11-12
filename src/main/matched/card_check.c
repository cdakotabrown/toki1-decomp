/* Likely Symbol: card_file_save (from o.bin, offset +152 bytes from 0x80054F3C) */
/* Address: 0x80054FD4 - 0x8005503C (104 bytes) */
/* Region: GCC (< 0x80081000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"
extern u8 album_work[];

extern u8 card_file_check(s32 arg0);

s32 card_check(void) {
    u8 *ptr;
    s32 count;
    s32 i;

    count = 0;
    ptr = &(&album_work[1])[0];
    i = 1;

    do {
        u8 result = card_file_check(i);
        i++;
        ptr++;
        ptr[3] = result;
        count++;
    } while (i != 15);

    return count;
}
