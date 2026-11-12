/* @SymbolSize: 84 */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --from 28 --to 43 */
/* shugaku_event_class_clear - SHUGAKU overlay, Address: 0x80134814, Size: 84 bytes (21 insns) */
#include "types.h"

extern u8 now_girl;
extern void get_g_zyotai_h(s32 a0);
extern void bustup_timer_clamp(s32 a0);
extern void class_clear_n(s32 a0);
extern void sub_sub_step_up(void);

void shugaku_event_class_clear(void)
{
    s32 spill;

    get_g_zyotai_h((u8)now_girl);
    bustup_timer_clamp((u8)spill);
    class_clear_n(96);
    class_clear_n(97);
    class_clear_n(98);
    sub_sub_step_up();
}
