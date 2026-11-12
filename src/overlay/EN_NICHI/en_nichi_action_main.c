/* @SymbolStart: 0x80132EC0 */
/* @SymbolSize: 128 */
/* en_nichi_action_main - EN_NICHI overlay, Address: 0x80132EC0, Size: 128 bytes (32 insns) */
#include "types.h"

extern s32 en_nichi_action_step;
extern void en_nichi_press_main();
extern void en_nichi_release_main();
extern void en_nichi_slot_retire();
extern void en_nichi_timed_wait();

void en_nichi_action_main(void)
{
    s32 step = en_nichi_action_step;

    switch (step) {
    case 0: en_nichi_press_main(); break;
    case 1: en_nichi_release_main(); break;
    case 2: en_nichi_slot_retire(); break;
    case 3: en_nichi_timed_wait(); break;
    }
}
