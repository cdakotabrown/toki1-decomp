/* @SymbolStart: 0x80137AD0 */
/* @SymbolSize: 268 */
/* @RodataBase: 0x8013C0A0 */
/* ending_girl_dispatch - ENDING overlay, Address: 0x80137AD0, Size: 268 bytes (67 instructions) */
#include "types.h"

extern u8 now_girl;
extern void ending_girl0_cue_step();
extern void ending_girl1_cue_step();
extern void ending_girl2_cue_step();
extern void ending_girl3_cue_step();
extern void ending_girl4_cue_step();
extern void ending_girl5_cue_step();
extern void ending_girl6_cue_step();
extern void ending_girl7_cue_step();
extern void ending_girl8_cue_step();
extern void ending_girl9_cue_step();
extern void ending_cuea();
extern void ending_cueb();
extern void ending_cuec();

void ending_girl_dispatch(void)
{
    u8 step = now_girl;

    switch (step) {
        case 0: ending_girl0_cue_step(); break;
        case 1: ending_girl1_cue_step(); break;
        case 2: ending_girl2_cue_step(); break;
        case 3: ending_girl3_cue_step(); break;
        case 4: ending_girl4_cue_step(); break;
        case 5: ending_girl5_cue_step(); break;
        case 6: ending_girl6_cue_step(); break;
        case 7: ending_girl7_cue_step(); break;
        case 8: ending_girl8_cue_step(); break;
        case 9: ending_girl9_cue_step(); break;
        case 10: ending_cuea(); break;
        case 12: ending_cueb(); break;
        default: ending_cuec(); break;
    }
}
