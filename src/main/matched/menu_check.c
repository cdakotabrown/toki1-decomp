/* Function: menu_check */
/* Address: 0x8004FDA4 */
/* Size: 260 bytes (65 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* (2026-08-29: a dead `@PostProc: strip_instructions` was removed here. */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s8 menu_check_1(s32 arg0, s16 arg1, s16 arg2);
extern s8 menu_check_2(s32 arg0, s16 arg1, s16 arg2);

extern u8 menu_mode;
extern s8 now_icon[];
extern u32 pad_trig;
extern u32 pad_data;

s8 menu_check(s32 arg0, s16 arg1, s16 arg2) {
    u8 mode;

    if (arg0 >= 2) {
        return -1;
    }

    mode = menu_mode;

    if ((mode & 3) == 0) {
        return menu_check_1(arg0, arg1, arg2);
    }

    if ((mode & 3) == 1) {
        menu_check_2(arg0, arg1, arg2);
        return now_icon[arg0];
    }

    if ((mode & 3) == 3) {
        menu_check_2(arg0, arg1, arg2);
        return now_icon[arg0];
    }

    if (pad_trig & 4) {
        return menu_check_1(arg0, arg1, arg2);
    }

    if (pad_data & 4) {
        return menu_check_2(arg0, arg1, arg2);
    }

    return menu_check_1(arg0, arg1, arg2);
}
