/* @PostProc: patch_insn_reg_field 0x30:rt:a0:v0 0x78:rs:a0:v0 0x78:rt:a0:v0 0x84:rt:a0:v0 0x9c:rt:a0:v0 0xf4:rt:v0:t0 0xf8:rs:v0:t0 0xf8:rt:v0:t0 0xfc:rt:v1:t2 0x100:rs:v1:t2 0x100:rt:v1:t2 0x104:rt:a0:v0 0x10c:rs:v0:t0 0x10c:rt:t0:t1 0x110:rs:a0:v0 0x110:rt:a0:v0 0x114:rt:t0:t1 0x11c:rd:t1:t3 0x11c:rs:v1:t2 0x11c:rt:a0:v0 0x120:rt:t1:t3 0x124:rt:t2:t4 0x128:rs:t2:t4 0x128:rt:t2:t4 0x130:rd:t3:t5 0x130:rs:t2:t4 0x130:rt:a0:v0 0x134:rt:t3:t5 0x144:rt:t4:t6 0x148:rs:t4:t6 0x148:rt:t4:t6 0x154:rs:t4:t6 0x15c:rd:t5:t7 0x160:rs:t5:t7 */
/* Address: 0x80141D78, Size: 368 bytes (92 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern void geko_serifuev_run(void);
extern void bustup_wink(void);
extern void bustup_timer_clamp(s32);
extern void bustup2_flag_on(void);

extern u16 now_serifuev_in;
extern u8 bustup_cd_read_tbl0;
extern u32 frame_counter;
extern volatile u8 bustup_cw_disp;
extern volatile u8 bustup2_cw_disp;
extern u8 cal_cw_mode;
extern s16 cal_cw_frame;
extern volatile s32 Sprites;
extern volatile s32 sprite1_attribute;
extern s32 bustup_on;
extern CLASS_W class_w[];

void geko_bustup_wink_step2(void)
{
    u32 tmpB;
    u32 tmpC;

    geko_serifuev_run();

    if (now_serifuev_in == 2) {
        bustup_cd_read_tbl0 = 0;
        bustup_on = 0;
        bustup2_cw_disp |= 0x80;
        bustup_cw_disp |= 0x80;
        class_w[31].frame = 0;
        Sprites &= 0x7FFFFFFF;
        sprite1_attribute &= 0x7FFFFFFF;
        if (frame_counter % 240 == 0) {
            bustup_wink();
        }
        return;
    }

    bustup_timer_clamp(0);
    bustup_on = 1;
    bustup2_cw_disp |= 0x80;
    tmpB = bustup_cw_disp;
    tmpC = Sprites;
    bustup_cw_disp = tmpB | 0x80;
    Sprites = tmpC & 0x7FFFFFFF;
    sprite1_attribute &= 0x7FFFFFFF;
    cal_cw_mode = 0;
    cal_cw_frame = 0;
    if (frame_counter % 240 == 120) {
        bustup2_flag_on();
    }
}
