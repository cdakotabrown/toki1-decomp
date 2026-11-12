/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x74 */
/* Address: 0x801482D8 */
/* Size: 168 bytes (42 insns) */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern s32 step_counter;
extern u8 taco_obj_3_attribute[];
extern u8 taco_obj_31_attribute[];
extern LIBGS_DOBJ2 taco_obj[];

extern void sub_sub_step_up(void);
extern void taco_drop(s32 a0, s32 a1);
extern void taco_timeline_step(s32 a0);
extern void taco_stage_reset(s32 a0);

void taco_maina_timeline_step(void)
{
    s32 v;
    s32 *p;

    taco_drop(1, 1);

    v = step_counter;
    if ((u32)v < 1150) {
        step_counter = v;
        taco_timeline_step(v);
    } else {
        taco_stage_reset(1);
        (*(s32 *)&taco_obj[1].attribute) = 0;
        (*(s32 *)&taco_obj[2].attribute) = 0;
        p = (s32 *)taco_obj_3_attribute;
        do {
            p = (s32 *)((u8 *)p + 64);
            *(s32 *)((u8 *)p - 64) = 0;
            *(s32 *)((u8 *)p - 48) = 0;
            *(s32 *)((u8 *)p - 32) = 0;
            *(s32 *)((u8 *)p - 16) = 0;
        } while ((u8 *)p != taco_obj_31_attribute);
        sub_sub_step_up();
    }

    v = step_counter + 1;
    step_counter = v;
}
