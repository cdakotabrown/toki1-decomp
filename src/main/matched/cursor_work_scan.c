/* Function: cursor_work_scan */
/* Address: 0x8005A668 */
/* Size: 328 bytes (82 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @SymbolStart: 0x8005A660 */
/* @SymbolSize: 336 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 48 --new 64 */
/* @PostProc: remap_stack_slots 24:40 */
/* @PostProc: swap_instructions 0x28 0x2c */
/* @PostProc: swap_registers a0:v1 --bidirectional --exclude-offsets 0x130 */
/* @PostProc: swap_registers t7:t0 --only 0x134,0x13c */
#include "types.h"

extern void icon_init(void);
extern void icon_disp_switch(s32 arg);

extern s16 icon_xy[];

extern u8 icon_num;
extern u8 icon_mode;

void cursor_work_scan(void)
{
    s32 cnt;
    s32 zeroCnt;
    s16 *ptr;
    s16 *end;
    s16 x, y;
    s32 mode;

    cnt = 0;
    zeroCnt = 0;

    if (icon_mode != 2) {
        goto check_result;
    }

    ptr = &icon_xy[0];
    end = &icon_xy[32];

    do {
        x = ptr[0];
        if (x >= 125) {
            cnt++;
        }
        if (x < -134) {
            cnt++;
        }

        y = ptr[1];
        if (y < -84) {
            cnt++;
        }
        if (y >= 85) {
            cnt++;
        }

        if (x == 0 && y == 0) {
            zeroCnt++;
        }

        x = ptr[2];
        if (x >= 125) {
            cnt++;
        }
        if (x < -134) {
            cnt++;
        }

        y = ptr[3];
        ptr += 4;
        if (y < -84) {
            cnt++;
        }
        if (y >= 85) {
            cnt++;
        }

        if (x == 0 && y == 0) {
            zeroCnt++;
        }
    } while (ptr != end);

check_result:
    if (zeroCnt < 2) {
        return;
    }
    if (cnt == 0) {
        return;
    }

    mode = icon_mode;
    icon_num = 15;
    icon_mode = 0;
    icon_init();
    icon_disp_switch(0);
    icon_mode = *(u8 *)&mode;
}
