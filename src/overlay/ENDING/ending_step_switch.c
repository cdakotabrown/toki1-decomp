/* @SymbolStart: 0x80132334 */
/* @SymbolSize: 204 */
/* @RodataBase: 0x8013BF70 */
/* ending_step_switch - ENDING overlay, Address: 0x80132334, Size: 204 bytes (51 instructions) */
#include "types.h"

extern u8 now_sub_step;
extern void ending_select_main();
extern void ending_bustup_name_main();
extern void ending_serifu_scene_main();
extern void ending_calendar_main();
extern void ending_scene4();
extern void ending_scene5();
extern void ending_sub_step_switch();
extern void ending_song_main();
extern void ending_goto_stepc2();

void ending_step_switch(void)
{
    u8 scene = now_sub_step;

    switch (scene) {
        case 0: ending_select_main(); break;
        case 1: ending_bustup_name_main(); break;
        case 2: ending_serifu_scene_main(); break;
        case 3: ending_calendar_main(); break;
        case 4: ending_scene4(); break;
        case 5: ending_scene5(); break;
        case 6: ending_sub_step_switch(); break;
        case 7: ending_song_main(); break;
        default: ending_goto_stepc2(); break;
    }
}
