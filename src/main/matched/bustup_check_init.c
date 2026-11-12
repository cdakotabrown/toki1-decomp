/* Function: bustup_check_init */
/* Address: 0x800838B0 */
/* Size: 352 bytes (88 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x800838A8 */
/* @SymbolSize: 360 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_instructions --offset1 0xf8 --offset2 0x100 */
/* @PostProc: swap_instructions --offset1 0xfc --offset2 0x108 */
/* @PostProc: swap_instructions --offset1 0x104 --offset2 0x108 */
/* @PostProc: swap_instructions --offset1 0x10c --offset2 0x114 */
/* @PostProc: swap_instructions --offset1 0x110 --offset2 0x118 */
/* @PostProc: swap_instructions --offset1 0x114 --offset2 0x11c */
/* @PostProc: swap_instructions --offset1 0x118 --offset2 0x11c */
/* @PostProc: swap_instructions --offset1 0x134 --offset2 0x13c */
/* @PostProc: swap_instructions --offset1 0x138 --offset2 0x140 */
/* @PostProc: swap_instructions --offset1 0x13c --offset2 0x144 */
/* @PostProc: swap_instructions --offset1 0x140 --offset2 0x144 */
/* @PostProc: strip_trailing_instructions --count 2 --nop-only */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=11:t8,12:t9,13:t0,14:t6,15:t1,16:t2,17:t3,18:t4,19:t5 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 now_sub_sub_step;
extern u8 now_girl;
extern s32 bustup_check_init_tbl3;
extern u32 *bustup_check_init_tbl0;
extern s32 bustup_check_init_tbl1;
extern s32 bustup_check_init_tbl2;
extern u8 bustup2_cw_bright;
extern u32 bustup2_cw_attr;
extern u32 bustup_cw_attr;
extern u32 Sprites;
extern u32 sprite1_attribute;

extern s32 block_checksum_check(s32 a0, u32 a1);
extern s32 sub_sub_step_up(void);
extern s32 get_g_zyotai_h(u8 a0);
extern void bustup_sprite_init(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);

s32 bustup_check_init(void) {
    s32 result;
    u32 *ptr = bustup_check_init_tbl0;

    if (ptr[0] != 0x801820ACU ||
        ptr[1] != 0x801860ACU ||
        ptr[2] != 0x8018A0ACU ||
        ptr[3] != 0x8018E0ACU ||
        ptr[4] != 0x801920ACU) {
        now_sub_sub_step -= 4;
        return 0;
    }

    if (block_checksum_check(45, 0x80180000) == 0) {
        sub_sub_step_up();
        now_sub_sub_step -= 4;
        return 0;
    }

    result = get_g_zyotai_h(now_girl);
    bustup_sprite_init(bustup_check_init_tbl3, (s32)bustup_check_init_tbl0, bustup_check_init_tbl1, bustup_check_init_tbl2, result & 0x7F);

    bustup2_cw_bright = 0;
    bustup2_cw_attr |= (u32)0x81000000;
    bustup_cw_attr |= (u32)0x81000000;
    Sprites |= (u32)0x81000000;
    sprite1_attribute |= (u32)0x81000000;

    return sub_sub_step_up();
}
