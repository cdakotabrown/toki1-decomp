/* Address: 0x8006AF70 */
/* Size: 336 bytes (84 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 screen_w[];

s32 reserved_date_check(s32 level, s32 kind, s32 sub) {
    s32 r = 0;
    s32 i;

    if (kind == 13) {
        kind = 1;
    }
    if (kind == 12 && sub == 24) {
        return -1;
    }
    if (kind == 13 && sub == 1) {
        return -1;
    }
    if (kind == 1 && sub == 1) {
        return -1;
    }
    if (kind >= 3 && level >= 98) {
        return -1;
    }
    for (i = 0; i < 8; i++) {
        if ((u32)*(u16 *)(&screen_w[i * 4] + 1356) >> 12 == kind) {
            if ((*(u16 *)(&screen_w[i * 4] + 1358) & 0x1F) == sub) {
                r = i + 1;
            }
        }
    }
    return r;
}
