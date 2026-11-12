/* Function: girl_menu_set */
/* Address: 0x80084B14 */
/* Size: 656 bytes (164 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: remap_stack_slots --map '84:100,92:108' */
/* Region: Game (0x80041000-0x80085FFF) */
#include "types.h"

extern void strcpy(void *dst, s32 src);
extern s32 strlen(void *str);
extern s32 set_kanji_string(s32 x, s32 y, s32 arg2, void *arg3, s32 arg4);
extern void k_disp_start(s32 arg0);
extern void menu_set(s32 a0, s32 a1, void *a2, void *a3, void *a4, void *a5);

extern u8 girl_appear[];
extern s16 girl_menu_set_tbl0[];
extern s16 girl_menu_set_tbl1[];
extern s16 girl_menu_select_tbl0[];
extern s16 girl_menu_set_tbl2[];
extern s32 date_name_set_tbl0[];
extern u8 girl_menu_set_str0[];
extern s8 menu_csr;
extern s32 menu_on;
extern s32 step_counter;

void girl_menu_set(volatile u8 arg0)
{
    s32 unused[2];
    s32 result;
    s16 i;
    s32 sp64[5];

    for (i = 0; i < 11; i++) {
        girl_menu_set_tbl0[i] = ((i / 3) << 6) - 128;
        girl_menu_set_tbl1[i] = ((i % 3) << 4) + 48;

        if (girl_appear[i] != 0) {
            strcpy(sp64, date_name_set_tbl0[i]);
            sp64[2] = set_kanji_string(girl_menu_set_tbl0[i], girl_menu_set_tbl1[i], 0, sp64, 0);
            girl_menu_select_tbl0[i] = (strlen(sp64) / 2) * 14;
        } else {
            girl_menu_select_tbl0[i] = 0;
        }
        girl_menu_set_tbl2[i] = 16;
    }

    girl_menu_set_tbl0[11] = 64;
    girl_menu_set_tbl1[11] = 80;
    girl_menu_select_tbl0[11] = 42;
    girl_menu_set_tbl2[11] = 16;

    if (arg0 != 0) {
        girl_menu_select_tbl0[11] = 0;
    }

    if (arg0 == 0) {
        sp64[2] = set_kanji_string(64, 80, 0, girl_menu_set_str0, 0);
    }

    k_disp_start(sp64[2]);
    menu_set(1, (12 - arg0) & 0xFF, girl_menu_set_tbl0, girl_menu_set_tbl1, girl_menu_select_tbl0, girl_menu_set_tbl2);

    result = 0;
    i = 0;
    while (i < 11) {
        if (girl_menu_select_tbl0[i] != 0) {
            menu_csr = (s8)i;
            break;
        }
        i++;
    }

    menu_on = 1;
    step_counter++;
}
