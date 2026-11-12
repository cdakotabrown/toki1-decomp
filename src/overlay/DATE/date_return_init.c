/* @SymbolStart: 0x8013CB80 */
/* @SymbolSize: 516 */
/* @PostProc: patch_stack_complete --old 48 --new 64 */
/* @PostProc: remap_stack_slots --from 40 --to 48 --from 42 --to 50 --from 44 --to 52 --from 46 --to 54 */
/* @PostProc: patch_insn_reg_field 0x4c:rt:t3:t4 0x60:rd:t0:t1 0x64:rs:t0:t1 0x6c:rt:t1:t2 0x74:rs:t1:t2 0x74:rt:t2:t3 0x78:rt:t2:t3 0x7c:rs:t3:t4 0x7c:rt:t3:t4 0x84:rs:v1:t4 0x84:rt:t3:v1 0x8c:rt:t4:t5 0x94:rs:t4:t5 0x94:rt:t5:t6 0x98:rt:t5:t6 0xa0:rd:t6:t7 0xa4:rt:t6:t7 0xbc:rt:v0:t9 0xc0:rs:v0:t9 0xc0:rt:v0:t9 0xc8:rd:t8:t0 0xc8:rt:v0:t9 0xcc:rd:t9:t0 0xcc:rs:t8:t0 0xcc:rt:v0:t9 0xd0:rt:t9:t0 0xd4:rd:v1:v0 0xd8:rs:v1:v0 0xe8:rs:v1:v0 */
/* Function: date_return_init */
/* Overlay: DATE */
/* Address: 0x8013CB80 (cross-prologue head) / 0x8013CB88 (body) */
/* Size: 516 bytes (129 instructions) */
#include "types.h"

extern void prim_init(void);
extern void sub_sub_step_up(void);
extern void draw2d3d(s32 a0, s32 a1);
extern void back_clear_switch(s32 a0);
extern void tpage_buf_clear(void);
extern void class_switch(s32 a0);
extern void class_clear_all(s32 a0);
extern void text_sys_init(void);
extern void k_reset(s32 a0);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void bustup_ptr_init(void);
extern void date_name_set(u8 a0);
extern void param_clamp(void);
extern void yazirushi_off(void);
extern void shared_system_state_reset(void);
extern void ClearImage(void *a0, s32 a1, s32 a2, s32 a3);

extern s32 rpg_bat_arg;
extern s8 fade_bri;
extern u8 ev_flag55;
extern u8 girl_no;
extern s16 para_str;
extern u8 screen_w[];
extern u8 screen_bri;
extern volatile u8 now_girl;
extern u8 vblank_flag;
extern volatile s32 vb_ready;

void date_return_init(void)
{
    s16 buf[4];
    s16 *rec;
    s32 idx;
    s16 i;

    if (rpg_bat_arg == 0) {
        if (ev_flag55 == 2) {
            for (i = 0; i < 11; i++) {
                rec = (s16 *)(screen_w + i * 56);
                if ((*(s32 *)(rec + 228) << 30) < 0) {
                    rec[223] = (s16)(rec[223] + 3);
                }
                if (i == girl_no) {
                    rec[223] = (s16)(rec[223] + 2);
                }
            }
        } else {
            idx = girl_no;
            rec = (s16 *)(screen_w + (((idx << 3) - idx) << 3));
            rec[223] = (s16)(rec[223] + 2);
        }
    } else {
        para_str = (s16)(para_str + 5);
    }

    param_clamp();
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    shared_system_state_reset();
    k_reset(1);

    buf[0] = 320;
    buf[1] = 256;
    buf[2] = 320;
    buf[3] = 248;
    ClearImage(buf, 8, 0, 0);

    buf[0] = 704;
    buf[1] = 0;
    ClearImage(buf, 8, 0, 0);

    vblank_flag = 0;
    vb_ready = 1;
    date_name_set(now_girl);
    bustup_ptr_init();
    screen_bri = 128;
    fade_bri = 0;
    hizuke_init();
    message_window_init();
    yazirushi_off();
    sub_sub_step_up();
}
