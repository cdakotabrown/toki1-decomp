/* Likely Symbol: card_file_save (from o.bin, offset +256 bytes from 0x80054F3C) */
/* Address: 0x8005503C - 0x80055064 (40 bytes) */
/* Region: GCC (OR encoding) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void bzero(void* ptr, s32 size);

extern u8 album_chr[];

void card_w_clear(void) {
    bzero(album_chr, 0x2000);
}
