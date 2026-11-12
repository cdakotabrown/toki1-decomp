/* Address: 0x80062930 */
/* Size: 120 bytes (30 instructions) */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void device_cmd_send(s32 arg);
extern void white_light(s32 arg);
extern void class_clear(s32 arg);
extern void cursor_init(s32 arg);
extern void set_flag_b3_high_bit(s32 arg);
extern void prim_init(void);
extern void sub_sub_step_up(void);

extern u32 step_counter_u;

void title_fade_out(void)
{
    u32 counter;

    device_cmd_send(209);

    counter = step_counter_u;
    white_light(counter * 2);

    counter = step_counter_u;
    if (counter >= 128U) {
        class_clear(0);
        cursor_init(0);
        set_flag_b3_high_bit(0);
        prim_init();
        sub_sub_step_up();
    }
}
