/* Function: CD_readsync */
/* Address: 0x8008AE24 */
/* Size: 228 bytes (57 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc272 */
/* @CompileFlags: -O2 -G0 -mips1 --fill-earliest */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x04 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x08 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xc8 */
/* @PostProc: swap_instructions --offset1 0xcc --offset2 0xd8 */
/* @PostProc: swap_instructions --offset1 0xd0 --offset2 0xd8 */
/* @PostProc: swap_instructions --offset1 0xd4 --offset2 0xd8 */
#include "types.h"

extern s32 libcd_read_timeout_vsync[];
extern u8 Result[8];

s32 VSync(s32);
void cd_read_retry(void);
void CD_datasync(s32);

s32 CD_readsync(s32 once, u8 *dest)
{
    volatile s32 *p;
    s32 i;
    u8 *d;
    u8 *s;

    p = libcd_read_timeout_vsync;
    do {
        s = Result;
        d = dest;
        if (d != 0) {
            for (i = 7; i != -1; i--) {
                *d++ = *s++;
            }
        }
        if (p[0] < VSync(-1)) {
            p[-1] = -1;
        }
        if (p[-1] < 0) {
            cd_read_retry();
        }
        if (p[-1] == 0) {
            CD_datasync(0);
        }
    } while (once == 0 && p[-1] > 0);
    return p[-1];
}
