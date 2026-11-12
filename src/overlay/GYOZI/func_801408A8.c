/* @SymbolStart: 0x8014088C */
/* @SymbolSize: 96 */
/* func_801408A8 -- GYOZI overlay, Address: 0x8014088C (prologue 0x801408A8), */
/* Size: 96 bytes (24 insns).  Clears D_80148624, then sets D_80148620 to 39 */
#include "types.h"

extern void func_8004DE1C(void);

extern u8 D_80148818;
extern u8 D_80148828[];
extern s16 D_80148620;
extern s16 D_80148624;

void func_801408A8(void)
{
    D_80148624 = 0;
    if (D_80148828[D_80148818] == 11) {
        D_80148620 = 39;
    } else {
        D_80148620 = 38;
    }
    func_8004DE1C();
}
