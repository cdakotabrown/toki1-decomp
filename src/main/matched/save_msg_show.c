/* Address: 0x8005D3B4 */
/* Size: 60 bytes */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 save_msg_show_str0;
extern void set_kanji_string(s32 a0, s32 a1, s32 a2, void *a3, s32 stack0);
extern void k_sub_disp_start(s32 a0);

void save_msg_show(void) {
    set_kanji_string(-128, 50, 0, &save_msg_show_str0, 0);
    k_sub_disp_start(1);
}
