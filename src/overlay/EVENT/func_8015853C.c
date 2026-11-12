/* @SymbolStart: 0x80158514 */
/* @SymbolSize: 116 */
#include "types.h"

typedef struct {
    unsigned lo : 12;
    unsigned b12 : 1;
    unsigned b13 : 1;
    unsigned hi : 18;
    u8 pad[48];
} Rec52;

extern u8 D_800B1746;
extern Rec52 D_800B0A10[];
extern void FUN_80011DFC(void);

void func_8015853C(void)
{
    Rec52 *r;

    r = &D_800B0A10[D_800B1746];
    if (r->b12) {
        goto call_A;
    }
    if (!r->b13) {
        goto call_BC;
    }
call_A:
    FUN_80011DFC();
    return;
call_BC:
    FUN_80011DFC();
    FUN_80011DFC();
}
