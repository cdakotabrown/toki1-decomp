/* @SymbolStart: 0x8014A21C */
/* @SymbolSize: 104 */
/* Function: date_open_dispatch */
/* Overlay: DATE */
/* Address: 0x8014A21C (cross-prologue head) / 0x8014A224 (body) */
/* Size: 104 bytes (26 instructions) */
#include "types.h"

extern s16 date_open_sub;

extern void date_init0(void);
extern void date_open_main(void);
extern void date_disp2(void);

void date_open_dispatch(void)
{
    switch (date_open_sub) {
    case 0:
        date_init0();
        break;
    case 1:
        date_open_main();
        break;
    case 2:
        date_disp2();
        break;
    }
}
