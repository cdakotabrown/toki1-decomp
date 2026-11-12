/* Address: 0x8006CBD0 */
/* Size: 60 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack 24 40 */
#include "types.h"

extern u8 now_sub_step;
extern void _schedule_init(void);
extern s32 get_last_gamen_mode(void);
extern void dec_bg_show(s32 a0);

void schedule_init(void) {
    u8 t6;
    s32 v0;

    t6 = now_sub_step;

    if (t6 == 0) {
        _schedule_init();
    }

    v0 = get_last_gamen_mode();
    dec_bg_show(v0);
}
