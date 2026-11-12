/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x80133980 */
/* @SymbolSize: 372 */
#include "types.h"

extern s16 taco_time_left;
extern s8 back_clear_r;
extern s8 back_clear_g;
extern s8 back_clear_b;
extern s8 taco_load1_phase;
extern u8 step_mode;
extern u32 tpage_set_adr;
extern u32 img_set_adr;
extern u32 anim_set_adr;
extern u32 clut_set_adr;

extern s32 device_cmd_send();
extern s32 draw2d3d();
extern s32 prim_init();
extern s32 taco_view_work_init();
extern s32 back_clear_switch();
extern s32 set_flag_b3_high_bit();
extern s32 text_sys_init();
extern s32 class_clear_all();
extern s32 taco_objects_init();
extern s32 cd_read_start2();
extern s32 vram_area_clear();
extern s32 get_CD_State2();
extern s32 tpage_palette_load();
extern s32 sub_sub_step_up();

s32 taco_boot_init_load()
{
    s32 ret;

    ret = step_mode;
    switch (step_mode) {
    case 0:
        device_cmd_send(36);
        taco_time_left = 1024;
        draw2d3d(1, 0);
        back_clear_g = 0;
        back_clear_r = 0;
        back_clear_b = 0;
        prim_init();
        taco_view_work_init();
        back_clear_switch(1);
        set_flag_b3_high_bit(0);
        text_sys_init();
        class_clear_all(0);
        taco_objects_init();
        taco_load1_phase = 0;
        cd_read_start2(33, 0x80180000, 0xB4F3);
        cd_read_start2(1, 0x801FA800, 0xB514);
        ret = vram_area_clear(255);
        step_mode = step_mode + 1;
        break;
    case 1:
    case 2:
        ret = get_CD_State2();
        if (ret & 1) {
            step_mode = step_mode + 1;
        }
        break;
    case 3:
        clut_set_adr = 0x8018A000;
        img_set_adr = 0x801FAA7C;
        anim_set_adr = 0x801FAA88;
        tpage_set_adr = 0x801FAA68;
        tpage_palette_load();
        ret = sub_sub_step_up();
        break;
    }
    return ret;
}
