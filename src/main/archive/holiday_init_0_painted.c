/* Function: holiday_init_0 */
/* Address: 0x80085098 */
/* Size: 580 bytes (145 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: custom_script postproc_80085098.py */
#include "types.h"

extern s32 date_sprite_on;
extern s32 wink2_stop;
extern s32 bustup_mode;
extern s32 etc_rec_tbl0;
extern s32 etc_rec_tbl1;
extern s32 etc_rec_tbl2;
extern s32 etc_rec_tbl3;
extern s32 etc_rec_tbl4;
extern s32 etc_rec_tbl5;

typedef struct {
      u32 field_0;
      s16 field_4;
      s16 field_6;
      s16 field_8;
      s16 field_A;
      s16 field_C;
      u8  field_E;
      u8  field_F;
      s16 field_10;
      s16 field_12;
      u8  field_14;
      u8  field_15;
      u8  field_16;
      u8  pad_17;
      s16 field_18;
      s16 field_1A;
      s16 field_1C;
      s16 field_1E;
      s32 field_20;
} HolidayStruct;

extern HolidayStruct Sprites[64];
extern u8 D_800EE740[];

extern void func_80044568(s32, s32, s32, s32, s32);

void holiday_init_0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s16 i;
    s32 c128;

    date_sprite_on = 0;
    wink2_stop = 0;
    bustup_mode = 0;

    func_80044568(320, 128, 64, 128, arg0);
    func_80044568(384, 128, 64, 128, arg1);
    func_80044568(448, 128, 64, 128, arg2);
    func_80044568(512, 128, 64, 128, arg3);
    func_80044568(576, 128, 64, 128, arg4);
    func_80044568(640, 128, 64, 128, arg5);

    etc_rec_tbl0 = 1;
    etc_rec_tbl1 = 1;
    etc_rec_tbl2 = 1;
    etc_rec_tbl3 = 1;
    etc_rec_tbl4 = 1;
    etc_rec_tbl5 = 1;

    c128 = 128;
    i = 58;
    do {
        HolidayStruct *ptr;
        s32 idx;

        ptr = &Sprites[i];
        ptr->field_C = i - 53;
        idx = i - 58;
        ptr->field_0 = 0xC1000000;
        ptr->field_8 = c128;
        ptr->field_A = c128;
        ptr->field_E = 0;
        ptr->field_F = c128;
        ptr->field_10 = 0;
        ptr->field_12 = 497;
        ptr->field_14 = 0;
        ptr->field_15 = 0;
        ptr->field_16 = 0;
        ptr->field_18 = 0;
        ptr->field_1A = 0;
        ptr->field_1C = 4096;
        ptr->field_1E = 4096;
        ptr->field_20 = 0;
        ptr->field_4 = (s16)((idx % 3) * 128 - 160);
        ptr->field_6 = (s16)((idx / 3) * 128 - 120);
        D_800EE740[i + 0x10A5] = 11;
        i = (s16)(i + 1);
    } while (i < 64);

    Sprites[60].field_8 = 64;
    Sprites[62].field_A = 32;
    Sprites[61].field_A = 32;
    Sprites[63].field_8 = 64;
    Sprites[63].field_A = 32;
}
