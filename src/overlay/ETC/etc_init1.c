/* Address: 0x801432A4 */
/* Size: 176 bytes (44 instructions) */
/* @SymbolSize: 176 */
#include "types.h"

extern void etc_roll_init(void);
extern void draw2d3d(s32, s32);
extern void title_init_View(void);
extern void title_init_Light(void);
extern void title_init_Coordinate(void);
extern void etc_model_init2(void);
extern void sub_step_up(void);
extern void load_header_check(s32);
extern void set_XAsector(s32, s32, s32);
extern void device_cmd_send(s32);
extern void etc_sprite_init(void);
extern void goto_sub_sub_step(s32);

extern s32 etc_roll_idx;
extern s32 etc_roll_allgirl;

void etc_init1(void)
{
    etc_roll_idx = 0;
    etc_roll_init();
    if (etc_roll_allgirl == 1) {
        draw2d3d(1, 1);
        title_init_View();
        title_init_Light();
        title_init_Coordinate();
        etc_model_init2();
        sub_step_up();
        return;
    }
    load_header_check(0);
    set_XAsector(0x07000001, 0x02000E0C, 15);
    device_cmd_send(768);
    etc_sprite_init();
    sub_step_up();
    goto_sub_sub_step(255);
}
