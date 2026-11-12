/* @SymbolStart: 0x8013815C */
/* @SymbolSize: 212 */
#include "types.h"

extern s32 serifu_no;

extern void strcpy(void *dst, void *src);
extern void sub_sub_step_up(void);

extern u8   shugaku_place;
extern u8   shu_name;
extern u8   ba_string2;
extern u8   shugaku_place_set_str0;
extern u8   shugaku_place_set_str1;
extern u8   shugaku_place_set_str2;
extern u8   shugaku_place_set_str3;
extern u8   shugaku_place_set_str4;
extern u8   shugaku_place_set_str5;

void shugaku_place_set(void)
{
    s32 sel = serifu_no;

    shugaku_place = (u8)sel;
    switch (sel) {
    case 0:
        strcpy(&shu_name, &shugaku_place_set_str0);
        strcpy(&ba_string2, &shugaku_place_set_str1);
        break;
    case 1:
        strcpy(&shu_name, &shugaku_place_set_str2);
        strcpy(&ba_string2, &shugaku_place_set_str3);
        break;
    case 2:
        strcpy(&shu_name, &shugaku_place_set_str4);
        strcpy(&ba_string2, &shugaku_place_set_str5);
        break;
    }
    sub_sub_step_up();
}
