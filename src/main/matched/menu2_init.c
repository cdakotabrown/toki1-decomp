/* Function: menu2_init */
/* Address: 0x800833DC */
/* Size: 148 bytes (37 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void menu_set(s32, s32, s16*, s16*, s16*, s16*);

extern u8 menu_csr;
extern s32 menu_on;
extern s32 step_counter;

s32 menu2_init(void) {
    s32 unused;
    s16 arr3[2];
    s16 arr2[2];
    s16 arr1[2];
    s16 arr0[2];

    arr3[0] = arr3[1] = -128;
    arr2[0] = 48;
    arr2[1] = 64;
    arr1[0] = arr1[1] = 252;
    arr0[0] = arr0[1] = 16;

    menu_set(1, 2, arr3, arr2, arr1, arr0);

    menu_csr = 0;
    menu_on = 1;
    step_counter++;
}
