/* @SymbolStart: 0x801320C0 */
/* @RodataBase: 0x80139440 */
/* @SymbolSize: 172 */
/* en_nichi_sub_main - EN_NICHI overlay, Address: 0x801320C0, Size: 172 bytes (43 insns) */
#include "types.h"

extern u8 now_sub_step;
extern void en_nichi_init_main();
extern void en_nichi_intro_main();
extern void en_nichi_class_init();
extern void en_nichi_work_clear();
extern void sub_step_up();
extern void en_nichi_play_main();
extern void en_nichi_score_main();
extern void en_nichi_exit();

void en_nichi_sub_main(void)
{
    u8 scene = now_sub_step;

    switch (scene) {
    case 0: en_nichi_init_main(); break;
    case 1: en_nichi_intro_main(); break;
    case 2: en_nichi_class_init(); en_nichi_work_clear(); sub_step_up(); break;
    case 3: en_nichi_play_main(); break;
    case 4: en_nichi_score_main(); break;
    case 5: en_nichi_exit(); break;
    }
}
