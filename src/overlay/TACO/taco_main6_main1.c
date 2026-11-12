/* @CompileEnv: IDO52_FORCE_WEB_COLOR=4:v1 IDO52_AS1_XBB_VOLATILE_OK=2 IDO52_UGEN_TEMP_REG=2:t8,3:t7,12:t7 IDO52_AS1_TIE_TAKE=55 IDO52_AS1_TIE_REJECT=66 */
/* @SymbolSize: 456 */
/* Address: 0x8013789C, Size: 456 bytes (114 insns; the .sym's 468 over-runs */
#include "types.h"
extern volatile s32 pad_data;
extern s32 pad_trig;
extern volatile s32 step_state;

extern void taco_phase_main();
extern void taco_objects_init();
extern void taco_obj_clear();
extern void all_object_disp_off();
extern void taco_actor_pool_clear();
extern void disp_mode_set2();
extern void goto_sub_step();
extern void taco_drop();
extern void taco_time_disp();
extern void taco_play();

void taco_main6_main1(void)
{
    s32 v1;
    u32 x;

    taco_phase_main();
    if (pad_trig != 0) {
        taco_objects_init();
        taco_obj_clear();
        all_object_disp_off();
        taco_actor_pool_clear();
        disp_mode_set2(0);
        goto_sub_step(2);
    } else {
        v1 = pad_data;
        pad_trig = pad_trig | 0x20;
        x = (u32)step_state & 0x1FF;
        v1 = v1 | 0x20;
        if (x >= 121 && x < 140) {
            v1 = v1 | 0x2000;
        }
        if (x >= 171 && x < 190) {
            v1 = v1 | 0x4000;
        }
        if (x >= 221 && x < 240) {
            v1 = v1 | 0x8000;
        }
        if (x >= 271 && x < 290) {
            v1 = v1 | 0x1000;
        }
        if (x >= 401 && x < 440) {
            v1 = v1 | 0x3000;
        }
        if (x >= 461 && x < 480) {
            v1 = v1 | 0xC000;
        }
        if (x >= 401 && x < 440) {
            v1 = v1 | 0x6000;
        }
        pad_data = v1;
        if (x >= 461) {
            pad_data = v1;
            if (x < 480) {
                pad_data = v1 | 0x9000;
            }
        }
        taco_drop(1, 1);
        taco_time_disp();
        taco_play();
    }

    if ((u32)step_state >= 1801) {
        taco_objects_init();
        taco_obj_clear();
        all_object_disp_off();
        taco_actor_pool_clear();
        disp_mode_set2(0);
        goto_sub_step(2);
    }
}
