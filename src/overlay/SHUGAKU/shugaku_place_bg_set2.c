/* @SymbolStart: 0x80139060 */
/* @SymbolSize: 180 */
/* @PostProc: patch_insn_reg_field 0x0:rt:v0:a1 0x4:rs:v0:a1 0x4:rt:v0:a1 0xc:rs:v0:a1 0x18:rs:v0:a1 */
#include "types.h"

extern void dec_bg_cache_step(s32 a0);
extern void strcpy(void *, void *);
extern void event_log_add(s32 a0, s32 a1);
extern void sub_sub_step_up(void);

extern u8 shugaku_place;

extern s32 ba_string;
extern s32 shugaku_place_bg_set2_str0;
extern s32 shugaku_place_bg_set2_str1;
extern s32 shugaku_place_bg_set2_str2;

void shugaku_place_bg_set2(void)
{
    switch (shugaku_place) {
    case 0:
        dec_bg_cache_step(16990);
        strcpy(&ba_string, &shugaku_place_bg_set2_str0);
        break;
    case 1:
        dec_bg_cache_step(16971);
        strcpy(&ba_string, &shugaku_place_bg_set2_str1);
        break;
    default:
        dec_bg_cache_step(17009);
        strcpy(&ba_string, &shugaku_place_bg_set2_str2);
        break;
    }
    event_log_add(3, (s32)shugaku_place);
    sub_sub_step_up();
}
