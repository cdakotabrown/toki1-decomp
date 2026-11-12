/* @SymbolStart: 0x801586C4 */
/* @SymbolSize: 140 */
/* @RodataBase: 0x801200E0 */
#include "types.h"

extern s32 D_80125320;

extern void func_8011DDB8(void);
extern void func_8011EDFC(void);
extern void func_8011D5C4(void);
extern void func_8011C750(void);

void func_801586CC(void) {
    s32 cmd_v = D_80125320;

    switch (cmd_v) {
    case 0:
        func_8011DDB8();
        break;
    case 1:
        func_8011EDFC();
        break;
    case 2:
        func_8011D5C4();
        break;
    case 3:
        func_8011C750();
        break;
    case 4:
        func_8011C750();
        break;
    }
}
