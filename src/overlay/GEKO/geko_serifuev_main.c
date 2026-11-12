/* @SymbolStart: 0x8013A374 */
/* @SymbolSize: 416 */
/* @PostProc: swap_instructions --offset1 0xbc --offset2 0xc0 */
/* @PostProc: swap_instructions --offset1 0x124 --offset2 0x128 */
/* @PostProc: patch_insn_reg_field 0x30:rt:v0:t8 0x34:rs:v0:t8 0x34:rt:v0:t8 0x3c:rs:v0:t8 0x40:rs:v0:t8 0x48:rt:t8:t9 0x4c:rs:t8:t9 0x4c:rt:t8:t9 0x54:rs:t8:t9 0x84:rt:t9:t0 0x8c:rt:t9:t0 0x9c:rt:t0:t1 0xa0:rs:t0:t1 0xa0:rt:t0:t1 0xa4:rt:a1:v1 0xa8:rs:t0:t1 0xb4:rs:a1:v1 0xb4:rt:a1:v1 0xbc:rs:a1:v1 0xbc:rt:t2:v0 0xc0:rs:a1:v1 0xc4:rs:t2:v0 0xc8:rt:t1:t2 0xd0:rt:t1:t2 0x118:rt:v0:t4 0x11c:rs:v0:t4 0x11c:rt:v0:t4 0x124:rs:v0:t4 0x124:rt:t5:v0 0x128:rs:v0:t4 0x128:rt:t4:t5 0x12c:rs:t5:v0 0x130:rt:t4:t5 0x164:rt:v0:t8 0x168:rs:v0:t8 0x168:rt:v0:t8 0x170:rs:v0:t8 0x170:rt:t9:v0 0x174:rs:t9:v0 0x174:rt:t0:v0 0x178:rs:v0:t8 0x178:rt:t8:t9 0x17c:rs:t0:v0 0x180:rt:t8:t9 0x184:rt:t1:t0 0x188:rs:t1:t0 0x188:rt:t1:t0 0x190:rs:t1:t0 */
/* geko_serifuev_main -- GEKO overlay, Address: 0x8013A384 (window 0x8013A374, +16 */
#include "types.h"

extern s32 device_cmd_send(s32);
extern s32 k_reset(s32);
extern s32 check_end_k(void);
extern s32 geko_serifuev_run(void);
extern s32 geko_serifuev_run2(void);

extern u16 now_serifuev;
extern volatile u16 now_serifuev_in;
extern s16 sound_seek_time;
extern s32 pad_trig;
extern volatile s32 step_counter;
extern u8 step_mode;
extern s32 serifu_wait;

s32 geko_serifuev_main(void)
{
    s32 counter;
    u32 lt;
    u32 sv;
    u32 slot;

    if ((pad_trig & 0x00400040) != 0 && (slot = now_serifuev_in) != 0 &&
        slot < 4U && !((u32)step_counter < 2U)) {
        device_cmd_send(5384);
        serifu_wait = 0;
        k_reset(1);
        sound_seek_time = 0;
        now_serifuev_in = 3;
    } else if (check_end_k() != 0 && (u32)now_serifuev_in < 4U) {
        sv = step_mode;
        lt = sv < 64U;
        sv = (u8)(sv + 1);
        if (lt != 0) {
            step_mode = sv;
            return 0;
        }
        step_mode = 0;
        k_reset(1);
    }

    if ((u32)now_serifuev_in < 3U) {
        geko_serifuev_run2();
    } else {
        geko_serifuev_run();
    }

    counter = step_counter;
    lt = (u32)counter < 1U;
    step_counter = counter + 1;
    if (lt != 0) {
        if ((u32)now_serifuev_in < 4U && now_serifuev == 0) {
            device_cmd_send(1545);
        }
    }

    counter = step_counter;
    lt = (u32)(counter ^ 1) < 1U;
    step_counter = counter + 1;
    if (lt != 0) {
        if (now_serifuev_in == 5) {
            device_cmd_send(1546);
        }
    }
}
