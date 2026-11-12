/* Function: icon_schedule_set (icon_schedule_set) */
/* Address: 0x8006CF20 */
/* Size: 180 bytes (45 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers s1 s2 --bidirectional */
/* @PostProc: remap_stack_slots --map '28:32,32:28' */
/* @PostProc: swap_instructions 0x18 0x20 */
/* @PostProc: swap_instructions 0xa4 0xa8 */
/* @PostProc: swap_commutative_operands --offset 0x34 */
#include "types.h"

extern u8 schedule_cursor;
extern u8 screen_w[];
extern u8 icon_mode;
extern s32 sched_icon;
extern void icon_can_use_set(s32, s32);

void icon_schedule_set(void) {
    s32 i;
    u8 *arr = screen_w;
    s32 bound = 14;

    i = 0;
    do {
        if (i != arr[(s32)schedule_cursor * 4 + 1693]) {
            icon_can_use_set(i, 0);
        } else {
            icon_can_use_set(i, 1);
        }
        i++;
    } while (i != bound);

    if (icon_mode == 2) {
        if ((sched_icon & 0xF) != 14) {
            icon_can_use_set(14, 0);
        }
    }
}
