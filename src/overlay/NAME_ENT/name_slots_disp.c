/* @SymbolStart: 0x8013AE34 */
/* @SymbolSize: 724 */
/* Address: 0x8013AE3C (true start 0x8013AE34, cross-prologue head) */
/* Size: 724 bytes (181 insns) including the 2-word hoisted head. */
#include "types.h"

extern u8 card_sys_present;
extern u8 last_album;
extern u8 album_work[];
extern u8 album_slot;
extern u8 album_chr[];

extern void name_num_disp(s32, s32, s32);

void name_slots_disp(void)
{
    s32 stride;

    if (card_sys_present != 0) {
        name_num_disp(*(s16 *)(album_work + last_album * 4 + 2698), 16, 30);
        name_num_disp(*(s16 *)(album_work + last_album * 4 + 2762), 16, 46);
        name_num_disp(*(s16 *)(album_work + last_album * 4 + 2826), 16, 62);
        name_num_disp(*(s16 *)(album_work + last_album * 4 + 2890), 16, 78);
        name_num_disp(*(s16 *)(album_work + last_album * 4 + 2954), 128, 14);
        name_num_disp(*(s16 *)(album_work + last_album * 4 + 3018), 128, 30);
        name_num_disp(*(s16 *)(album_work + last_album * 4 + 3082), 128, 46);
        name_num_disp(*(s16 *)(album_work + last_album * 4 + 3146), 128, 62);
        name_num_disp(*(s16 *)(album_work + last_album * 4 + 3210), 128, 78);
    } else {
        stride = 3840;
        name_num_disp(*(s16 *)(album_chr + album_slot * stride + 710), 16, 30);
        name_num_disp(*(s16 *)(album_chr + album_slot * stride + 714), 16, 46);
        name_num_disp(*(s16 *)(album_chr + album_slot * stride + 718), 16, 62);
        name_num_disp(*(s16 *)(album_chr + album_slot * stride + 722), 16, 78);
        name_num_disp(*(s16 *)(album_chr + album_slot * stride + 726), 128, 14);
        name_num_disp(*(s16 *)(album_chr + album_slot * stride + 730), 128, 30);
        name_num_disp(*(s16 *)(album_chr + album_slot * stride + 734), 128, 46);
        name_num_disp(*(s16 *)(album_chr + album_slot * stride + 738), 128, 62);
        name_num_disp(*(s16 *)(album_chr + album_slot * stride + 742), 128, 78);
    }
}
