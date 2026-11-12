/* @SymbolStart: 0x801393AC */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x24:rs:v0:v1 0x30:rs:v0:v1 0x3c:rs:v0:v1 */
/* @SymbolSize: 152 */
#include "types.h"

extern s32 serifu_no;

extern void strcpy(void *dst, void *src);
extern void sub_sub_step_up(void);

extern u8 shugaku_fade_skip;
extern u8 shu_name;
extern u8 shugaku_shu_name_set2_str0;
extern u8 shugaku_shu_name_set2_str1;
extern u8 shugaku_shu_name_set2_str2;

void shugaku_shu_name_set2(void)
{
    s32 sel = serifu_no;

    shugaku_fade_skip = 0;
    switch (sel) {
    case 0:
        strcpy(&shu_name, &shugaku_shu_name_set2_str0);
        break;
    case 1:
        strcpy(&shu_name, &shugaku_shu_name_set2_str1);
        break;
    case 2:
        strcpy(&shu_name, &shugaku_shu_name_set2_str2);
        break;
    }
    sub_sub_step_up();
}
