/* Address: 0x80134510 (absorbed cross-prologue head) .. 0x801345E4 */
/* Size: 212 bytes (53 instructions) */
/* @SymbolStart: 0x80134510 */
/* @SymbolSize: 212 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 */
/* @PostProc: patch_insn_reg_field 0xb4:rt:t7:v1 0xb8:rs:t7:v1 0xb8:rt:t7:v1 0xc0:rs:t7:v1 0xc0:rt:t8:t7 0xc8:rt:t8:t7 */
#include "types.h"

extern u8 back_clear_g;
extern u8 back_clear_b;
extern u8 screen_bri;
extern u8 back_clear_r;
extern u8 vblank_flag;
extern u8 step_mode;
extern s32  goto_sub_step(s32);
extern void draw2d3d(s32, s32);
extern void initView(void);
extern void initCoordinate(void);
extern void initModelingData_init(s32);
extern void taco_light_init(void);
extern void record_work_init(void);
extern void taco_coord_init(void);
extern s32  taco_obj_link(s32);

s32 taco_view_init(void)
{
    s32 r;
    s32 sel;

    sel = step_mode;
    r = sel;
    switch (sel) {
    case 0:
        back_clear_g = 0;
        back_clear_b = 0;
        vblank_flag = 0;
        screen_bri = 128;
        back_clear_r = 0;
        draw2d3d(1, 1);
        initView();
        taco_light_init();
        initCoordinate();
        record_work_init();
        initModelingData_init(0x8018A000);
        taco_coord_init();
        r = taco_obj_link(0x801AC000);
        step_mode++;
        break;
    case 1:
        r = goto_sub_step(1);
        break;
    }
    return r;
}
