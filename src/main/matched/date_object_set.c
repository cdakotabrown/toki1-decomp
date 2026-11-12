/* Function: date_object_set */
/* Address: 0x800852DC */
/* Size: 352 bytes (88 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_instructions 0x060 0x064 */
/* @PostProc: swap_registers s4:s5 --only 0x05c,0x060,0x074,0x084,0x0b0 --bidirectional */
/* @PostProc: swap_registers t8:t3 --only 0x08c,0x094 */
/* @PostProc: swap_instructions 0x0a4 0x0a8 */
/* @PostProc: swap_instructions 0x0a0 0x0a4 */
/* @PostProc: swap_instructions 0x09c 0x0a0 */
/* @PostProc: swap_instructions 0x098 0x09c */
/* @PostProc: swap_registers t0:t9 --only 0x09c,0x0d8 */
/* @PostProc: swap_registers t1:t0 --only 0x0a0,0x0dc */
/* @PostProc: swap_registers t2:t1 --only 0x0a4,0x0e0 */
/* @PostProc: swap_registers t3:t2 --only 0x0a8,0x0e4 */
/* @PostProc: swap_registers v1:t8 --only 0x0ac,0x0b0 */
/* @PostProc: swap_registers t4:t5 --only 0x0fc,0x104 */
/* @PostProc: swap_registers t5:t6 --only 0x110,0x124 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 class_w;

extern u8 cal_cw_mode;
extern u8 cal_cw_disp;
extern s16 cal_cw_anim;
extern u8 date_object_set_tbl0;

extern void class_clear_n(s32);

s32 date_object_set(s32 a0arg, s32 a1arg, s32 a2arg, s16 scale) {
    s16 i;
    char *base;
    s32 offset;

    if (a0arg == 0) {
        class_clear_n(96);
        class_clear_n(97);
        return 0;
    }

    base = (char *)&class_w;
    i = 0;

    do {
        class_clear_n(i + 96);
        offset = i * 68;
        i = (s16)(i + 1);

        base[offset + 6529] = 10;
        base[offset + 6530] = 0;
        *(s32 *)(base + offset + 6584) = 0x41000000;
        base[offset + 6531] = 4;
        *(s32 *)(base + offset + 6540) = a0arg;
        *(s32 *)(base + offset + 6544) = a1arg;
        *(s32 *)(base + offset + 6580) = a2arg;
        *(s16 *)(base + offset + 6550) = 0;
        *(s16 *)(base + offset + 6552) = 0;
        base[offset + 6534] = 2;
        base[offset + 6595] = 17;
        *(s16 *)(base + offset + 6566) = -160;
        *(s16 *)(base + offset + 6570) = -120;
        base[offset + 6535] = 0;
        base[offset + 6533] = 0;
        base[offset + 6532] = 0;
        *(s16 *)(base + offset + 6548) = scale;
    } while (i < 2);

    cal_cw_mode = 4;

    if (scale >= 2) {
        date_object_set_tbl0 = 0;
        cal_cw_anim = 2;
    } else {
        cal_cw_disp = 0;
    }
}
