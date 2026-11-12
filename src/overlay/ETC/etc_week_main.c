/* @SymbolStart: 0x80146700 */
/* @SymbolSize: 176 */
/* @RodataBase: 0x8014D7D4 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x24 */
#include "types.h"

extern s32 step_counter;
extern u8 now_youbi;

extern void etc_week_youbi0_main(void);
extern void etc_week_youbi1_main(void);
extern void etc_week_youbi2_main(void);
extern void etc_week_youbi3_main(void);
extern void etc_week_youbi4_main(void);
extern void etc_week_result_main(void);

void etc_week_main(void) {
    step_counter++;
    switch (now_youbi) {
    case 0:
        etc_week_youbi0_main();
        break;
    case 1:
        etc_week_youbi1_main();
        break;
    case 2:
        etc_week_youbi2_main();
        break;
    case 3:
        etc_week_youbi3_main();
        break;
    case 4:
        etc_week_youbi4_main();
        break;
    case 5:
        etc_week_result_main();
        break;
    }
}
