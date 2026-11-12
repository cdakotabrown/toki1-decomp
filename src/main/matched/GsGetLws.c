/* Function: GsGetLws */
/* Likely Symbol: GsGetLws */
/* Address: 0x8009AB00 */
/* Size: 660 bytes (165 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 --fill-earliest --fill-load-delay --jal-load-nop */
/* - `--jal-load-nop` added to @CompileFlags: retail keeps the hazard nop */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0xb4 --offset2 0xc0 */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xbc */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xd4 */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xcc */
/* @PostProc: swap_instructions --offset1 0x130 --offset2 0x13c */
/* @PostProc: swap_instructions --offset1 0x134 --offset2 0x138 */
/* @PostProc: swap_instructions --offset1 0x140 --offset2 0x150 */
/* @PostProc: swap_instructions --offset1 0x144 --offset2 0x148 */
/* @PostProc: swap_instructions --offset1 0x184 --offset2 0x190 */
/* @PostProc: swap_instructions --offset1 0x188 --offset2 0x18c */
/* @PostProc: swap_instructions --offset1 0x194 --offset2 0x1a0 */
/* @PostProc: swap_instructions --offset1 0x198 --offset2 0x19c */
/* @PostProc: swap_instructions --offset1 0x1d0 --offset2 0x1d4 */
/* @PostProc: swap_instructions --offset1 0x1d4 --offset2 0x1d8 */
/* @PostProc: swap_instructions --offset1 0x1d8 --offset2 0x1dc */
/* @PostProc: swap_instructions --offset1 0x1dc --offset2 0x1e0 */
/* @PostProc: swap_instructions --offset1 0x1e0 --offset2 0x1e4 */
/* @PostProc: swap_instructions --offset1 0x1e4 --offset2 0x1e8 */
/* @PostProc: swap_instructions --offset1 0x1e8 --offset2 0x1ec */
/* @PostProc: swap_instructions --offset1 0x1ec --offset2 0x1f0 */
/* @PostProc: swap_instructions --offset1 0x1f0 --offset2 0x200 */
/* @PostProc: swap_instructions --offset1 0x1f4 --offset2 0x1fc */
/* @PostProc: swap_instructions --offset1 0x200 --offset2 0x210 */
/* @PostProc: swap_instructions --offset1 0x204 --offset2 0x20c */
/* @PostProc: swap_instructions --offset1 0x210 --offset2 0x214 */
/* @PostProc: swap_instructions --offset1 0x244 --offset2 0x24c */
/* @PostProc: swap_instructions --offset1 0x248 --offset2 0x250 */
/* @PostProc: swap_instructions --offset1 0x24c --offset2 0x250 */
/* @PostProc: swap_instructions --offset1 0x254 --offset2 0x25c */
/* @PostProc: swap_instructions --offset1 0x258 --offset2 0x260 */
/* @PostProc: swap_instructions --offset1 0x25c --offset2 0x264 */
/* @PostProc: swap_instructions --offset1 0x264 --offset2 0x268 */
/* @PostProc: patch_insn_reg_field 0x1a8:rd:v1:v0 0x1ac:rt:v0:v1 0x1b0:rs:v0:v1 0x1b0:rt:v0:v1 0x1b4:rs:v1:v0 0x1b4:rt:v0:v1 */
#include "types.h"

typedef struct {
    s32 data[8];
} Block;

extern s32 *DSTACK[100];
extern s32 GsWSMATRIX;

extern void GsMulCoord3(s32 *, s32 *);
extern void GsMulCoord2(s32 *, s32 *);

void GsGetLws(s32 *arg0, Block *output, Block *dest) {
    s32 counter;
    s32 final_idx;
    s32 *next;
    s32 **arr;
    s32 **p2;

    counter = 0;
    final_idx = 100;
    arr = DSTACK;

    do {
        *arr = arg0;
        next = (s32 *)arg0[18];
        if (next == 0) {
            if (arg0[0] == 0) {
                final_idx = counter;
            }
            goto loop_exit;
        }
        if (arg0[0] == 0) {
            final_idx = counter;
        }
        arr++;
        counter++;
        arg0 = next;
    } while (1);
loop_exit:

    if (counter == 0) {
        *output = *(Block *)(DSTACK[0] + 1);
    } else if (final_idx == 100) {
        counter = 0;
        *output = *(Block *)(DSTACK[0] + 9);
    } else if (final_idx == counter) {
        *output = *(Block *)(DSTACK[counter] + 1);
    } else {
        counter = final_idx + 1;
        *output = *(Block *)(DSTACK[final_idx + 1] + 9);
    }

    if (counter > 0) {
        p2 = &DSTACK[counter - 1];
        do {
            GsMulCoord3((s32 *)output, *p2 + 1);
            *(Block *)(*p2 + 9) = *output;
            (*p2)[0] = 1;
            counter--;
            p2--;
        } while (counter > 0);
    }

    *dest = *output;
    GsMulCoord2(&GsWSMATRIX, (s32 *)dest);
}
