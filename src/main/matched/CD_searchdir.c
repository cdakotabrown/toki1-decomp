/* Function: CD_searchdir */
/* Address: 0x8008954C */
/* Size: 164 bytes (41 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 --fill-epilogue --load-dest-temp */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x04 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x08 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x28 */
/* The function was closed by `@PostProc: custom_script CD_searchdir.py` */
#include "types.h"

extern s32 strcmp(void *a0, void *a1);

extern u32 libcd_dir_ent_parent_view[];
extern u8 libcd_dir_ent_name_view[];

s32 CD_searchdir(u32 match1, void *match2) {
    s32 i = 0;
    u8 *search_ptr = libcd_dir_ent_name_view;
    s32 offset = 0;

    do {
        u32 entry = *(u32 *)((u8 *)libcd_dir_ent_parent_view + offset);

        if (entry == 0) {
            return -1;
        }

        if (entry == match1) {
            if (strcmp(match2, search_ptr) == 0) {
                return i + 1;
            }
        }

        search_ptr += 44;
        i++;
        offset += 44;

    } while (i < 128);

    return -1;
}
