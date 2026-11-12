/* Function: cd_command_issue */
/* Address: 0x80045630 */
/* Size: 460 bytes (115 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_instructions 0x1c 0x28 */
/* @PostProc: swap_instructions 0x20 0x28 */
/* @PostProc: scheduler_normalizer */
#include "types.h"

extern s32 last_sector;

extern s32 cdq_command_enqueue(s32, s32, void*);

void cd_command_issue(u8 arg0, s32 arg1, void *arg2) {
    s32 val;
    u8 sp40[12];
    u8 pad[12];

    val = arg1;
    if ((u32)arg1 >= 0x4AB51U) {
        val = 0x4AB50;
    }

    switch (arg0) {
        case 0:
            cdq_command_enqueue(0, 0, 0);
            break;
        case 9:
            cdq_command_enqueue(9, 0, 0);
            break;
        case 6:
            cdq_command_enqueue(9, 0, 0);
            last_sector = val;
            cdq_command_enqueue(6, val, arg2);
            break;
        case 14:
            cdq_command_enqueue(14, 0, arg2);
            break;
        case 13:
            if (val == 0) {
                cdq_command_enqueue(13, 0, arg2);
            } else {
                cdq_command_enqueue(13, 0, arg2);
                cdq_command_enqueue(2, val, 0);
                cdq_command_enqueue(27, val, 0);
                last_sector = val;
            }
            break;
        case 2:
            cdq_command_enqueue(2, val, 0);
            break;
        case 21:
            cdq_command_enqueue(2, val, 0);
            cdq_command_enqueue(21, val, 0);
            break;
        case 10:
            cdq_command_enqueue(10, val, 0);
            break;
        case 3:
            sp40[0] = 5;
            cdq_command_enqueue(14, 0, sp40);
            cdq_command_enqueue(2, val, 0);
            cdq_command_enqueue(3, 0, 0);
            break;
        case 27:
        default:
            break;
    }
}
