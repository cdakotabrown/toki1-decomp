/* @SymbolStart: 0x8014A924 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x24 */
/* @RodataBase: 0x8014D970 */
/* @SymbolSize: 208 */
#include "types.h"

extern s32 step_counter;
extern u8 now_sub_sub_step;

extern void etc_siken_main2_0(void);
extern void etc_siken_taku_select(void);
extern void etc_siken_main2_2(void);
extern void etc_siken_bg_load(void);
extern void etc_siken_sound_load(void);
extern void etc_siken_bustup_load(void);
extern void etc_siken_cw_main(void);
extern void etc_sound_main7(void);

void etc_siken_main2(void) {
    step_counter++;
    switch (now_sub_sub_step) {
    case 0:
        etc_siken_main2_0();
        break;
    case 1:
        etc_siken_taku_select();
        break;
    case 2:
        etc_siken_main2_2();
        break;
    case 3:
        etc_siken_bg_load();
        break;
    case 4:
        etc_siken_sound_load();
        break;
    case 5:
        etc_siken_bustup_load();
        break;
    case 6:
        etc_siken_cw_main();
        break;
    case 7:
        etc_sound_main7();
        break;
    }
}
