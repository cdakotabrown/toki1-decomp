/* @RodataBase: 0x8013BE18 */
/* @CompileEnv: IDO52_COPYPROP_VAR=-5 */
/* master_girl_serifu_talk_step - MASTER overlay, Address: 0x80135CF4, Size: 708 bytes (177 insns) */
/* @SymbolSize: 708 */
/* @PostProc: patch_insn_reg_field 0x1c0:rt:v0:v1 0x1c8:rs:v0:v1 0x210:rs:v0:v1 */
#include "types.h"

typedef struct {
    s16 v[13];
} Table26;

extern Table26 master_talk17_tbl;
extern u8 now_sub_sub_step;
extern u8 master_talk_girl_no;
extern u8 now_girl;
extern u16 basyo_pack;
extern s32 girl11_flag;
extern s16 now_serifu;
extern s16 now_serifu_in;

extern void bustup_ani(void);
extern s32 get_CD_State2(void);
extern void date_name_set(s32);
extern void bustup_ptr_init(void);
extern void zyotai_sprite_set_chk(void);
extern void bustup_face_change(s32);
extern void sprite_fade_main(void);
extern void device_cmd_send(s32);
extern void don_init(void);
extern void device_port_init(s32, s32);
extern void sub_sub_step_up(void);
extern void serifu_run(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void sub_step_up(void);
extern void game_reset(void);
extern void common_disp(void);

void master_girl_serifu_talk_step(void) {
    s32 state;
    u8 v1;
    Table26 buf;

    buf = master_talk17_tbl;
    bustup_ani();
    state = now_sub_sub_step;

    switch (state) {
    case 0:
        if (get_CD_State2() & 1) {
            date_name_set((s32)master_talk_girl_no);
            v1 = master_talk_girl_no;
            now_serifu = buf.v[v1];
            if (v1 == 4 && ((u32)basyo_pack >> 12) == 6) {
                now_serifu = 60;
            }
            now_serifu_in = 0;
            bustup_ptr_init();
            now_girl = master_talk_girl_no;
            zyotai_sprite_set_chk();
            if (master_talk_girl_no == 12) {
                bustup_face_change(0);
            }
        }
        break;
    case 1:
        sprite_fade_main();
        break;
    case 2:
        if (master_talk_girl_no == 11) {
            device_cmd_send(1280);
            don_init();
        }
        device_port_init(0, 0);
        sub_sub_step_up();
        break;
    case 3:
        serifu_run();
        break;
    case 4:
        fade_up_main();
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        v1 = master_talk_girl_no;
        if (v1 == 11) {
            if (((u32)(girl11_flag << 20) >> 29) == 0) {
                now_serifu = 30;
            } else {
                now_serifu = 31;
            }
            fade_down_main();
        } else if (v1 == 12) {
            now_serifu = 32;
            sub_sub_step_up();
        } else {
            now_serifu = 29;
            sub_sub_step_up();
        }
        break;
    case 7:
        serifu_run();
        break;
    case 8:
        if (master_talk_girl_no == 11) {
            sub_sub_step_up();
        } else {
            fade_down_main();
        }
        break;
    case 9:
        sprite_fade_main();
        break;
    case 10:
        device_port_init(1, 0);
        sub_step_up();
        break;
    default:
        game_reset();
        break;
    }

    common_disp();
}
