/* @SymbolStart: 0x8013A3DC */
/* @PostProc: patch_insn_reg_field 0xa4:rt:a1:v0 0xa8:rs:a1:v0 0xa8:rt:a1:v0 0xb0:rs:a1:v0 */
/* @SymbolSize: 200 */
#include "types.h"

extern void dec_bg_cache_step(s32 a0);
extern void strcpy(s32 a0, s32 a1);
extern void event_log_add(s32 a0, s32 a1);
extern void sub_sub_step_up(void);

extern u8 shugaku_cmd_sent;
extern u8 shugaku_place;
extern u8 ba_string;
extern u8 shugaku_place_bg_set3_str0;
extern u8 shugaku_place_bg_set3_str1;
extern u8 shugaku_place_bg_set3_str2;

void shugaku_place_bg_set3(void)
{
    s32 param_v0 = shugaku_place;

    shugaku_cmd_sent = 0;
    switch (param_v0) {
        case 0:
            dec_bg_cache_step(16999);
            strcpy((s32)&ba_string, (s32)&shugaku_place_bg_set3_str0);
            break;
        case 1:
            dec_bg_cache_step(16981);
            strcpy((s32)&ba_string, (s32)&shugaku_place_bg_set3_str1);
            break;
        default:
            dec_bg_cache_step(17019);
            strcpy((s32)&ba_string, (s32)&shugaku_place_bg_set3_str2);
            break;
    }
    event_log_add(3, (shugaku_place + 3) & 0xFF);
    sub_sub_step_up();
}
