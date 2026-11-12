/* @SymbolStart: 0x80141BD4 */
/* @RodataBase: 0x8014D098 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v1:22 IDO52_FORCE_WEB_COLOR=42:a0 */
/* Address: 0x80141BDC */
/* Size: 544 bytes (136 insns) */
/* @SymbolSize: 552 */
/* @PostProc: scheduler_normalizer --const-fp --store-fp --branch-fp */
/* @PostProc: patch_insn_reg_field 0xf0:rt:v0:v1 0xf4:rs:v0:v1 0xf4:rt:v0:v1 0xfc:rs:v0:v1 */
#include "types.h"

extern void card_step_init(void);
extern void etc_rec_collect(void);
extern s32  card_file_write_main(void);
extern void card_state_clear(void);
extern void goto_sub_sub_step(s32);
extern void printf(s32, s32);
extern void set_XAsector(s32, s32, s32);
extern void device_cmd_send(s32);
extern s32  get_CD_State(void);
extern void device_state_init(void);
extern void goto_step(s32);
extern void set_dec_bri(s32);
extern void dec_bg_show(s32);

extern u8  step_mode;
extern u32 card_cnt;
extern u8  card_result;
extern u8  now_girl;
extern u8  mascot_flag2[];
extern u8  screen_bri;
extern s32 etc_card_save_xa_tbl[];
extern s32 voice_click_check_tbl0[];
extern u8  etc_c_girl_fmt[];

void etc_card_save(void) {
    s32 n;
    s32 c;

    switch (step_mode) {
    case 0:
        card_step_init();
        etc_rec_collect();
        step_mode++;
        break;
    case 1:
        if (card_file_write_main() == 19) {
            card_state_clear();
            step_mode++;
        } else {
            if (card_cnt < 513 && card_result != 240) {
                break;
            }
            card_state_clear();
            goto_sub_sub_step(255);
        }
        break;
    case 2:
        printf((s32)etc_c_girl_fmt, now_girl);
        n = now_girl & 0xF;
        if (mascot_flag2[n] & 1) {
            set_XAsector(etc_card_save_xa_tbl[n * 2], etc_card_save_xa_tbl[n * 2 + 1], n);
        } else {
            set_XAsector(voice_click_check_tbl0[n * 2], voice_click_check_tbl0[n * 2 + 1], n);
        }
        device_cmd_send(768);
        step_mode++;
        break;
    case 3:
        if (get_CD_State() & 4) {
            step_mode++;
        }
        break;
    case 4:
        if (get_CD_State() & 2) {
            step_mode++;
        }
        break;
    case 5:
        n = screen_bri;
        c = (u32)n < 8;
        screen_bri = n - 1;
        if (c != 0) {
            screen_bri = 0;
            device_state_init();
            goto_step(196);
            now_girl = now_girl + 16;
        }
        set_dec_bri(screen_bri);
        break;
    }
    dec_bg_show(2);
}
