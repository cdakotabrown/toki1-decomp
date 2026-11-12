/* @SymbolStart: 0x801392FC */
/* @SymbolSize: 68 */
/* Address: 0x801392FC (cross-prologue head) / 0x80139304 (body) */
/* Size: 68 bytes (17 instructions) */
#include "types.h"

extern s16 para_ri;
extern s16 now_serifu;

extern void sub_sub_step_up(void);

void date_basyo6_para_ri_step(void) {
    if (para_ri < 120) {
        now_serifu += 2;
    }
    sub_sub_step_up();
}
