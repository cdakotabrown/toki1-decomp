/* @PostProc: swap_instructions --offset1 0x170 --offset2 0x174 */
/* @PostProc: patch_insn_reg_field 0x30:rt:v0:v1 0x34:rs:v0:v1 0x34:rt:v0:v1 0x3c:rs:v0:v1 0x40:rs:v0:v1 0x84:rt:t9:v1 0x8c:rt:t9:v1 0x9c:rt:v0:v1 0xa0:rs:v0:v1 0xa0:rt:v0:v1 0xa8:rs:v0:v1 0xb4:rs:v0:v1 0xb8:rt:v1:v0 0xbc:rs:v1:v0 0xbc:rt:v1:v0 0xc0:rt:a1:v1 0xc4:rs:v1:v0 0xcc:rs:v1:v0 0xd0:rd:t0:t9 0xd4:rd:t2:t0 0xd4:rt:t0:t9 0xdc:rt:t2:t0 0xe4:rs:t2:t0 0xf4:rt:v1:v0 0xf8:rs:v1:v0 0xf8:rt:v1:v0 0x100:rs:v1:v0 0x118:rs:a1:v1 0x118:rt:v0:v1 0x11c:rt:t4:t2 0x120:rs:v0:v1 0x120:rt:t3:t1 0x124:rs:t3:t1 0x128:rd:t6:t4 0x128:rt:v0:v1 0x12c:rs:t4:t2 0x12c:rt:t4:t2 0x134:rd:t5:t3 0x134:rt:t4:t2 0x138:rd:t5:t3 0x138:rs:t5:t3 0x138:rt:t4:t2 0x13c:rd:t7:t5 0x13c:rs:t5:t3 0x13c:rt:t6:t4 0x140:rt:t7:t5 0x150:rt:t8:t6 0x154:rs:t8:t6 0x154:rt:t9:t7 0x158:rs:t9:t7 0x160:rt:v0:v1 0x164:rt:a1:v1 0x168:rs:a1:v1 0x168:rt:a1:v1 0x170:rs:a1:v1 0x170:rt:at:v0 0x174:rs:a1:v1 0x178:rs:at:v0 0x17c:rt:t1:t9 0x184:rt:t1:t9 0x1a4:rt:v0:v1 0x1a8:rs:v0:v1 0x1a8:rt:v0:v1 0x1ac:rt:t2:t0 0x1b0:rs:v0:v1 0x1bc:rs:t2:t0 0x1bc:rt:t2:t0 0x1c4:rs:t2:t0 0x1d4:rt:v0:v1 0x1e4:rt:v0:v1 0x1ec:rs:v0:v1 0x1f0:rt:t3:t1 0x1f4:rs:t3:t1 0x1f4:rt:t3:t1 0x1fc:rs:t3:t1 0x20c:rt:t4:t2 0x210:rs:t4:t2 0x210:rt:t4:t2 0x218:rs:t4:t2 0x218:rt:t5:t3 0x21c:rt:t5:t3 */
/* @SymbolStart: 0x801333F0 */
/* @SymbolSize: 544 */
/* Address: 0x80133400 (window starts at the absorbed cross-prologue head */
/* Size: 528 bytes (132 insns) + the 16-byte head = 136 insns. */
#include "types.h"

extern void device_cmd_send(s32);
extern void k_reset(s32);
extern s32 check_end_k(void);
extern void shougatu_serifu_run(void);

extern u8 holiday_tel_exit_tbl0[];
extern s16 sound_seek_time;
extern u8 now_girl;
extern s32 pad_trig;
extern s32 step_counter;
extern u8 step_mode;
extern s32 serifu_wait;
extern volatile s16 shougatu_meet_now_serifu_in;
extern s16 shougatu_voice_cnt;

s32 shougatu_voice_step2(void)
{
    s32 st;
    s32 n;
    u8 fa;
    u8 *p;
    u8 fb;
    s32 k;
    s32 m;
    s32 tv;
    u8 fc;

    if ((pad_trig & 0x00400040) != 0) {
        st = shougatu_meet_now_serifu_in;
        if (st > 0) {
            if (st < 3) {
                if ((u32)step_counter >= 2) {
                    device_cmd_send(5384);
                    serifu_wait = 0;
                    k_reset(1);
                    sound_seek_time = 0;
                    shougatu_meet_now_serifu_in = 2;
                    goto tail;
                }
            }
        }
    }
    if (check_end_k() == 0) {
        goto tail;
    }
    st = shougatu_meet_now_serifu_in;
    if (st >= 3) {
        goto tail;
    }
    if (st == 0) {
        n = shougatu_voice_cnt;
        p = &step_mode;
        if (n < 90) {
            n = (s16)(n + 1);
            shougatu_voice_cnt = (s16)n;
            if (n == 1) {
                device_cmd_send(1546);
            }
            if (shougatu_voice_cnt == 30) {
                device_cmd_send(1540);
            }
            return 0;
        }
        fa = *p;
        if ((fa & 7) == 0) {
            step_mode = (u8)fa;
            k = now_girl * 7 + ((u32)fa >> 3);
            device_cmd_send((u16)((u8)(holiday_tel_exit_tbl0[k] + 1285) | 0x600));
        }
    }
    fb = step_mode;
    fc = (u8)(fb + 1);
    if ((u32)fb < 40U) {
        step_mode = fc;
        return 0;
    }
    step_mode = 0;
    k_reset(1);
tail:
    shougatu_serifu_run();
    tv = shougatu_meet_now_serifu_in;
    if (tv < 3 && step_counter == 1) {
        shougatu_meet_now_serifu_in = (s16)tv;
        device_cmd_send(1544);
    } else {
        shougatu_meet_now_serifu_in = (s16)tv;
        if (tv == 3 && step_counter == 1) {
            device_cmd_send(1547);
        }
    }
    step_counter = step_counter + 1;
}
