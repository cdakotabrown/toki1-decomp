/* Function: move_255_line */
/* Address: 0x8004D680 */
/* Size: 168 bytes (42 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: scheduler_normalizer */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void image_rect_move(s32 a0, s32 a1, s32 a2, s32 a3, s32 sp10, s32 sp14);

void move_255_line(s32 start, s32 end, s32 flags) {
    s32 i;
    s32 val1;
    s32 val2;
    s32 row;

    if (start > end) {
        return;
    }

    val1 = (flags & 0xF) << 6;
    val2 = ((flags & 0x10) << 4) & 0xFFFF;
    row = (start << 6) + 63;
    i = start;

    do {
        image_rect_move(row & 0xFFFF, 0, 1, 255,
                      val1 + i - start, val2);
        row += 64;
        i++;
    } while (i <= end);
}
