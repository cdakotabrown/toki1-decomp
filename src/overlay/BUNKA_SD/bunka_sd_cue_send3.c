/* @SymbolStart: 0x80136904 */
/* bunka_sd_cue_send3 -- BUNKA_SD overlay, Address: 0x8013690C */
/* @SymbolSize: 168 */
#include "types.h"

extern u32 step_counter;
extern s32 bunka_sd_kind;

extern void device_cmd_send(s32);

void bunka_sd_cue_send3(void)
{
    s32 param;
    u32 temp;

    param = bunka_sd_kind;

    if (param == 3) {
        temp = step_counter;
        switch (temp) {
        case 332:
            device_cmd_send(1281);
            break;
        case 447:
            device_cmd_send(1282);
            break;
        }
    } else {
        temp = step_counter;
        switch (temp) {
        case 343:
            device_cmd_send(1281);
            break;
        case 458:
            device_cmd_send(1282);
            break;
        }
    }
}
