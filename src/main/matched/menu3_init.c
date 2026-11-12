/* Address: 0x800831D8 */
/* Size: 168 bytes (42 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 72 --new 88 */
/* @PostProc: remap_stack_slots --map '40:52,42:54,44:56,48:60,50:62,52:64,56:68,58:70,60:72,64:76,66:78,68:80' */
#include "types.h"

extern void menu_set(s32, s32, s16*, s16*, s16*, s16*);

extern u8 menu_csr;
extern s32 menu_on;
extern s32 step_counter;

s32 menu3_init(void) {
    s16 arr3[4];
    s16 arr2[4];
    s16 arr1[4];
    s16 arr0[4];

    arr3[0] = arr3[1] = arr3[2] = -128;
    arr2[0] = 48;
    arr2[1] = 64;
    arr2[2] = 80;
    arr1[0] = arr1[1] = arr1[2] = 252;
    arr0[0] = arr0[1] = arr0[2] = 16;

    menu_set(1, 3, arr3, arr2, arr1, arr0);

    menu_csr = 0;
    menu_on = 1;
    step_counter++;
}
