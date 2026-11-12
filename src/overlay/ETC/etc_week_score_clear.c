/* @SymbolStart: 0x80146AAC */
/* @SymbolSize: 128 */
/* Address: 0x80146AB4 (true entry 0x80146AAC, 2 insns hoisted above .sym) */
#include "types.h"

extern void sub_step_up(void);

extern u8 now_youbi;
extern s32 work_fuku[];

void etc_week_score_clear(void)
{
    s32 i;

    if (now_youbi == 0) {
        for (i = 0; i < 6; i++) {
            work_fuku[i + 4] = 0;
            work_fuku[i + 10] = 0;
        }
    }
    sub_step_up();
}
