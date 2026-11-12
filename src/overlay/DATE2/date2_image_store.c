/* @SymbolSize: 104 */
/* @PostProc: patch_stack_complete --old 32 --new 56 */
/* @PostProc: remap_stack_slots --from 24 --to 44 --from 26 --to 46 --from 28 --to 48 --from 30 --to 50 */
/* date2_image_store - DATE2 overlay, Address: 0x80132F48, Size: 128 bytes (32 insns) */
/* @PostProc: swap_registers_auto v0:a0 */
#include "types.h"

extern u8 girl_no;
extern u8 now_girl;

void StoreImage(void *arg0, void *arg1);
void date_name_set(s32 arg0);
void date2_girl_res_set(void);
void sub_sub_step_up(void);

void date2_image_store(void)
{
    s16 rect[4];

    rect[0] = 320;
    rect[1] = 128;
    rect[2] = 384;
    rect[3] = 128;
    StoreImage(rect, (void *)0x80180000);
    now_girl = girl_no;
    date_name_set((s32)girl_no);
    date2_girl_res_set();
    sub_sub_step_up();
}
