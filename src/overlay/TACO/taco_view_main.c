/* @SymbolStart: 0x80143D7C */
/* @SymbolSize: 172 */
/* Address: 0x80143D84 */
/* Size: 164 bytes (41 insns) */
#include "types.h"

extern s32 taco_view_flag;

extern void sd_wait_down(void);
extern void taco_cd_load_step(void);
extern void taco_view_pad_move(void);
extern void taco_view_angle_step(void);
extern void taco_ref_view_set(void);
extern void taco_grid_disp(void);
extern void taco_mesh_disp(void);
extern void taco_view_nop_hook(void);

void taco_view_main(void)
{
    s32 arg0 = taco_view_flag;

    if (arg0 != 0) {
        taco_ref_view_set();
        taco_view_pad_move();
        taco_view_angle_step();
        taco_cd_load_step();
        sd_wait_down();
    }

    if (taco_view_flag & 0x1) {
        taco_grid_disp();
    }

    if (taco_view_flag & 0x2) {
        taco_mesh_disp();
    }

    if (taco_view_flag & 0x4) {
        taco_view_nop_hook();
    }
}
