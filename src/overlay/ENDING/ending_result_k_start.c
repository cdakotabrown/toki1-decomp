/* @SymbolStart: 0x8013B614 */
/* @SymbolSize: 92 */
/* ending_result_k_start - ENDING overlay, Address: 0x8013B614, Size: 92 bytes (23 instructions) */
#include "types.h"

extern u32 step_counter;
extern void *work_tarao;
extern u32 event_girl_no;
extern s32 k_disp_start(void *arg);
extern void sub_sub_step_up(void);

s32 ending_result_k_start(void)
{
    u32 counter = step_counter;
    s32 lt = counter < 128;

    counter = counter + 1;
    step_counter = counter;
    if (lt) {
        return 0;
    }
    k_disp_start(work_tarao);
    event_girl_no = 1;
    sub_sub_step_up();

}
