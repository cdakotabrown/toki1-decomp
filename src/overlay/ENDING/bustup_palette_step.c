/* @SymbolSize: 64 */
#include "types.h"

extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, s32 stack4);
extern void sub_sub_step_up(void);

extern s32 bustup_check_init_tbl3;

void bustup_palette_step(void)
{
    palette_upload_enqueue(bustup_check_init_tbl3, 16, 1, 2, 1);
    sub_sub_step_up();
}
