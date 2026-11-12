/* Function: menu_girl_taku_set */
/* Address: 0x80051FFC */
/* Size: 480 bytes (120 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: remap_stack_slots --map "116:132" --map "164:180" --map "140:156" --map "92:108" */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 set_kanji_string(s16, s16, s32, s32, s32);
extern void menu_set(s32, s32, s16*, s16*, s16*, s16*);

extern u8 screen_w[];
extern s32 date_name_set_tbl0[];

void menu_girl_taku_set(s32 arg0, s32 *arg1) {
    s32 i;
    s32 v0;
    u8 *charData;
    s16 spB4[12];
    s16 sp9C[12];
    s16 sp84[12];
    s16 sp6C[12];

    for (i = 0; i < 12; i++) {
        if (i == 11) {
            v0 = 12;
        } else {
            v0 = i;
        }

        charData = &screen_w[v0 * 0x38];

        if ((u32)((*(u32*)(charData + 456) << 30) >> 31)) {
            *arg1 = set_kanji_string(
                (s16)(((i / 3) << 6) - 128),
                (s16)(((i % 3) * 16) + 50),
                0,
                date_name_set_tbl0[v0],
                0
            );
        }

        spB4[i] = (s16)(((i / 3) << 6) - 128);
        sp9C[i] = (s16)(((i % 3) * 16) + 50);

        if ((u32)((*(u32*)(charData + 456) << 30) >> 31)) {
            sp84[i] = 0x30;
        } else {
            sp84[i] = 0;
        }
        sp6C[i] = 0x10;

        menu_set(arg0, 12, spB4, sp9C, sp84, sp6C);
    }
}
