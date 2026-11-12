/* @SymbolSize: 172 */
/* @PostProc: swap_registers t2:t3 */
/* @PostProc: swap_registers t1:t2 */
/* @PostProc: swap_registers t0:t1 */
/* @PostProc: swap_registers t9:t0 */
/* @PostProc: swap_registers t8:t9 */
/* @PostProc: scheduler_normalizer */
/* tel_reserved_clear - TEL overlay, Address: 0x80139850, Size: 180 bytes (45 insns) */
#include "types.h"

extern s32 reserved_date;
extern u8 screen_w[];
extern u8 tel_canceled_fmt;

extern void printf(s32, s32);
extern void reserved_date_sort(void);

void tel_reserved_clear(void) {
    s32 i;
    s32 flags;
    s32 byte_nibble;
    u8 *base;
    s32 nibble;

    nibble = ((u32)(reserved_date << 3)) >> 28;
    base = screen_w;
    i = 0;

    do {
        flags = *(s32 *)(base + 1356);
        if ((s32)(flags << 2) < 0) {
            byte_nibble = ((u32)(flags << 3)) >> 28;
            if (nibble == byte_nibble) {
                *(u8 *)(base + 1359) = (u8)(*(u8 *)(base + 1359) & 0xFFDF);
                printf((s32)&tel_canceled_fmt, i);
            }
        }
        i++;
        base += 4;
    } while (i != 8);

    reserved_date_sort();
}
