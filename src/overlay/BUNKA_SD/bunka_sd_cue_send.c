/* @SymbolStart: 0x80134B88 */
/* bunka_sd_cue_send - BUNKA_SD overlay, Address: 0x80134B90, Size: 516 bytes (129 insns) */
/* @SymbolSize: 524 */
#include "types.h"

extern void device_cmd_send(s32);

extern s32 bunka_sd_kind;

extern s32 step_counter;

void bunka_sd_cue_send(void)
{
    switch ((u32)step_counter) {
    case 1266:
        device_cmd_send(1289);
        break;
    case 1346:
        device_cmd_send(1289);
        break;
    case 1426:
        device_cmd_send(1289);
        break;
    case 1542:
        device_cmd_send(1280);
        break;
    case 240:
        device_cmd_send(1290);
        break;
    case 336:
        device_cmd_send(1290);
        break;
    case 432:
        device_cmd_send(1290);
        break;
    case 528:
        device_cmd_send(1290);
        break;
    case 624:
        device_cmd_send(1290);
        break;
    case 720:
        device_cmd_send(1290);
        break;
    case 816:
        device_cmd_send(1290);
        break;
    case 912:
        device_cmd_send(1290);
        break;
    case 1008:
        device_cmd_send(1290);
        break;
    case 1104:
        device_cmd_send(1290);
        break;
    case 1552:
        device_cmd_send(1282);
        break;
    case 1200:
        if (bunka_sd_kind == 3) {
            device_cmd_send(1290);
        }
        break;
    case 1312:
        if (bunka_sd_kind == 3) {
            device_cmd_send(1290);
        }
        break;
    default:
        break;
    }
}
