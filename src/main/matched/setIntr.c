/* Function: setIntr -- libetc: enable/disable one of eleven slots. */
/* Address: 0x800B3E64 */
/* Size: 300 bytes (75 instructions) */
/* Region: Library (0x80086000+), Psy-Q GCC 2.6.0 */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -fno-schedule-insns -fno-schedule-insns2 -fno-rerun-cse-after-loop --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x70 */
/* @PostProc: swap_instructions --offset1 0x118 --offset2 0x11c */
/* @PostProc: remap_stack_slots --map 20:24,24:20 */
/* @PostProc: swap_registers s1:s2 --bidirectional */
#include "types.h"

typedef struct {
    u16 enable;
    u16 unk2;
    s32 tbl[11];
    s32 mask;
} SlotTable;

extern SlotTable libapi_intr_env;
extern volatile u16 *_mask;
extern s32 libapi_intr_callback_mask;

#define ENABLE_WORD (libapi_intr_env.enable)
#define MASK_WORD   (libapi_intr_env.mask)
#define libapi_intr_callback_tbl  (libapi_intr_env.tbl)

extern void ChangeClearRCnt(s32 ch, s32 off);

s32 setIntr(s32 idx, s32 val)
{
    s32 old;
    s32 bit;
    s32 flags;
    u16 t;

    old = libapi_intr_callback_tbl[idx];
    if (val == old) {
        return old;
    }
    if (ENABLE_WORD == 0) {
        return old;
    }
    t = *_mask;
    *_mask = 0;
    flags = t;
    if (val != 0) {
        libapi_intr_callback_tbl[idx] = val;
        flags |= 1 << idx;
        *(volatile s32 *)&MASK_WORD = MASK_WORD | (1 << idx);
    } else {
        bit = 1 << idx;
        libapi_intr_callback_tbl[idx] = 0;
        flags &= ~bit;
        libapi_intr_callback_mask &= ~bit;
    }
    if (idx == 4) {
        ChangeClearRCnt(0, val == 0);
    }
    if (idx == 5) {
        ChangeClearRCnt(1, val == 0);
    }
    if (idx == 6) {
        ChangeClearRCnt(2, val == 0);
    }
    *_mask = flags;
    return old;
}
