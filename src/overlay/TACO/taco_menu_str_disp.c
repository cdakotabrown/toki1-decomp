/* @SymbolStart: 0x8013556C */
/* @RodataBase: 0x8015D684 */
/* @SymbolSize: 220 */
#include "types.h"

extern u8 taco_menu_csr;

extern void taco_str_disp2(void* a0, s32 a1, s32 a2, s32 a3);

extern char taco_menu_str_disp_str0[];
extern char taco_menu_str_disp_str1[];
extern char taco_menu_str_disp_str2[];
extern char taco_menu_str_disp_str3[];
extern char taco_menu_str_disp_str4[];

void taco_menu_str_disp(void) {
    u8 cmd = taco_menu_csr;

    switch (cmd) {
    case 0:
        taco_str_disp2(taco_menu_str_disp_str0, -64, 72, 2);
        break;
    case 1:
        taco_str_disp2(taco_menu_str_disp_str1, -64, 72, 3);
        break;
    case 2:
        taco_str_disp2(taco_menu_str_disp_str2, -64, 72, 5);
        break;
    case 3:
        taco_str_disp2(taco_menu_str_disp_str3, -64, 72, 6);
        break;
    case 4:
        taco_str_disp2(taco_menu_str_disp_str4, -64, 72, 0);
        break;
    }
}
