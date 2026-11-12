/* Function: func_80067490 */
/* Address: 0x80067490 */
/* Size: 44 bytes (11 instructions) */
/* Status: ASM_FINAL (hand-edited assembly — will not match from C) */
/* Region: Game (< 0x80086000) */
/* @PostProc: patch_stack_complete --old 24 --new 40 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern void load_image_rect(s32, s32, s32, s32);

void func_80067490(void) {
    load_image_rect(160, 481, 16, 1);
}
