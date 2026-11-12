/* Function: load_tpage_buf_lock */
/* Address: 0x80048854 */
/* Size: 168 bytes (42 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_insn_reg_field --patch 0x7C:rt:t8:t6 --patch 0x80:rt:t8:t6 */
/* @PostProc: strip_trailing_instructions --count 2 --nop-only */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 tpage_buf[];
extern void image_upload_queue_add_flags(s32, s32, u8, s32, s32, s32, s32);

void load_tpage_buf_lock(s32 arg0, s32 arg1, s32 arg2) {
    tpage_buf[arg0] = arg1;

    (void)&arg0;
    (void)&arg1;
    (void)&arg2;

    if (arg0 < 32) {
        image_upload_queue_add_flags(arg1, (u8)(arg0 & 0x1F), *(u8 *)&arg2, 0, 0, 128, 128);
    } else {
        image_upload_queue_add_flags(arg1, (u8)(arg0 & 0x1F), *(u8 *)&arg2, 0, 128, 128, 128);
    }
}
