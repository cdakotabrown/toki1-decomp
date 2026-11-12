/* en_nichi_score_main - EN_NICHI overlay, Address: 0x80136420 */
/* Size: 1116 bytes (279 insns) */
/* @SymbolSize: 1116 */
/* @RodataBase: 0x801394E4 */
/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=occ:1 IDO52_AS1_XBB_VOLATILE_OK=occ:10 */
/* @PostProc: patch_insn_reg_field 0x130:rt:a2:v0 0x138:rs:a2:v0 */
#include "types.h"

extern u8 now_sub_sub_step;
extern volatile u8 step_mode;
extern volatile s32 step_counter;
extern volatile s32 pad_trig;
extern s32 en_nichi_get_cnt;
extern s32 en_nichi_rare_cnt;
extern s32 en_nichi_grid_bright;

extern void en_nichi_konami_command_set(void);
extern void en_nichi_sway(void);
extern void en_nichi_phase_count(void);
extern void en_nichi_obj(void);
extern void en_nichi_fade_show(void);
extern void en_nichi_count_set(s32, s32);
extern void en_nichi_fade_out(void);
extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);
extern void sub_step_up(void);
extern s32 get_CD_State(void);
extern void set_XAsector(s32, s32, s32);

void en_nichi_score_main(void)
{
    u32 v0;
    s32 cx;
    s32 lim;
    s32 x;
    s32 y;
    s32 sub;

    en_nichi_konami_command_set();
    en_nichi_sway();
    en_nichi_phase_count();
    en_nichi_obj();

    switch ((u32)now_sub_sub_step) {
    case 0:
        en_nichi_fade_show();
        return;

    case 1:
        if (en_nichi_get_cnt == 0 && en_nichi_rare_cnt == 0) {
            device_cmd_send(516);
        } else {
            device_cmd_send(514);
        }
        if (en_nichi_get_cnt >= 100) {
            en_nichi_get_cnt = 99;
        }
        if (en_nichi_rare_cnt >= 100) {
            en_nichi_rare_cnt = 99;
        }
        sub_sub_step_up();
        return;

    case 2:
        v0 = step_counter + 1;
        step_counter = v0;
        if (v0 % 15 == 0) {
            x = en_nichi_get_cnt;
            lim = step_mode;
            cx = lim;
            if (x < lim) {
                cx = x;
            }
            y = en_nichi_rare_cnt;
            if (y < lim) {
                lim = y;
            }
            if (cx == x && lim == y) {
                device_cmd_send(1285);
                en_nichi_count_set(en_nichi_get_cnt, en_nichi_rare_cnt);
                sub_sub_step_up();
            }
            en_nichi_count_set(cx, lim);
            if (step_mode != 0) {
                device_cmd_send(1285);
            }
            step_mode = step_mode + 1;
        }
        if ((pad_trig & 0x40) != 0) {
            en_nichi_count_set(en_nichi_get_cnt, en_nichi_rare_cnt);
            if (en_nichi_get_cnt == 0 && en_nichi_rare_cnt == 0) {
                sub_sub_step_up();
            } else {
                sub_sub_step_up();
                sub_sub_step_up();
                sub_sub_step_up();
            }
        }
        return;

    case 3:
        if (en_nichi_get_cnt != 0 || en_nichi_rare_cnt != 0) {
            goto busy;
        }
        sub = step_mode;
        switch (sub) {
        case 0:
            if (get_CD_State() == 2) {
                step_mode = step_mode + 1;
            }
            break;
        case 1:
            set_XAsector(0x01000007, 0x03000015, 14);
            device_cmd_send(768);
            step_mode = step_mode + 1;
            break;
        case 2:
            if (get_CD_State() == 4) {
                step_mode = step_mode + 1;
            }
            break;
        case 3:
            if (get_CD_State() == 2) {
                device_cmd_send(1285);
                en_nichi_count_set(1, 0);
                sub_sub_step_up();
            }
            break;
        }
        v0 = step_counter + 1;
        step_counter = v0;
        if (v0 >= 361) {
            device_cmd_send(127);
            sub_sub_step_up();
            sub_sub_step_up();
        }
        if ((pad_trig & 0x800) != 0) {
            device_cmd_send(127);
            sub_sub_step_up();
            sub_sub_step_up();
        }
        return;

    busy:
        sub_sub_step_up();
        return;

    case 4:
        v0 = step_counter + 1;
        step_counter = v0;
        if ((pad_trig & 0x60) != 0) {
            device_cmd_send(127);
            sub_sub_step_up();
        }
        return;

    case 5:
        en_nichi_fade_out();
        if (en_nichi_grid_bright == 0) {
            sub_step_up();
        }
        return;
    }
}
