/* Function: title_set */
/* Address: 0x80062848 */
/* Size: 124 bytes (31 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v0 v1 */
#include "types.h"

extern u32 step_counter;
extern u8 now_sub_sub_step;
extern void title_show_init(void);
extern void title_show(void);
extern void title_elem(void);

void title_set(void) {
    step_counter++;
    {
        s32 mode = now_sub_sub_step;
        switch (mode) {
        case 0:
            title_show_init();
            break;
        case 1:
            title_show();
            break;
        case 2:
            title_elem();
            break;
        }
    }
}
