/* Function: title_move */
/* Address: 0x800623A0 */
/* Size: 132 bytes (33 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v0 v1 */
#include "types.h"

extern u32 step_counter;
extern u8 now_sub_sub_step;
extern void title_move_show(void);
extern void title_white_out(void);
extern void title_white_in(void);
extern void title_obj_move(void);

void title_move(void) {
    step_counter++;
    {
        s32 mode = now_sub_sub_step;
        switch (mode) {
        case 0:
            title_move_show();
            break;
        case 1:
            title_white_out();
            break;
        case 2:
            title_white_in();
            break;
        }
    }
    title_obj_move();
}
