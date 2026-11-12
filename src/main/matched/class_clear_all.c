/* Address: 0x80049144 */
/* Size: 172 bytes */
/* Region: GCC (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack 56 72 */
/* @PostProc: manual sp offset patches (+16 at offsets 0x28,0x2c,0x34,0x74,0x7c,0x88) */
/* Compiler: IDO 5.2 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s16 cursor_cw_frame;

extern void class_clear_n(s32 arg0);

void class_clear_all(s32 arg0) {
    s32 i;
    s32 limit;
    s32 saved_d6;
    s32 saved_da;
    s32 saved_c8;

    saved_d6 = cursor_cw_x;
    saved_da = cursor_cw_y;
    saved_c8 = cursor_cw_frame;

    if (arg0 == 0) {
        limit = 160;
    } else if (arg0 >= 161) {
        limit = 160;
    } else {
        limit = arg0;
    }

    for (i = 1; i < limit; i++) {
        class_clear_n(i);
    }

    cursor_cw_x = saved_d6;
    cursor_cw_y = saved_da;
    cursor_cw_frame = saved_c8;
}
