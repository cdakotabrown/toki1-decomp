/* @CompileFlags: -Wc,-pic0 */
#include "types.h"

extern void menu_set();

void option_menu_set(s32 a0) {
    s16 padTop[2];
    s16 arr4[16];
    s16 arr3[16];
    s16 arr2[16];
    s16 arr1[16];
    s16 padBot[6];
    s32 i;

    (void)&a0;
    (void)&padTop;
    (void)&padBot;
    for (i = 0; i < 16; i++) {
        arr4[i] = 0;
        arr3[i] = (s16)(i * 10 - 100);
        arr2[i] = 64;
        arr1[i] = 10;
        menu_set(1, 16, arr4, arr3, arr2, arr1);
    }
}
