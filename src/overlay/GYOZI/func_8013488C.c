/* @SymbolStart: 0x8013483C */
/* @SymbolSize: 124 */
/* func_8013488C -- GYOZI overlay, Address: 0x8013483C (prologue 0x8013488C), */
/* Size: 124 bytes (31 insns).  Sets bit 7 of D_8012B8C3 / D_8012B907 / */
#include "types.h"

extern void func_8004DE1C(void);

extern volatile u8 D_8012B8C3;
extern volatile u8 D_8012B906;
extern volatile u8 D_8012B907;
extern volatile u8 D_8012B94A;
extern volatile u8 D_8012B94B;
extern u8 D_801461A0;

void func_8013488C(void)
{
    D_8012B8C3 |= 0x80;
    D_8012B906 = 1;
    D_8012B907 |= 0x80;
    D_8012B94A = 1;
    D_8012B94B |= 0x80;
    D_801461A0 = 1;
    func_8004DE1C();
}
