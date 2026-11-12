/* @SymbolStart: 0x80137E68 */
/* ending_girl1_cue_step - ENDING overlay, Address: 0x80137E70, Size: 284 bytes (71 insns) */
/* @SymbolSize: 292 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=1 */
/* @PostProc: patch_insn_reg_field 0x58:rt:v0:a0 0x5c:rs:v0:a0 0x5c:rt:v0:a0 0x60:rt:t3:t2 0x64:rd:t8:v0 0x64:rt:v0:a0 0x68:rd:t9:t8 0x68:rt:t8:v0 0x6c:rs:t9:t8 0x6c:rt:t0:t9 0x70:rs:t0:t9 0x70:rt:t0:t9 0x74:rt:t0:t9 0x78:rt:t2:t1 0x7c:rt:t2:t1 0x80:rt:t6:t5 0x84:rs:t3:t2 0x84:rt:t3:t2 0x8c:rs:t3:t2 0x90:rt:t4:t3 0x98:rt:t4:t3 0xa8:rt:t5:t4 0xb4:rt:t5:t4 0xb8:rs:t6:v1 0xb8:rt:v1:t5 0xbc:rt:t8:t7 0xc0:rt:t7:t6 0xc4:rs:t7:t6 0xc4:rt:t7:t6 0xcc:rs:t7:t6 0xe8:rs:t8:v1 0xe8:rt:v1:t7 0xec:rt:t9:t8 0xf0:rs:t9:t8 0xf0:rt:t9:t8 0xf8:rs:t9:t8 0x108:rt:t0:t9 0x110:rt:t0:t9 0x114:rt:t1:t0 0x118:rs:t1:t0 0x118:rt:t1:t0 0x124:rs:t1:t0 0x124:rt:t2:t1 0x128:rt:t2:t1 */
#include "types.h"

extern u32 girl1_flag;
extern s16 ending_now_serifu_in;
extern s32 ending_serifu_frame;
extern u16 bustup2_cw_anim;
extern u32 wink2_stop;

void ending_bustup_anim_set(s32 arg0, s32 arg1);

void ending_girl1_cue_step(void)
{
    s32 h_v1 = ending_now_serifu_in;
    u32 a0;
    s32 v0;

    if (h_v1 == 1) {
        if (ending_serifu_frame == 0) {
            bustup2_cw_anim = 4;
            ending_bustup_anim_set(0, 255);
            wink2_stop = 0;
        }
    } else {
        a0 = girl1_flag;
        v0 = (((a0 << 17) >> 31) >= 1U) << 1;
        if (h_v1 == v0 + 19) {
            if (ending_serifu_frame == 1) {
                bustup2_cw_anim = 5;
                ending_bustup_anim_set(1, 1);
                wink2_stop = 1;
            }
        } else if (h_v1 == v0 + 22) {
            if (ending_serifu_frame == 1) {
                ending_bustup_anim_set(2, 0);
                wink2_stop = 0;
            }
        } else if (h_v1 == v0 + 23) {
            if (ending_serifu_frame == 1) {
                ending_bustup_anim_set(3, 5);
                wink2_stop = 1;
            }
        }
    }

    ending_serifu_frame = ending_serifu_frame + 1;
}
