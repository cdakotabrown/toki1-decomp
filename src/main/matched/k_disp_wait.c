/* Function: k_disp_wait */
/* Address: 0x800848C8 */
/* Size: 120 bytes (30 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x800848C8 */
/* @SymbolSize: 120 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: remap_stack_slots --map '27:43' */
#include "types.h"

extern s32 k_wait_cnt;
extern s32 pad_data;

extern s32 get_k_speed(void);
extern void k_speed_set(s32);
extern void k_disp_inc(void);

void k_disp_wait(void) {
    s32 mode = k_wait_cnt;
    u8 result;
    s32 below;

    below = (mode < 1);
    k_wait_cnt = mode - 1;
    if (below != 0) {
        k_wait_cnt = 0;
        result = get_k_speed();
        if (pad_data & 0x00600060) {
            k_speed_set(0);
        }
        k_disp_inc();
        k_speed_set(result);
    }
}
