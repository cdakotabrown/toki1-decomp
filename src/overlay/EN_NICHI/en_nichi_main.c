/* @SymbolStart: 0x80132000 */
/* @SymbolSize: 64 */
/* en_nichi_main - EN_NICHI overlay, Address: 0x80132000, Size: 64 bytes (16 insns) */
#include "types.h"

extern u8 now_year;
extern void en_nichi_sub_main();
extern void en_nichi_sub_dispatch();

void en_nichi_main(void)
{
    u8 sel = now_year;

    if (sel == 95) {
        en_nichi_sub_main();
    } else {
        en_nichi_sub_dispatch();
    }
}
