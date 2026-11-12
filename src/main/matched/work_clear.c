/* Address: 0x80042108 */
/* Size: 272 bytes (68 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -Wo,-no_const_in_reg,-loopunroll,0 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 24 --new 40 */
/* @PostProc: swap_instructions 0x30 0x34 */
/* @PostProc: swap_instructions 0x3c 0x40 */
/* @PostProc: swap_instructions 0x58 0x5C */
/* @PostProc: swap_instructions 0x78 0x7C */
/* @PostProc: swap_instructions 0xA0 0xA4 */
/* @PostProc: swap_instructions 0xC0 0xC4 */
#include "types.h"

extern void bzero(void *dst, s32 size);

extern u8 screen_w[];
extern u8 screen_intmode;
extern u8 reserved_date;
extern u8 bukatu_rec;
extern u8 schedule_cursor;
extern u8 girl_no;
extern u8 machiawase_flag;
extern u8 rec_idx;
extern u8 rec_tbl;

void work_clear(void) {
    u8 *p;
    u8 *end;

    bzero(&reserved_date, 32);
    bzero(&bukatu_rec, 48);

    p = screen_w;
    end = screen_w + 0x80;
    do {
        schedule_cursor = 0;
        *(s32 *)(p + 0x69C) = 0;
        p += 4;
    } while (p < end);

    p = screen_w;
    end = screen_w + 0x100;
    do {
        *(u8 *)(p + 0x56C) = 0;
        p++;
    } while (p < end);

    p = screen_w;
    end = screen_w + 4;
    do {
        *(u8 *)(p + 0x724) = 0;
        p++;
    } while (p < end);

    girl_no = 0;

    p = screen_w;
    end = &screen_intmode;
    do {
        *(u8 *)(p + 0x728) = 0;
        p++;
    } while (p < end);

    p = screen_w;
    end = screen_w + 0x10;
    do {
        *(u8 *)(p + 0x72C) = 0;
        *(u8 *)(p + 0x72D) = 0;
        *(u8 *)(p + 0x72E) = 0;
        *(u8 *)(p + 0x72F) = 0;
        p += 4;
    } while (p != end);

    machiawase_flag = 0;
    rec_idx = 0;
    bzero(&rec_tbl, 2048);
}
