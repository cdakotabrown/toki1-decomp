/* @SymbolStart: 0x80135F38 */
/* bunka_sd_cue_send2 - BUNKA_SD overlay, Address: 0x80135F40, Size: 632 bytes (158 insns) */
/* @SymbolSize: 640 */
/* @PostProc: patch_insn_reg_field 0x248:rs:v1:a0 0x248:rt:a0:v1 0x268:rs:v1:a0 0x268:rt:a0:v1 */
#include "types.h"

extern void device_cmd_send(s32);

extern s32 step_counter;
extern s32 bunka_sd_kind;

void bunka_sd_cue_send2(void)
{
    s32 sel;

    switch ((u32)step_counter) {
    case 146:
        device_cmd_send(1283);
        step_counter = step_counter;
        break;
    case 373:
        device_cmd_send(1283);
        step_counter = step_counter;
        break;
    case 517:
        device_cmd_send(1283);
        step_counter = step_counter;
        break;
    case 659:
        device_cmd_send(1283);
        step_counter = step_counter;
        break;
    case 1119:
        device_cmd_send(1283);
        step_counter = step_counter;
        break;
    case 1219:
        device_cmd_send(1283);
        step_counter = step_counter;
        break;
    case 1321:
        device_cmd_send(1283);
        step_counter = step_counter;
        break;
    case 617:
        device_cmd_send(1287);
        step_counter = step_counter;
        break;
    case 966:
        device_cmd_send(1291);
        step_counter = step_counter;
        break;
    case 967:
        device_cmd_send(1286);
        step_counter = step_counter;
        break;
    case 998:
        device_cmd_send(1286);
        step_counter = step_counter;
        break;
    case 1030:
        device_cmd_send(1286);
        step_counter = step_counter;
        break;
    case 1062:
        device_cmd_send(1286);
        step_counter = step_counter;
        break;
    case 730:
        device_cmd_send(1284);
        step_counter = step_counter;
        break;
    case 794:
        device_cmd_send(1284);
        step_counter = step_counter;
        break;
    case 858:
        device_cmd_send(1284);
        step_counter = step_counter;
        break;
    default:
        break;
    }

    sel = bunka_sd_kind;
    if (sel != 3) {
        if (step_counter == 935) {
            device_cmd_send(1285);
            return;
        }
    }
    if (sel == 3) {
        if (step_counter == 925) {
            device_cmd_send(1285);
        }
    }
}
