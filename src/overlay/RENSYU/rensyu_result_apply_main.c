/* @SymbolStart: 0x80133C70 */
/* @SymbolSize: 420 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=2:v1 */
/* @PostProc: swap_instructions --offset1 0xc0 --offset2 0xc8 */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xc8 */
/* @PostProc: swap_instructions --offset1 0xd4 --offset2 0xd8 */
/* @PostProc: swap_instructions --offset1 0x120 --offset2 0x124 */
/* @PostProc: swap_instructions --offset1 0x124 --offset2 0x128 */
/* @PostProc: swap_instructions --offset1 0x128 --offset2 0x12c */
/* @PostProc: patch_insn_reg_field 0x98:rd:v1:v0 0x9c:rs:v1:v0 0xa0:rs:v1:v0 0xb4:rs:v1:v0 */
#include "types.h"

extern void device_cmd_send(u32 a0);
extern void season_bgm_set(void);
extern void sub_sub_step_up(void);
extern u32 device_load_poll(void);
extern void shared_system_state_reset(void);
extern void set_dec_bri(u32 a0);
extern void return_schedule(u32 a0);
extern void game_reset(void);
extern void common_disp(void);

typedef struct { u16 lo : 4; u16 hi : 12; } Nib;
extern u8 now_sub_sub_step;
extern u8 rensyu_succ_cnt;
extern u8 rensyu_girl_no;
extern u16 basyo_pack;
extern u8 screen_w[];
extern s16 para_kon;
extern s16 para_str;
extern s16 girl0_tokimeki;

void rensyu_result_apply_main(void)
{
    u32 sel;
    u32 t6;
    u32 idx;
    u8 *p;
    u32 row;
    s32 i;

    sel = now_sub_sub_step;
    switch (sel) {
    case 0:
        device_cmd_send(191);
        season_bgm_set();
        sub_sub_step_up();
        break;

    case 1:
        if (device_load_poll() == 1) {
            t6 = rensyu_succ_cnt;
            if (t6 >= 3) {
                idx = (u32)basyo_pack >> 12;
                p = &screen_w[idx * 4];
                ((Nib *)(p + 1646))->lo++;

                girl0_tokimeki = girl0_tokimeki + 2;

                for (i = 1; i < 13; i++) {
                    *(s16 *)&screen_w[i * 56 + 446] += 2;
                }
            } else {
                para_kon = para_kon + 10;
                para_str = para_str + 10;
            }

            row = rensyu_girl_no;
            if (row != 255) {
                p = &screen_w[row * 56];
                *(s16 *)(p + 450) = *(s16 *)(p + 450) + 2;
            }

            shared_system_state_reset();
            set_dec_bri(0);
            return_schedule(1);
        }
        break;

    default:
        game_reset();
        break;
    }

    common_disp();
}
