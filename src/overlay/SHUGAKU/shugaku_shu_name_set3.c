/* @SymbolStart: 0x8013AA8C */
/* @PostProc: patch_insn_reg_field 0xa4:rt:t3:t1 0xa8:rs:t3:t1 0xa8:rt:t3:t1 0xb4:rd:t4:t2 0xb4:rs:t3:t1 0xb4:rt:t2:t0 0xbc:rt:t4:t2 0xb0:rd:t2:t0 */
/* @SymbolSize: 192 */
#include "types.h"

extern u8 shugaku_place;

extern void strcpy(void *dst, void *src);
extern void sub_sub_step_up(void);

extern u8  shu_name;
extern u8  shugaku_shu_name_set3_str0;
extern u8  shugaku_shu_name_set3_str1;
extern u8  shugaku_shu_name_set3_str2;
extern u8  date_place;
extern u8  menu_x;
extern s16 now_serifu_shugaku;

void shugaku_shu_name_set3(void)
{
    s32 sel = shugaku_place;

    s32 t6, t9;

    switch (sel) {
    case 0:
        strcpy(&shu_name, &shugaku_shu_name_set3_str0);
        break;
    case 1:
        strcpy(&shu_name, &shugaku_shu_name_set3_str1);
        break;
    case 2:
        strcpy(&shu_name, &shugaku_shu_name_set3_str2);
        break;
    }
    t6 = (s32)menu_x;
    t9 = 1 - (s32)date_place;
    now_serifu_shugaku = (s16)(now_serifu_shugaku + (s16)(t6 * t9));
    sub_sub_step_up();
}
