/* Address: 0x80135428 */
/* Size: 84 bytes (21 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern s16 mes_win_cw_y;
extern CLASS_W class_w[];

extern void sub_sub_step_up();
extern void all_object_disp_off();
extern void disp_mode_set2();
extern void ResetGraph();
extern void taco_obj_init();

void taco_select_init(void)
{
    ResetGraph(0);
    all_object_disp_off();
    disp_mode_set2(0);
    taco_obj_init();
    class_w[1].bright = 64;
    mes_win_cw_y = -120;
    sub_sub_step_up();
}
