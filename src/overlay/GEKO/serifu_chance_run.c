/* @SymbolStart: 0x8013396C */
/* @SymbolSize: 64 */
/* serifu_chance_run -- GEKO overlay, Address: 0x80133974, Size: 56 bytes (14 insns) */
#include "types.h"

extern void geko_serifu_run(void);
extern void sub_sub_step_up(void);

extern u8 geko_serifu_state;

void serifu_chance_run(void)
{
    s32 t6 = geko_serifu_state;

    if (t6 != 100) goto label0;
    geko_serifu_run();
    goto label1;

label0:
    sub_sub_step_up();

label1:
    ;
}
