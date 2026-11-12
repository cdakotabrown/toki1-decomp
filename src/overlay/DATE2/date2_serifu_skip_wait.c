/* @SymbolStart: 0x801328D0 */
/* @SymbolSize: 288 */
/* @PostProc: swap_instructions --offset1 0xa0 --offset2 0xa4 */
/* @PostProc: patch_insn_reg_field 0x30:rt:v1:a0 0x34:rs:v1:a0 0x34:rt:v1:a0 0x3c:rs:v1:a0 0xa0:rt:at:v1 0xa4:rt:v1:v0 0xa8:rs:at:v1 0xac:rs:v1:v0 0xd4:rt:v1:a0 0xd8:rs:v1:a0 0xd8:rt:v1:a0 0xe0:rs:v1:a0 0xe0:rt:v0:v1 0xe4:rs:v1:a0 0xe4:rt:v1:a0 0xe8:rs:v0:v1 0xec:rt:v1:a0 */
/* Address: 0x801328E0, Size: 272 bytes (68 insns) + 16-byte cross-prologue head */
#include "types.h"

extern u32 pad_trig;
extern u32 step_counter;
extern u32 serifu_wait;
extern u16 sound_seek_time;
extern u16 date2_now_s;
extern s16 date2_now_s_in;
extern u8  step_mode;

void device_cmd_send(s32 arg0);
void k_reset(s32 arg0);
void sub_sub_step_up(void);
s32  check_end_k(void);
void date2_serifu_run2(void);

s32 date2_serifu_skip_wait(void)
{
    u32 a0;
    u32 lt;
    s32 v0;
    u8  cnt;
    u8  nc;
    s16 t;
    s16 t1;

    if (pad_trig & 0x00400040) {
        a0 = step_counter;
        if (a0 >= 2) {
            device_cmd_send(5384);
            serifu_wait = 0;
            k_reset(1);
            sound_seek_time = 0;
            date2_now_s = 1;
            date2_now_s_in = 0;
            sub_sub_step_up();
            return 0;
        }
    }

    v0 = check_end_k();
    if (v0 != 0) {
        cnt = step_mode;
        nc = cnt + 1;
        if (cnt < 64U) {
            step_mode = nc;
            return 0;
        }
        step_mode = 0;
        k_reset(1);
    }

    date2_serifu_run2();
    a0 = step_counter;
    lt = (a0 < 1U);
    step_counter = a0 + 1;
    if (lt) {
        t = date2_now_s_in;
        if (t < 3) {
            t1 = date2_now_s;
            if (t1 == 0) {
                device_cmd_send(1545);
            }
        }
    }

}
