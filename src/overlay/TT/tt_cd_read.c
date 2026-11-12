/* @SymbolSize: 120 */
/* tt_cd_read -- TT overlay, Address: 0x8014303C */
#include "types.h"

extern u8 tt_read_cnt;
extern u8 tt_read_prev_cnt;

extern s32 cd_command_issue(s32, s32, s32);
extern s32 get_reads(void);
extern s32 cd_read_start(s32, s32, s32);

void tt_cd_read(s32 a0, s32 a1, s32 a2)
{
    s32 v;
    s32 saved_a0, saved_a1, saved_a2;
    s32 shifted;

    saved_a0 = a0;
    saved_a1 = a1;
    saved_a2 = a2;
    tt_read_prev_cnt = tt_read_cnt;
    v = get_reads();
    tt_read_cnt = (u8)v;
    cd_command_issue(9, 0, 0);
    shifted = (saved_a2 + 2047) / 2048;
    cd_read_start(shifted, saved_a0, saved_a1);
}
