/* @SymbolStart: 0x80132000 */
/* @SymbolSize: 416 */
/* @RodataBase: 0x8015D3F0 */
/* Address: 0x8013201C */
/* Size: 388 bytes (97 insns) */
/* @SymbolSize: 388 */
#include "types.h"

typedef struct {
    u32 frame;
    u32 frame4;
    u8  pad8;
    u8  step;
    u8  stepA;
    u8  padB;
} TacoTop;
extern TacoTop step_state;
extern void taco_case0_boot_main(void);
extern void taco_step_dispatch4(void);
extern void taco_slot_dispatch(void);
extern void taco_select_step(void);
extern void taco_step_dispatch3(void);
extern void taco_subsub_dispatch(void);
extern void taco_case6_phase_main(void);
extern void taco_case7_cd_image_main(void);
extern void taco_case10_scene_play_main(void);
extern void taco_case11_play_main(void);
extern void taco_maina(void);
extern void taco_mainb(void);
extern void taco_mainc(void);
extern void taco_case31_stage_play_main(void);
extern void taco_case32_stage_play_main(void);
extern void taco_main1f(void);
extern void taco_step_dispatch(void);
extern void taco_main2d(void);
extern void taco_step_dispatch2(void);
extern void game_reset(void);
extern void taco_view_main(void);
extern void taco_obj_disp(void);

void taco_main(void) {
    step_state.frame += 1;
    switch (step_state.step) {
    case 0:  taco_case0_boot_main(); break;
    case 1:  taco_step_dispatch4(); break;
    case 2:  taco_slot_dispatch(); break;
    case 3:  taco_select_step(); break;
    case 4:  taco_step_dispatch3(); break;
    case 5:  taco_subsub_dispatch(); break;
    case 6:  taco_case6_phase_main(); break;
    case 7:  taco_case7_cd_image_main(); break;
    case 10: taco_case10_scene_play_main(); break;
    case 11: taco_case11_play_main(); break;
    case 12: taco_maina(); break;
    case 21: taco_mainb(); break;
    case 22: taco_mainc(); break;
    case 31: taco_case31_stage_play_main(); break;
    case 32: taco_case32_stage_play_main(); break;
    case 8:  taco_main1f(); break;
    case 9:  taco_step_dispatch(); break;
    case 45: taco_main2d(); break;
    case 46: taco_step_dispatch2(); break;
    default: game_reset(); break;
    }
    taco_view_main();
    taco_obj_disp();
}
