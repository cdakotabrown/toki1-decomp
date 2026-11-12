/* @CompileEnv: IDO52_UGEN_TEMP_REG=16:t0,6:t2,8:t4,9:t5,7:t3,17:t1,5:t1 */
/* Address: 0x8013B278 (true symbol start 0x8013B270, cross-prologue). */
/* @SymbolStart: 0x8013B270 */
/* @SymbolSize: 248 */
/* @PostProc: patch_stack_complete --old 40 --new 64 */
/* @PostProc: remap_stack_slots --map '32:48,28:56' */
/* @PostProc: patch_insn_reg_field 0xc0:rt:t0:v0 0xc4:rt:t0:v0 0xcc:rt:t1:v0 */
#include "types.h"

extern void taco_screen_capture();
extern void taco_obj_clear();
extern void all_object_disp_off();
extern void taco_objects_init();
extern void device_port_init2();
extern void sub_sub_step_up();

typedef struct { u8 b[8]; } Blk8;

extern u8 taco_load_no;
extern u8 *taco_actor_rec;
extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;
extern s32 taco_view_flag;

void taco_game_init(void)
{
    Blk8 save;
    s32 saveb;

    if (taco_load_no == 0) {
        taco_screen_capture();
        taco_obj_clear();
        all_object_disp_off();
        save = *(Blk8 *)(taco_actor_rec + 124);
        saveb = taco_actor_rec[133];
        taco_objects_init();
        *(Blk8 *)(taco_actor_rec + 124) = save;
        taco_actor_rec[133] = saveb;
        back_clear_b = 16;
        back_clear_g = 16;
        back_clear_r = 16;
        taco_view_flag = 0;
        taco_load_no = 68;
        device_port_init2(0, 514);
        sub_sub_step_up();
    }
}
