/* @SymbolStart: 0x8014A30C */
/* @SymbolSize: 320 */
/* date_bustup_check -- DATE overlay, Address: 0x8014A314, 312 bytes (78 insns) */
#include "types.h"

extern s32 *bustup_check_init_tbl0;
extern s32 *bustup_check_init_tbl1;
extern s32 *bustup_check_init_tbl2;
extern s32 bustup_check_init_tbl3;
extern s16 date_open_step;
extern u8 now_girl;

extern s32 sub_sub_step_up(void);
extern s32 get_g_zyotai_h(s32);
extern void bustup_sprite_init(s32, s32 *, s32 *, s32 *, s32);
extern void hizuke_disp_switch(s32);

s32 date_bustup_check(void)
{
    s32 *p;
    s32 v1;
    u8 masked;

    p = bustup_check_init_tbl0;
    if (p[0] != (s32)0x801820AC || p[1] != (s32)0x801860AC ||
        p[2] != (s32)0x8018A0AC || p[3] != (s32)0x8018E0AC ||
        p[4] != (s32)0x801920AC || bustup_check_init_tbl2[0] != (s32)0x8018005C ||
        bustup_check_init_tbl2[1] != (s32)0x80180070 ||
        bustup_check_init_tbl1[0] != (s32)0x80180000) {
        date_open_step = date_open_step - 2;
        return 0;
    }
    v1 = get_g_zyotai_h(now_girl);
    masked = (u8)(v1 & 0x7F);
    v1 = masked;
    if ((u32)now_girl >= 12U) {
        v1 = 0;
    }
    bustup_sprite_init(bustup_check_init_tbl3, bustup_check_init_tbl0, bustup_check_init_tbl1, bustup_check_init_tbl2, v1);
    hizuke_disp_switch(1);
    return sub_sub_step_up();
}
