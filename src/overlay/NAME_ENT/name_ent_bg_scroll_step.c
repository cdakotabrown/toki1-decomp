/* @SymbolStart: 0x80145784 */
/* @RodataBase: 0x8014C938 */
/* @PostProc: patch_stack_complete --old 72 --new 88 */
/* @PostProc: swap_instructions --offset1 0x120 --offset2 0x124 */
/* @PostProc: swap_instructions --offset1 0x1c8 --offset2 0x1d4 */
/* @PostProc: swap_instructions --offset1 0x1cc --offset2 0x1d0 */
/* @PostProc: remap_stack_slots --from 64 --to 76 --from 66 --to 78 --from 68 --to 80 --from 70 --to 82 */
/* @PostProc: patch_insn_reg_field 0x3c:rt:v0:v1 0x40:rs:v0:v1 0x40:rt:v0:v1 0x48:rs:v0:v1 0x50:rt:v0:v1 0x7c:rt:t2:v1 0x80:rs:t2:v1 0x80:rt:t2:v1 0x88:rs:t2:v1 0x88:rt:t3:t2 0x90:rt:t3:t2 0xa4:rt:t4:t3 0xa8:rs:t4:t3 0xa8:rt:t4:t3 0xac:rt:t5:t4 0xb0:rs:t4:t3 0xb8:rs:v0:v1 0xb8:rt:t6:t5 0xc0:rt:t5:t4 0xcc:rt:t6:t5 0xd0:rt:t7:t6 0xd8:rt:t7:t6 0xf0:rt:t8:v1 0xf4:rs:t8:v1 0xf4:rt:t8:v1 0xfc:rs:t8:v1 0xfc:rt:t9:t7 0x104:rt:t9:t7 0x110:rt:t0:t8 0x118:rt:v1:v0 0x11c:rt:t0:t8 0x124:rs:v1:v0 0x124:rt:v1:v0 0x12c:rs:v1:v0 0x134:rs:v0:v1 0x134:rt:t1:t9 0x140:rt:t1:t9 0x144:rt:t2:t0 0x150:rt:t2:t0 0x154:rt:t3:t1 0x158:rs:t3:t1 0x174:rt:t4:t2 0x178:rs:t4:t2 0x178:rt:t4:t2 0x180:rs:t4:t2 0x1ac:rt:t9:t7 0x1b0:rs:t9:t7 0x1b0:rt:t9:t7 0x1b4:rt:t5:t3 0x1b8:rt:t6:t4 0x1bc:rt:t7:t5 0x1c0:rt:t8:t6 0x1c4:rt:t0:t8 0x1c8:rt:t8:t6 0x1cc:rt:t7:t5 0x1d0:rt:t6:t4 0x1d4:rt:t5:t3 0x1d8:rt:t0:t8 0x1fc:rt:t9:t7 0x200:rt:t1:t9 0x204:rs:t1:t9 0x204:rt:t1:t9 0x20c:rs:t1:t9 */
/* @SymbolSize: 556 */
/* Address: 0x8014578C, Size: 548 bytes (137 insns) */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
    s16 w;
    s16 h;
} Rect;

extern s32 frame_counter;
extern s32 name_ent_step_cnt;
extern u8 name_ent_serifu_no;
extern u8 step_mode;
extern s32 step_counter;
extern s32 dec_bg_handle;

extern void name_ent_serifu_set(void);
extern void name_ent_text_scroll(void);
extern void value_cache_find_or_load(s32, s32);
extern s32 get_CD_State2(void);
extern s32 dec_bg_init(s32);
extern void indexed_byte_lsb_set(s32, s32);
extern void dec_bg_reset(void);
extern void sub_step_up(void);
extern void srn_tpage_show(s32, s32, s32, Rect *, s32, s32, s32, s32, s32);
extern void name_ent_fall_obj_move(s32);

void name_ent_bg_scroll_step(void)
{
    Rect r;
    s32 v;
    s32 ge;

    if (frame_counter & 1) {
        name_ent_step_cnt = name_ent_step_cnt + 1;
    }

    switch (step_mode) {
    case 0:
        name_ent_serifu_no = 1;
        name_ent_serifu_set();
        step_mode = step_mode + 1;
        break;
    case 1:
        name_ent_text_scroll();
        break;
    case 2:
        if (name_ent_step_cnt >= 321) {
            name_ent_step_cnt = 320;
            step_mode = step_mode + 1;
        }
        break;
    case 3:
        name_ent_step_cnt = 320;
        step_counter = 0;
        value_cache_find_or_load(18781, 1);
        step_mode = step_mode + 1;
        break;
    case 4:
        v = step_counter;
        ge = (u32)v >= 121;
        name_ent_step_cnt = 320;
        step_counter = v + 1;
        if (ge) {
            step_mode = step_mode + 1;
        }
        break;
    case 5:
        name_ent_step_cnt = 320;
        if (get_CD_State2() & 1) {
            indexed_byte_lsb_set(1, dec_bg_init(1));
            if (dec_bg_handle == -1) {
                dec_bg_reset();
                value_cache_find_or_load(18781, 1);
            } else {
                sub_step_up();
            }
        }
        break;
    }

    r.x = -160;
    r.y = -120;
    r.w = 320;
    r.h = 160;
    srn_tpage_show(5, 2, 0, &r, name_ent_step_cnt, 0, 0, 0, 13);
    if (name_ent_step_cnt == 320) {
        name_ent_fall_obj_move(1);
    } else {
        name_ent_fall_obj_move(0);
    }
}
