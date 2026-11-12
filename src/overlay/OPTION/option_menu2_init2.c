/* @SymbolSize: 220 */
/* Address: 0x80135034, Size: 220 bytes (55 instructions) */
#include "types.h"
#include "tokimeki/class_w.h"

typedef struct {
    u8 pad0[3];
    u8 flags;
    u8 pad4[20];
    s16 field_24;
    u8 pad26[12];
    s16 field_38;
    u8 pad40[2];
    s16 field_42;
    u8 pad44[23];
    u8 field_67;
} Entry;

extern u8 option_click_cnt;
extern CLASS_W class_w[];
extern void menu_set();
extern void sub_sub_step_up(void);
extern void option_menu_box_show(void);

void option_menu2_init2(void)
{
    s32 unused0;
    s32 unused1;
    s32 i;
    s16 x[2];
    s16 y[2];
    s16 w[2];
    s16 h[2];

    for (i = 0; i < 2; i++) {
        x[i] = 0;
        y[i] = i * 16 + 40;
        w[i] = 108;
        h[i] = 16;
    }

    menu_set(1, 2, x, y, w, h);

    ((Entry *)&class_w[96].active)[20].field_24 = 121;
    ((Entry *)&class_w[96].active)[20].field_38 = 64;
    ((Entry *)&class_w[96].active)[20].field_42 = 48;
    ((Entry *)&class_w[96].active)[20].flags |= 0x80;
    ((Entry *)&class_w[96].active)[20].field_67 = 31;
    option_click_cnt = 0;
    sub_sub_step_up();
    option_menu_box_show();
}
