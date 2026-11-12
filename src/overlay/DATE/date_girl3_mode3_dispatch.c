/* date_girl3_mode3_dispatch - DATE overlay, Address: 0x80152980, Size: 296 bytes (74 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

typedef struct {
    void (*fn[48])(s32);
} HandlerTable;

extern void date_light_disp(void);
extern void date_light_effect(void);

extern volatile u8 now_sub_sub_step;
extern s32 date_sprite_on;
extern u8 fade_bri;
extern HandlerTable date_girl3_mode3_dispatch_tbl;
extern CLASS_W class_w[];

void date_girl3_mode3_dispatch(void)
{
    u32 idx;
    HandlerTable table;

    idx = now_sub_sub_step;
    table = date_girl3_mode3_dispatch_tbl;

    if (idx >= 16 && idx < 19) {
        date_light_disp();
        idx = now_sub_sub_step;
    }
    if (date_sprite_on == 0 && fade_bri == 128) {
        date_light_effect();
        idx = now_sub_sub_step;
    }
    table.fn[idx](128);
    idx = now_sub_sub_step;
    if (idx >= 15 && idx < 20) {
        class_w[98].disp_flag |= 0x80;
        class_w[98].bright = fade_bri;
    }
    if (idx >= 20) {
        class_w[98].disp_flag &= ~0x80;
    }
}
