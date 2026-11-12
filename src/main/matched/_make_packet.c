/* Function: _make_packet  (name inherited from the old signature scan and */
/* Address: 0x8009E0A0 */
/* Size: 140 bytes (35 instructions) + one trailing alignment nop */
/* Region: Library (0x80086000-0x800BFFFF) */
/* Compiler: Psy-Q GCC 2.6.0  (was marked gcc281 -- WRONG, see below) */
/* COMPILER CORRECTION: the header said `@Compiler: gcc281` with */
/* `@PostProc: strip_leading_instructions --count 19`.  Both are wrong.  GCC */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 -fno-schedule-insns -fno-schedule-insns2 --fill-earliest --fill-load-delay */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_registers s0:s1 --bidirectional */
/* @PostProc: remap_stack_slots --keep-frame --map '16:20,20:16' */
/* @PostProc: swap_instructions 0x4 0xc */
/* @PostProc: swap_instructions 0x8 0x1c */
/* @PostProc: swap_instructions 0xc 0x10 */
/* @PostProc: swap_instructions 0x10 0x1c */
/* @PostProc: swap_instructions 0x14 0x1c */
/* @PostProc: swap_instructions 0x18 0x1c */
/* @PostProc: swap_instructions 0x1c 0x20 */
/* @PostProc: swap_instructions 0x20 0x24 */
/* @PostProc: swap_instructions 0x6c 0x70 */
/* @PostProc: swap_instructions 0x74 0x7c */
/* @PostProc: swap_instructions 0x78 0x80 */
/* @PostProc: swap_instructions 0x7c 0x80 */
#include "types.h"

extern void printf(char *);
extern char _make_packet_tbl0[];

typedef struct {
    s32 field_0;
    s32 *field_4;
    s32 field_8;
} AutoCtrl;

s32 _make_packet(s32 *node, AutoCtrl *ctrl, s32 panval, s32 pan)
{
    s32 idx;
    s32 *entry;
    s32 prev;
    s32 pmask;

    idx = (s32)(u16)panval - ctrl->field_8;
    if (idx < 0) {
        printf(_make_packet_tbl0);
    }
    entry = ctrl->field_4 + idx;
    prev = *entry;
    pmask = pan & 0xFF;
    node[0] = prev;
    *(u8 *)((s32)node + 3) = (u8)pan;
    *entry = (s32)node;
    *(u8 *)((s32)entry + 3) = 0;
    return (s32)&node[pmask + 1];
}
