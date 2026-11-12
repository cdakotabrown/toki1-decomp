/* Function: magazine_slot_get — school-year -> schedule-slot table lookup */
/* Address: 0x80077D34 */
/* Size: 392 bytes (98 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 now_year;
extern u8 now_month;

s32 magazine_slot_get(void)
{
    s32 r;
    u32 m;

    r = now_year;
    switch (r) {
    case 0x5F:
        m = now_month;
        if (m < 5) {
            r = 0;
            break;
        }
        if (m < 8) {
            r = 1;
            break;
        }
        if (m < 11) {
            r = 2;
            break;
        }
        r = 3;
        break;
    case 0x60:
        m = now_month;
        if (m < 2) {
            r = 3;
            break;
        }
        if (m < 5) {
            r = 4;
            break;
        }
        if (m < 8) {
            r = 5;
            break;
        }
        if (m < 11) {
            r = 6;
            break;
        }
        r = 7;
        break;
    case 0x61:
        m = now_month;
        if (m == 8) {
            r = 13;
            break;
        }
        if (m < 2) {
            r = 7;
            break;
        }
        if (m < 5) {
            r = 8;
            break;
        }
        if (m < 8) {
            r = 9;
            break;
        }
        if (m < 11) {
            r = 10;
            break;
        }
        r = 11;
        break;
    case 0x62:
        m = now_month;
        if (m < 2) {
            r = 11;
            break;
        }
        r = 12;
        break;
    }
    return r;
}
