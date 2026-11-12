/* @SymbolStart: 0x80138DC8 */
/* @SymbolSize: 364 */
/* @PostProc: swap_instructions --offset1 0xd0 --offset2 0xd4 */
/* @PostProc: patch_insn_reg_field 0x5c:rt:v0:v1 0x60:rs:v0:v1 0x60:rt:v0:v1 0x68:rs:v0:v1 0x68:rt:t8:v0 0x6c:rs:t8:v0 0x6c:rt:t9:v0 0x70:rs:v0:v1 0x70:rt:t7:v1 0x74:rs:t9:v0 0x78:rt:t7:v1 0xc8:rt:t0:t7 0xcc:rs:t0:t7 0xcc:rt:t0:t7 0xd0:rt:t1:a1 0xd8:rs:t1:a1 0xd8:rt:t1:a1 0xdc:rt:t0:t7 0xe4:rs:t1:a1 0xf4:rt:v1:a1 0xf8:rs:v1:a1 0xf8:rt:v1:a1 0x100:rs:v1:a1 0x104:rt:a1:v1 0x108:rt:t2:t8 0x10c:rs:t2:t8 0x10c:rt:t2:t8 0x110:rs:a1:v1 0x110:rt:a1:v1 0x114:rd:t3:t9 0x114:rt:t2:t8 0x118:rs:t3:v1 0x118:rt:a1:t9 0x11c:rt:t4:t0 0x124:rs:t4:t0 0x128:rt:a0:v0 0x12c:rs:a0:v0 0x12c:rt:t5:t1 0x130:rt:t6:t2 0x134:rs:v1:a1 0x134:rt:t5:t1 0x140:rs:a0:v0 0x140:rt:t6:t2 0x144:rt:t7:t4 0x148:rs:t7:t4 0x148:rt:t7:t4 0x14c:rt:t9:t3 0x150:rd:t8:t5 0x150:rt:t7:t4 0x154:rd:v0:t6 0x154:rs:t8:v1 0x154:rt:a1:t5 0x158:rs:v0:t6 0x158:rt:t9:t3 */
/* geko_return_schedule2 -- GEKO overlay, Address: 0x80138DD0, Size: 356 bytes (89 insns) */
#include "types.h"

extern void device_cmd_send(s32 a0);
extern s32 device_state_init(void);
extern s32 device_load_poll(void);
extern void CDQ_init(void);
extern void season_bgm_set(void);
extern s32 return_schedule(s32);
extern void event_log_add(s32 a0);

extern u8 screen_w[];

extern u8 now_event_no;
extern volatile u8 rec_idx;
extern u8 now_girl;
extern volatile s32 step_counter;
extern volatile u8 step_mode;
extern s32 event_girl_no;
extern s32 date_disp_flag;

s32 geko_return_schedule2(void)
{
    s32 v0 = step_mode;

    s32 counter;
    u32 ge;
    u8 m868;
    u8 *base;
    u8 *p;
    u8 *p2;

    if (v0 == 0) {
        device_cmd_send(178);
        season_bgm_set();
        v0 = (u8)(step_mode + 1);
    }
    step_mode = v0;

    if (v0 == 1) {
        counter = step_counter;
        ge = ((u32)counter < 1024U) ^ 1;
        step_counter = counter + 1;
        if (ge != 0) {
            CDQ_init();
            device_state_init();
            step_mode = 0;
        }
        if (device_load_poll() != 1) {
            return 0;
        }
    }

    device_cmd_send(34);
    device_cmd_send(33);
    device_cmd_send(47);
    now_girl = event_girl_no;

    m868 = now_event_no;
    if (m868 != 255) {
        event_log_add(2);
        m868 = now_event_no;
        if (m868 == 20) {
            base = screen_w;
            p = base + rec_idx * 8;
            if (p[1880] == 2) {
                p2 = (u8 *)((u32)p - 8U);
                if (m868 == p2[1887]) {
                    p2[1889] = 4;
                    goto end;
                }
            }
            p = base + rec_idx * 8;
            p[1889] = 4;
        }
    }

end:
    date_disp_flag = 0;
    return_schedule(1);
}
