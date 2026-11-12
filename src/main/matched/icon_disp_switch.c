/* Address: 0x80065144 */
/* Size: 164 bytes (41 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 icon_num;
extern u8 class_w[];
extern u8 screen_w[];

void icon_disp_switch(s32 on) {
    u8 *q = screen_w;
    u32 i;
    u8 *p;

    if (on != 0) {
        i = 0;
        if (icon_num != 0) {
            p = class_w;
            do {
                p[5443] = p[5443] | 0x80;
                p[4355] = p[4355] | 0x80;
                i++;
                p += 68;
            } while (i < q[3949]);
        }
    } else {
        i = 0;
        if (icon_num != 0) {
            p = class_w;
            do {
                p[5443] = p[5443] & 0x7F;
                p[4355] = p[4355] & 0x7F;
                i++;
                p += 68;
            } while (i < q[3949]);
        }
    }
}
