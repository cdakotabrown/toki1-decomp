/* @SymbolStart: 0x80137FF0 */
/* @SymbolSize: 156 */
/* @RodataBase: 0x80143870 */
/* Address: 0x80137FF8 */
/* Size: 148 bytes (37 instructions) */
#include "types.h"

extern void shougatu_step_dispatch3(void);
extern void shougatu_step_dispatch4(void);
extern void shougatu_sprite_dispatch(void);
extern void shougatu_step_dispatch5(void);
extern void shougatu_step_dispatch6(void);
extern void shougatu_step_dispatch7(void);
extern u8 shougatu_date_kind;

void shougatu_part2(void)
{
    switch (shougatu_date_kind) {
        case 0:
            shougatu_step_dispatch3();
            break;
        case 1:
            shougatu_step_dispatch4();
            break;
        case 2:
            shougatu_sprite_dispatch();
            break;
        case 3:
            shougatu_step_dispatch5();
            break;
        case 4:
            shougatu_step_dispatch6();
            break;
        default:
            shougatu_step_dispatch7();
            break;
    }
}
