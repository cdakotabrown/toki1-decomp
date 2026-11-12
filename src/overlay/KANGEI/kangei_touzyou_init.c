/* @CompileEnv: IDO52_FORCE_WEB_COLOR=10:t4,65:t0,32:t0 */
/* @SymbolStart: 0x80135848 */
/* @SymbolSize: 632 */
/* Address: 0x80135850 (true entry 0x80135848: the kangei_touzyou_done_flag read is */
/* Size: 624 bytes (156 insns) + 8 hoisted = 632 */
/* @PostProc: swap_instructions --offset1 0x178 --offset2 0x17c */
/* @PostProc: patch_insn_reg_field 0x34:rt:t4:t7 0x38:rs:t4:t7 0x38:rt:t4:t7 0x40:rs:t4:t7 0x7c:rs:t7:t8 0x7c:rt:t7:t8 0x8c:rs:t8:t9 0x8c:rt:t8:t9 0x10c:rt:t5:t8 0x13c:rd:t6:t0 0x13c:rt:v1:v0 0x14c:rs:t9:t2 0x14c:rt:t9:t2 0x21c:rt:t5:t0 0x24c:rt:t0:a0 0x70:rt:t7:t8 0x74:rs:t7:t8 0x80:rs:t7:t8 0x88:rt:t8:t9 0x94:rs:t8:t9 0x100:rd:t7:t6 0x100:rt:v1:v0 0x104:rd:t8:t7 0x104:rt:t7:t6 0x108:rs:t8:v1 0x108:rt:v0:t7 0x110:rs:t5:t8 0x110:rt:t5:t8 0x114:rs:v1:v0 0x118:rs:t9:t8 0x118:rt:t5:t9 0x130:rt:v1:v0 0x134:rs:v1:v0 0x134:rt:v1:v0 0x138:rt:t9:t2 0x140:rd:t7:t1 0x140:rt:t6:t0 0x144:rs:t7:v1 0x144:rt:v0:t1 0x150:rs:v1:v0 0x150:rt:t8:t3 0x154:rs:t8:t2 0x154:rt:t9:t3 0x214:rt:t5:t0 0x250:rs:t0:a0 0xc4:rt:v1:v0 0xc8:rt:a0:v1 0xcc:rd:t9:t1 0xd0:rd:t5:t2 0xd0:rt:t9:t1 0xd4:rs:t5:t2 0xd4:rt:v0:t3 0xdc:rd:t6:t5 0xdc:rt:v0:t3 0xe0:rs:t6:t5 0xf8:rs:v1:v0 0xf8:rt:v1:v0 0xfc:rs:a0:v1 0xfc:rt:v0:v1 */
#include "types.h"

extern void sub_step_up(void);
extern s32 srand(s32 a0);
extern s32 rand(void);
extern void back_clear_switch(s32);
extern void class_clear_all(s32);
extern void draw2d3d(s32, s32);
extern void class_switch(s32);
extern void prim_init(void);
extern void message_window_init(void);
extern void hizuke_init(void);
extern void tpage_buf_clear(void);
extern void text_sys_init(void);
extern void k_reset(s32);
extern void shared_system_state_reset(void);
extern void parameter_disp_switch(s32);
extern void icon_disp_switch(s32);
extern void bustup_ptr_init(void);
extern void yazirushi_off(void);
extern void date_name_set(s32);
extern void kangei_ptr_tbl2_init(void);
extern void kangei_girl_msg_init(void);
extern void sub_sub_step_up(void);

extern u8 girl_no;
extern volatile u8 now_girl;
extern s32 frame_counter;
extern u8 screen_w[];
extern s32 girl0_birth;
extern u8 now_day;
extern u8 now_month;
extern s32 p_birthday;
extern s32 para_fixed;
extern u8 kangei_touzyou_done_flag;
extern u8 vblank_flag;
extern s32 vb_ready;
extern s16 kangei_now_s;
extern s16 kangei_now_s_in;
extern u8 kangei_icon_flag;

s32 kangei_touzyou_init(void)
{
    u8 scene_id;
    s32 rnd;
    s32 rnd_masked;
    u8 cur_char;
    u8 *char_entry;
    s32 flags;
    s32 sprite_data1;
    s32 sprite_data2;
    u8 val1, val2;
    s32 half;
    s32 shift_result;

    if (kangei_touzyou_done_flag != 0) {
        sub_step_up();
        return 0;
    }

    scene_id = girl_no;
    if (scene_id == 0xFF) {
        sub_step_up();
        return 0;
    }

    srand(frame_counter);
    rnd = rand();

    rnd_masked = rnd % 256;

    if (rnd_masked >= 5 || now_girl >= 11U) {
        sub_step_up();
        return 0;
    }

    cur_char = *(u8 *)&now_girl;
    char_entry = screen_w + cur_char * 0x38;
    flags = *(s32 *)(char_entry + 0x1C8);

    if ((flags << 30) >= 0) {
        sub_step_up();
        return 0;
    }

    sprite_data1 = girl0_birth;
    val1 = now_day;
    shift_result = (s32)(((u32)(sprite_data1 << 23)) >> 27);

    if (val1 == shift_result) {
        val2 = now_month;
        if (val2 == (sprite_data1 & 0xF)) {
            sub_step_up();
            return 0;
        }
    }

    sprite_data2 = p_birthday;
    shift_result = (s32)(((u32)(sprite_data2 << 23)) >> 27);

    if (val1 == shift_result) {
        val2 = now_month;
        if (val2 == (sprite_data2 & 0xF)) {
            sub_step_up();
            return 0;
        }
    }

    scene_id = girl_no;
    now_girl = scene_id;

    half = para_fixed;
    kangei_touzyou_done_flag = 1;

    para_fixed = half / 2;

    {
        u8 *entry = screen_w + scene_id * 0x38;
        *(s32 *)(entry + 0x1C4) = 0;
    }

    back_clear_switch(1);
    class_clear_all(0);
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    message_window_init();
    hizuke_init();
    tpage_buf_clear();
    text_sys_init();
    k_reset(1);

    vblank_flag = 0;
    vb_ready = 1;

    shared_system_state_reset();
    parameter_disp_switch(0);
    icon_disp_switch(0);

    kangei_now_s = 0;
    kangei_now_s_in = 0;

    bustup_ptr_init();
    yazirushi_off();

    cur_char = now_girl;
    date_name_set(cur_char);

    kangei_ptr_tbl2_init();
    kangei_girl_msg_init();

    kangei_icon_flag = 0;
    sub_sub_step_up();

}
