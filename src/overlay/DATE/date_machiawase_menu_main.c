/* @SymbolStart: 0x8013F420 */
/* @SymbolSize: 64 */
/* Function: date_machiawase_menu_main */
/* Overlay: DATE */
/* Address: 0x8013F420 (cross-prologue head) / 0x8013F428 (body) */
/* Size: 64 bytes (16 instructions) */
#include "types.h"

extern u8 machiawase_basyo3;

extern void menu3_main(void);
extern void menu2_main(void);

void date_machiawase_menu_main(void)
{
    if (machiawase_basyo3 != 255) {
        menu3_main();
    } else {
        menu2_main();
    }
}
