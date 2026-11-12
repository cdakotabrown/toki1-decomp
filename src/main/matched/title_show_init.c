/* Function: title_show_init */
/* Address: 0x80062424 */
/* Size: 468 bytes (117 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 120 --new 128 */
/* @PostProc: scheduler_normalizer */
#include "types.h"

extern s32 work_fuku;
extern u8 back_clear_g;
extern u8 back_clear_r;
extern u8 back_clear_b;
extern u8 screen_bri;
extern s16 cw97_y;
extern s16 cw98_y;
extern s16 cw97_x;
extern s16 cw98_x;
extern u8 class_w[];

extern void menu_set(s32, s32, s16*, s16*, s16*, s16*);
extern void prim_init(void);
extern void cursor_init(s32);
extern void set_flag_b3_high_bit(s32);
extern void cursor_select(void);
extern void class_clear_n(s32);
extern void sub_sub_step_up(void);

void title_show_init(void) {
    s16 arr2[2];
    s16 arr1[2];
    s16 arr3[2];
    s16 arr4[2];
    s32 s1;
    s32 s2;
    u8 *s0;
    u8 *p797;
    u8 *p796;
    u8 *p798;

    arr1[0] = 66;
    arr2[0] = -48;
    arr3[0] = 160;
    arr4[0] = 20;
    arr1[1] = 82;
    arr2[1] = -48;
    arr3[1] = 160;
    arr4[1] = 20;

    menu_set(0, 2, arr2, arr1, arr3, arr4);

    work_fuku = 0;
    p797 = &back_clear_g;
    p796 = &back_clear_r;
    p798 = &back_clear_b;
    *p796 = *p797 = *p798 = 255;
    prim_init();
    screen_bri = 128;
    cursor_init(0);
    set_flag_b3_high_bit(1);
    cursor_select();

    s0 = class_w;
    s1 = 0;
    s2 = 0;
    do {
        class_clear_n(s1 + 96);
        *(s16 *)(s0 + 6552) = (s16)s2;
        s1++;
        s2 += 2;
        s0 += 68;
        s0[6461] = 13;
        s0[6462] = 64;
        *(s32 *)(s0 + 6516) = 0x01000000;
        s0[6463] = 164;
        *(s32 *)(s0 + 6472) = 0x801EAB3C;
        *(s32 *)(s0 + 6476) = 0x801EAB6C;
        *(s32 *)(s0 + 6512) = 0x801EAB20;
        s0[6464] = 8;
        s0[6465] = 128;
        *(s16 *)(s0 + 6498) = -160;
        *(s16 *)(s0 + 6502) = -120;
    } while (s1 < 3);

    cw97_y = -110;
    cw98_y = -120;
    cw97_x = -170;
    cw98_x = -170;

    sub_sub_step_up();
}
