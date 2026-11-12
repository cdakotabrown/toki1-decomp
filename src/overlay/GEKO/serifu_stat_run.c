/* @SymbolStart: 0x80133164 */
/* @SymbolSize: 60 */
/* serifu_stat_run -- GEKO overlay, Address: 0x8013316C, Size: 52 bytes (13 insns) */
#include "types.h"

extern void geko_serifu_run(void);
extern void sub_sub_step_up(void);

extern u8 geko_stat_hi;

void serifu_stat_run(void)
{
    s32 t6 = geko_stat_hi;

    if (t6 == 0) goto label0;
    geko_serifu_run();
    goto label1;

label0:
    sub_sub_step_up();

label1:
    ;
}
