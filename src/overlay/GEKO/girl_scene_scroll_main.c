/* @PostProc: patch_stack_complete --old 320 --new 336 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* @PostProc: patch_insn_reg_field 0x110:rt:t1:t2 0x114:rt:t1:t2 */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=8 */
/* girl_scene_scroll_main -- GEKO overlay, Address: 0x801351B4, Size: 328 bytes (82 insns) */
#include "types.h"

typedef void (*GekoFn)(s32);

typedef struct {
    GekoFn fn[73];
} GekoFnTable;

extern s32 dec_bg_show(s32);
extern s32 parameter_show(void);
extern s32 update_anim_state(s32);
extern s32 step_directional_scroll_once(void);
extern s32 dir_scroll_main(void);

extern GekoFnTable girl_scene_scroll_main_tbl;

extern s32 pad_data;
extern u8 now_sub_sub_step;
extern volatile u8 cursor_cw_disp;
extern s32 date_sprite_on;
extern s32 script_on;
extern s32 menu_on;
extern u8 geko_dir_scroll_flag;

void girl_scene_scroll_main(void)
{
    GekoFnTable buf;
    u8 v;

    buf = girl_scene_scroll_main_tbl;
    buf.fn[now_sub_sub_step](128);

    if (geko_dir_scroll_flag != 0) {
        update_anim_state(2);
        dir_scroll_main();
        return;
    }

    update_anim_state(1);
    dec_bg_show(0);
    parameter_show();

    if (script_on == 0 && date_sprite_on == 0 &&
        ((pad_data & 0xF000F000) != 0 || menu_on == 1)) {
        v = cursor_cw_disp | 0x80;
        cursor_cw_disp = v;
    }

    v = cursor_cw_disp;
    if (date_sprite_on != 0) {
        v &= 0x7F;
    }
    cursor_cw_disp = v;
    if ((v & 0x80) != 0) {
        step_directional_scroll_once();
    }
}
