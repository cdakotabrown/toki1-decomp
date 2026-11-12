/* @SymbolStart: 0x8013AA50 */
/* @SymbolSize: 96 */
/* Address: 0x8013AA58 */
/* Size: 88 bytes (22 instructions) */
#include "types.h"

extern void shougatu_girl4_kind0_step(void);
extern void shougatu_girl4_kind1_step(void);
extern void shougatu_girl4_kind_other_step(void);
extern u8 shougatu_date_kind;

void shougatu_part4(void)
{
    switch (shougatu_date_kind) {
    case 0:
        shougatu_girl4_kind0_step();
        goto end;
    case 1:
        shougatu_girl4_kind1_step();
        goto end;
    default:
        shougatu_girl4_kind_other_step();
    }
end:
    ;
}
