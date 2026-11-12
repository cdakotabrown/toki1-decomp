/* Likely Symbol: card_file_save (from o.bin, offset +92 bytes from 0x80054F3C) */
/* Address: 0x80054F98 */
/* Size: 60 bytes */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 card_file_check(s32 arg0);
extern u8 card_sys_present;

void card_sys_present_set(void) {
    if (card_file_check(0) != 0) {
        card_sys_present = 1;
    } else {
        card_sys_present = 0;
    }
}
