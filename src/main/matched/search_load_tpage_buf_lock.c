/* Function: search_load_tpage_buf_lock */
/* Address: 0x800488FC */
/* Size: 232 bytes (58 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_trailing_instructions --count 2 --nop-only */
#include "types.h"

extern s32 screen_w[];
extern void image_upload_queue_add_flags(s32, s32, s32, s32, s32, s32, s32);

s32 search_load_tpage_buf_lock(s32 arg0, s32 arg1) {
    u8 masked;
    s32 i;

    for (i = 5; i < 64; i++) {
        if (screen_w[i + 1162] == -1) {
            screen_w[i + 1162] = arg0;
            if (i < 32) {
                masked = (u8)(i & 0x1F);
                image_upload_queue_add_flags(arg0, (s32)masked, (u8)arg1, 0, 0, 128, 128);
                return i;
            } else {
                masked = (u8)(i & 0x1F);
                image_upload_queue_add_flags(arg0, masked, (u8)arg1, 0, 128, 128, 128);
                return i;
            }
        }
    }
    return -1;
}
