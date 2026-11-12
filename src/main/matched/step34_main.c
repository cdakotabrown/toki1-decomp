/* Address: 0x8005A300 */
/* Size: 240 bytes (60 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v0:v1 --unidirectional */
#include "types.h"

extern u8 now_sub_step;
extern u8 mascot_cw_disp;

extern void step34_main0(void);
extern void step34_main1(void);
extern void cal_menu(void);
extern void menu_step_main(void);
extern void system_config_main(void);
extern void join_club(void);
extern void step34_mainff(void);
extern void mascot_move(void);
extern void update_anim_state(s32);

void step34_main(void) {
    u8 v1 = now_sub_step;

    if ((u32)v1 >= 6U) {
        if (v1 != 0xFF) {
            goto tail;
        }
        goto call_ff;
    }

    switch (v1) {
        case 0: step34_main0(); break;
        case 1: step34_main1(); break;
        case 2: cal_menu(); break;
        case 3: menu_step_main(); break;
        case 4: system_config_main(); break;
        case 5: join_club(); break;
    }
    goto tail;

call_ff:
    step34_mainff();

tail:

    if (mascot_cw_disp & 0x80) {
        mascot_move();
    }
    update_anim_state(0);
}
