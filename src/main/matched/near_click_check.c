/* Function: near_click_check */
/* Address: 0x8005B440 */
/* Size: 268 bytes (67 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* Residual FRAME (+8) carried by @PostProc: patch_stack_complete.  Retail */
/* @SymbolStart: 0x8005B438 */
/* @SymbolSize: 276 */
/* @CompileEnv: IDO52_UGEN_NO_ARG_COALESCE=1 IDO52_UGEN_TEMP_REG=12:t6,13:t8,14:t0,15:t1,16:t2 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_stack_complete --old 48 --new 56 */
#include "types.h"

typedef struct {
    s32 x;
    s32 y;
} D_800BA088_Entry;

extern u8 mascot_cw_disp;
extern s16 mascot_cw_x;
extern s16 cursor_cw_x;
extern s16 mascot_cw_y;
extern s16 cursor_cw_y;
extern s32 pad_trig;
extern s32 frame_counter;
extern s16 near_click_check_tbl1;
extern u8 mascot_cw_frame;
extern u8 now_girl;
extern u8 near_click_check_tbl0[];

extern s32 get_g_zyotai_s(s32);
extern void set_XAsector(s32, s32);
extern void device_cmd_send(s32);

s32 near_click_check(void)
{
    volatile s32 a2;
    u8 tmp;
    s32 arg;
    s32 ret;
    s32 sel;
    s32 ix;
    D_800BA088_Entry *entry;

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

    if ((u32)(frame_counter - near_click_check_tbl1) < 241) {
        goto fail;
    }

    tmp = mascot_cw_frame;
    now_girl = tmp;
    *(s32 *)&a2 = tmp;
    arg = tmp;
    ret = get_g_zyotai_s(arg);
    ix = a2;
    if (ret & 0x80) {
        sel = 1;
    } else {
        sel = 0;
    }
    entry = (D_800BA088_Entry *)(near_click_check_tbl0 + (ix * 16 + sel * 8));
    set_XAsector(entry->x, entry->y);
    device_cmd_send(768);
    return 1;

fail:
    return 0;
}
