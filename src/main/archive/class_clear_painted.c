/* Function: class_clear */
/* Address: 0x8004905C */
/* Size: 168 bytes (42 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 24 --new 40 */
/* @PostProc: custom_script custom_func_8004905C.py */
#include "types.h"

extern u8 class_w[];
extern u8 srn_w;
extern s32 class_clear_str0;

extern void printf(s32 *, s32);

void class_clear(s32 arg0) {
    u8 *ptr;
    u8 *limit;
    u8 *end;

    printf(&class_clear_str0, arg0);

    if (arg0 != 0) {
        if (arg0 > 0) {
            limit = class_w + arg0 * 68;
            end = &srn_w;
            ptr = class_w + 68;
            while (1) {
                *(ptr - 65) = 0;
                if (ptr >= limit) {
                    return;
                }
                ptr += 68;
                if (ptr == end) break;
            }
        }
    } else {
        ptr = class_w;
        end = &srn_w;
        do {
            ptr += 272;
            *(ptr - 201) = 0;
            *(ptr - 133) = 0;
            *(ptr - 65) = 0;
            *(ptr - 269) = 0;
        } while (ptr != end);
    }
}
