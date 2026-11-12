/* Address: 0x8006D784 */
/* Size: 268 bytes (67 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=1 IDO52_COPYPROP_VAR=-16 IDO52_FORCE_WEB_COLOR=5:a0,8:v1,60:a0,62:v0,70:a1,71:a1,72:v1,73:a2 IDO52_AS1_NO_PASS1_PROP=42 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 screen_w[];
extern u8 now_youbi;
extern u8 schedule_cursor;
extern volatile u8 schedule_cursor_v;

void schedule_cursor_set(void) {
    u32 n = now_youbi;
    u8 *p = screen_w;
    s32 i;
    s32 cur;

    for (i = 0; i < 32; i++) {
        if (p[1692] == 251) {
            if (n == 0) {
                schedule_cursor = i;
                if (p[1688] != 37) {
                    schedule_cursor = schedule_cursor_v - 1;
                    if (p[1684] != 37) {
                        schedule_cursor = schedule_cursor - 1;
                    }
                }
                cur = schedule_cursor;
                goto found;
            }
            n--;
        }
        p += 4;
    }
    cur = schedule_cursor;
found:
    if (cur != 0) {
        cur = (u8)(cur - 1);
    }
    schedule_cursor = cur;
    for (i = cur; i < 32; i++) {
        if (screen_w[i * 4 + 1692] == 37) {
            screen_w[i * 4 + 1692] = 0;
        }
        if (screen_w[i * 4 + 1692] == 255) {
            break;
        }
    }
}
