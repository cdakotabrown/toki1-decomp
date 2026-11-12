/* Function: get_top_girl */
/* Address: 0x8008669C */
/* Size: 300 bytes (75 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: remap_stack_slots --map 100:92,104:96 */
/* @PostProc: strip_trailing_instructions --nop-only --count 1 */
/* @PostProc: custom_script custom_func_8008669C.py */
#include "types.h"

extern s32 screen_w[];

extern s32 get_h_yuukou(s32);
extern s32 get_h_tokimeki(s32);
extern u8 get_g_zyotai_low(s32);

s32 get_top_girl(void) {
    s32 pad0;
    s32 pad1;
    s32 slots[11];
    s32 *ptr;
    u8 slotCount;
    s32 i;
    s32 bestSlot;
    s32 bestScore;
    s32 curScore;
    u8 randVal;
    s32 *slotPtr;
    u8 val1;
    u8 val2;

    (void)&pad0;
    (void)&pad1;

    slotCount = 0;
    ptr = (s32 *)((char *)screen_w + 0);
    i = 0;

    do {
        s32 val = *(s32 *)((char *)ptr + 456);
        if (((u32)(val << 30) >> 31) == 1U) {
            slots[slotCount] = i;
            slotCount = (u8)(slotCount + 1);
        }
        i++;
        ptr = (s32 *)((char *)ptr + 56);
    } while (i < 11);

    randVal = get_h_yuukou(0);
    bestScore = get_h_tokimeki(0) + randVal;
    bestSlot = 0;
    i = 1;

    if ((u8)slotCount >= 2U) {
        slotPtr = &slots[1];
        do {
            randVal = get_h_tokimeki(*slotPtr);
            curScore = get_h_yuukou(*slotPtr) + randVal;

            if (bestScore < curScore) {
                val1 = get_g_zyotai_low(bestSlot);
                val2 = get_g_zyotai_low(*slotPtr);
                if (!((u32)val1 < (u32)val2)) {
                    bestSlot = *slotPtr;
                    bestScore = curScore;
                }
            }
            i++;
            slotPtr++;
        } while ((u32)i < (u32)slotCount);
    }

    return bestSlot;
}
