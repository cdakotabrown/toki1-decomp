/* Address: 0x8004F04C */
/* Size: 144 bytes (36 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "common.h"

extern u8 get_k_speed_tbl0;
extern s32 frame_counter;
extern s16 check_end_k_tbl1;
extern s16 check_end_k_tbl0;

s32 k_disp_inc(void) {
    if ((u32)frame_counter % ((u32)get_k_speed_tbl0 + 1) == 0) {
        if (check_end_k_tbl1 < check_end_k_tbl0) {
            check_end_k_tbl1 = check_end_k_tbl1 + 1;
            return 0;
        }
        return 1;
    }

    if (check_end_k_tbl1 == check_end_k_tbl0) {
        return 1;
    }
    return 0;
}
