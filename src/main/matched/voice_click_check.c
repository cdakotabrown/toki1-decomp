/* Function: voice_click_check */
/* Address: 0x8005B554 */
/* Size: 216 bytes (54 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x8005B54C */
/* @SymbolSize: 224 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 mascot_cw_disp;
extern s16 mascot_cw_x;
extern s16 cursor_cw_x;
extern s16 mascot_cw_y;
extern s16 cursor_cw_y;
extern s32 pad_trig;
extern s32 frame_counter;
extern s16 voice_click_check_tbl1;
extern u8 mascot_cw_frame;
extern u8 now_girl;
extern s32 voice_click_check_tbl0[];

extern void set_XAsector(s32, s32, s32);
extern void device_cmd_send(s32);

s32 voice_click_check(void)
{
    s16 ref;
    s16 cur;
    s32 *ptr;
    s32 x;
    s32 y;
    u8 a2;

    if (!(mascot_cw_disp & 0x80)) {
        goto fail;
    }

    if (mascot_cw_x + 12 <= cursor_cw_x) {
        goto fail;
    }
    if (mascot_cw_x - 12 >= cursor_cw_x) {
        goto fail;
    }

    if (mascot_cw_y + 48 <= cursor_cw_y) {
        goto fail;
    }
    if (mascot_cw_y >= cursor_cw_y) {
        goto fail;
    }

    if (!(pad_trig & 0x20)) {
        goto fail;
    }

    if ((u32)(frame_counter - voice_click_check_tbl1) < 241) {
        goto fail;
    }

    a2 = mascot_cw_frame;
    ptr = &voice_click_check_tbl0[a2 * 2];
    x = ptr[0];
    y = ptr[1];
    now_girl = a2;
    set_XAsector(x, y, a2);
    device_cmd_send(0x300);
    return 1;

fail:
    return 0;
}
