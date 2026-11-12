/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
#include "types.h"

extern u32 pad_trig;
extern volatile u8 name_ent_cell_no;

extern void step_directional_scroll_once();
extern void name_ent_hizuke_cell_set();
extern void name_ent_hizuke_disp();
extern void name_ent_hizuke_button_step();
extern void name_ent_hizuke_rand();
extern void sub_sub_step_up();
extern void name_ent_hizuke_grid();

void name_ent_hizuke_input(void)
{
    u32 input;
    u32 circle;
    u32 cross;
    u32 l1;
    u8 subState;

    step_directional_scroll_once();
    name_ent_hizuke_cell_set();
    name_ent_hizuke_disp(0);
    name_ent_hizuke_button_step(0);

    input = pad_trig;
    circle = input & 0x0040;
    cross = input & 0x0020;
    l1 = input & 0x0800;

    if (circle) {
        subState = name_ent_cell_no;
        if (subState != 0) {
            name_ent_cell_no = subState - 1;
        }
        goto end;
    }

    if (cross) {
        subState = name_ent_cell_no;
        name_ent_cell_no = subState + 1;
        name_ent_hizuke_disp(0);

        subState = name_ent_cell_no;
        if ((u32)subState >= 3) {
            name_ent_hizuke_rand(0);
            sub_sub_step_up();
        }
        goto end;
    }

    if (l1) {
        name_ent_hizuke_rand(0);
        sub_sub_step_up();
    }

end:

    name_ent_hizuke_grid();
}
