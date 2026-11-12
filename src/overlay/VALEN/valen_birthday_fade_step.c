/* @SymbolStart: 0x80133618 */
/* @PostProc: patch_insn_reg_field 0x44:rt:a0:v0 0x48:rs:a0:v0 */
/* @SymbolSize: 184 */
/* valen_birthday_fade_step - VALEN overlay, Address: 0x8013362C, Size: 184 bytes (46 insns) */
#include "types.h"

extern s32 p_birthday;
extern u8 now_day;
extern u8 now_month;
extern u8 now_girl;
extern u8 valen_top3_tbl[];
extern s32 get_h_tokimeki(s32);
extern void sub_sub_step_up(void);
extern s32 fade_down_main(void);

s32 valen_birthday_fade_step(void)
{
    u32 v0;
    s32 charId;

    v0 = p_birthday;
    if (now_day != ((v0 << 23) >> 27)) {
        goto do_default;
    }
    if (now_month != (v0 & 0xF)) {
        goto do_default;
    }
    charId = now_girl;
    if ((u32)get_h_tokimeki(charId) < 70) {
        goto do_default;
    }
    charId = now_girl;
    if (charId == valen_top3_tbl[0] || charId == valen_top3_tbl[1] || charId == valen_top3_tbl[2]) {
        if (charId != 9) {
            goto do_default;
        }
        sub_sub_step_up();
        return 0;
    }
do_default:
    return fade_down_main();
}
