/* @SymbolStart: 0x80153F70 */
/* @SymbolSize: 72 */
/* Function: date_serifuev_cmd517 */
/* Overlay: DATE */
/* Address: 0x80153F70 (cross-prologue head) / 0x80153F78 (body) */
/* Size: 72 bytes (18 instructions) */
#include "types.h"

extern volatile u8 step_mode;

extern void device_cmd_send(s32 id);
extern void func_8014C530(void);

void date_serifuev_cmd517(void)
{
    if (step_mode == 0) {
        device_cmd_send(517);
        step_mode = step_mode + 1;
    }
    func_8014C530();
}
