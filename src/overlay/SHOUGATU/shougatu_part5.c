/* @SymbolStart: 0x8013A290 */
/* @SymbolSize: 60 */
/* Address: 0x8013A298 */
/* Size: 52 bytes (13 instructions) */
#include "types.h"

extern void shougatu_girl5_kind0_step(void);
extern void shougatu_girl5_kind_other_step(void);
extern u8 shougatu_date_kind;

void shougatu_part5(void)
{
    if (shougatu_date_kind == 0) {
        shougatu_girl5_kind0_step();
    } else {
        shougatu_girl5_kind_other_step();
    }
}
