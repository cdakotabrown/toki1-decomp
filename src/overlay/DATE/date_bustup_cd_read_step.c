/* @SymbolStart: 0x8013BF30 */
/* @SymbolSize: 100 */
/* Function: date_bustup_cd_read_step */
/* Overlay: DATE */
/* Address: 0x8013BF30 (cross-prologue head) / 0x8013BF38 (body) */
/* Size: 100 bytes (25 instructions) */
#include "types.h"

extern u8 now_basyo;

extern void sub_sub_step_up(void);
extern void date_basyo20_bustup_cd_read(void);
extern void date_basyo18_bustup_cd_read(void);

void date_bustup_cd_read_step(void)
{
    if (now_basyo == 18) {
        date_basyo18_bustup_cd_read();
        sub_sub_step_up();
    } else if (now_basyo == 20) {
        date_basyo20_bustup_cd_read();
        sub_sub_step_up();
    }

    sub_sub_step_up();
}
