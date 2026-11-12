/* Function: msg_part_reorder */
/* Address: 0x80082F60 */
/* Size: 348 bytes (87 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 352 --new 368 */
/* @PostProc: remap_stack_slots --map 96:100 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: strip_trailing_instructions --count 1 --nop-only */
#include "types.h"

extern void strncpy(u8 *dst, u8 *src, s32 size);
extern void sprintf(u8 *str, u8 *fmt, u8 *buf0, u8 *buf1, u8 *buf2);

extern u8 junban;
extern u8 menu3_select_tbl0;
extern u8 menu3_select_tbl1;

extern u8 msg_part_reorder_str0[];

void msg_part_reorder(u8 *arg0) {
    u8 buf[4][64];
    s32 pad;
    s32 pad2;
    s16 count;
    s16 start;
    s16 i;

    (void)&pad;
    (void)&pad2;
    count = 0;
    start = 0;
    i = 0;

    do {
        if (arg0[i] == 'P' || arg0[i] == 'G') {
            i--;
        } else if (arg0[i] == 'q' || arg0[i] == ' ') {
            arg0[i] = 0;
            strncpy(buf[count], arg0 + start, 64);
            count++;
            start = (s16)(i + 1);
            i--;
        } else if (arg0[i] == 0) {
            strncpy(buf[count], arg0 + start, 64);
            break;
        }
        i = (s16)(i + 2);
    } while (1);

    sprintf(arg0, msg_part_reorder_str0,
                  buf[junban], buf[menu3_select_tbl0], buf[menu3_select_tbl1]);
}
