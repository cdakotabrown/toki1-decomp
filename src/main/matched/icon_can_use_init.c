/* Function: icon_can_use_init */
/* Address: 0x80075BAC */
/* Size: 240 bytes (60 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: scheduler_normalizer */
#include "types.h"

extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u8 now_youbi;
extern u8 icon_mode;
extern u16 basyo_pack;
extern u8 screen_w[];

extern s32 reserved_date_check(u8 a0, u8 a1, u8 a2);
extern void icon_can_use_set(s32 a0, s32 a1);

void icon_can_use_init(void) {
    s32 count;
    u8 *ptr;
    u8 *end;

    if (reserved_date_check(now_year, now_month, now_day) <= 0) {
        icon_can_use_set(9, 0);
    }

    icon_can_use_set(13, 0);

    if (icon_mode == 2) {
        icon_can_use_set(14, 0);
    }

    if (now_youbi == 6) {
        count = 0;
        ptr = screen_w;
        end = screen_w + 44;
        do {

            if (ptr[0x66F] & 1) {
                count++;
            }
            ptr += 4;
        } while (ptr != end);
        if (count >= 11) {
            icon_can_use_set(4, 0);
        }
    } else if (((u32)basyo_pack >> 12) == 11) {
        icon_can_use_set(4, 0);
    }
}
