/* @SymbolStart: 0x80139900 */
/* @SymbolSize: 144 */
#include "types.h"

extern void shougatu_girl1_kind0_step(void);
extern void shougatu_girl1_kind1_step(void);
extern void shougatu_girl1_kind2_step(void);
extern void shougatu_girl1_kind3_step(void);
extern void shougatu_girl1_kind_other_step(void);
extern u8 shougatu_date_kind;

void shougatu_part1(void)
{
    switch (shougatu_date_kind) {
    case 0:
        shougatu_girl1_kind0_step();
        break;
    case 1:
        shougatu_girl1_kind1_step();
        break;
    case 2:
        shougatu_girl1_kind2_step();
        break;
    case 3:
        shougatu_girl1_kind3_step();
        break;
    default:
        shougatu_girl1_kind_other_step();
        break;
    }
}
