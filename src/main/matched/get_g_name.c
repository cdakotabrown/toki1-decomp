/* Address: 0x80051858 */
/* Size: 208 bytes (52 instructions) */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
/* Stamped @CompileFlags: -Wc,-pic1: retail's frame is 48 bytes and the arg home */
#include "types.h"

extern void strcpy(s32 a0, s32 a1);
extern void strcat(s32 a0, s32 a1);
extern s32 get_g_name_tbl1;
extern s32 get_g_name_tbl0;
extern s32 date_name_set_tbl0[];
extern char get_g_name_str0[];
extern char get_g_name_str1[];
extern char get_g_name_str2[];

s32 get_g_name(s32 a0, s32 a1) {
    s32 s0;
    s32 idx;

    s0 = a1;

    if (s0 == 0) {
        strcpy(a0, (s32)get_g_name_str0);
        goto check_range;
    }

    if (s0 == 10) {
        strcpy(a0, get_g_name_tbl1);
        strcat(a0, (s32)get_g_name_str1);
        goto check_range;
    }

    if (s0 == 12) {
        idx = s0 & 0xF;
        strcpy(a0, get_g_name_tbl0);
        goto check_range;
    }

    idx = s0 & 0xF;
    strcpy(a0, date_name_set_tbl0[idx]);
    strcat(a0, (s32)get_g_name_str2);

check_range:
    if (s0 >= 16 || s0 < 0) {
        return -1;
    }
    return 1;
}
