/* @SymbolStart: 0x80138230 */
/* @SymbolSize: 144 */
#include "types.h"

extern u8 shugaku_place;

extern void strcpy(void *dst, void *src);
extern void sub_sub_step_up(void);

extern u8 shu_name;
extern u8 shugaku_shu_name_set_str0;
extern u8 shugaku_shu_name_set_str1;
extern u8 shugaku_shu_name_set_str2;

void shugaku_shu_name_set(void)
{
    s32 sel = shugaku_place;

    switch (sel) {
    case 0:
        strcpy(&shu_name, &shugaku_shu_name_set_str0);
        break;
    case 1:
        strcpy(&shu_name, &shugaku_shu_name_set_str1);
        break;
    case 2:
        strcpy(&shu_name, &shugaku_shu_name_set_str2);
        break;
    }
    sub_sub_step_up();
}
