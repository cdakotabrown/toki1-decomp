/* @SymbolStart: 0x80136860 */
/* @SymbolSize: 560 */
/* @CompileEnv: IDO52_AS1_TIE_SRC_ORDER=bb:5,6 IDO52_UGEN_TEMP_REG=11:t2,12:t7,13:t8,14:t9,15:t0,16:t6 */
/* @PostProc: patch_insn_reg_field 0x1d4:rs:t0:t5 0x14:rt:v1:v0 0x1c:rs:v1:v0 0x2c:rt:v1:v0 0x30:rs:v1:v0 0x30:rt:v1:v0 0x38:rs:v1:v0 0x38:rt:v1:v0 0x3c:rs:v1:v0 0x40:rd:v1:v0 0x48:rt:v1:v0 0x50:rs:v1:v0 0x64:rt:t7:v0 0x6c:rs:t7:v0 0x9c:rt:t8:v0 0xa0:rs:t8:v0 0xa0:rt:t8:v0 0xa8:rs:t8:v0 0xa8:rt:t9:t7 0xb0:rt:t9:t7 0xc8:rt:t0:t8 0xcc:rt:t3:t0 0xd0:rs:t3:t0 0xd0:rt:t4:t1 0xd4:rs:t0:t8 0xd4:rt:t1:t9 0xd8:rt:t7:t3 0xdc:rs:t7:t3 0xdc:rt:t8:t4 0xe0:rs:t1:t9 0xe0:rt:t2:t9 0xe4:rs:t4:t1 0xe4:rt:t5:t1 0xe8:rs:t2:t9 0xe8:rt:t5:t1 0xec:rs:t8:t4 0xec:rt:t9:t4 0xf0:rt:t9:t4 0xf4:rt:t0:t5 0xf8:rs:t0:t5 0xfc:rs:t0:t5 0x108:rt:t1:t7 0x10c:rt:t1:t7 0x110:rt:t3:t8 0x114:rs:t3:t8 0x114:rt:t3:t8 0x118:rt:t5:t9 0x11c:rd:t4:t0 0x11c:rt:t3:t8 0x120:rd:t4:t0 0x120:rs:t4:t0 0x120:rt:t3:t8 0x124:rd:t4:t0 0x124:rt:t4:t0 0x128:rs:t5:t9 0x128:rt:t5:t9 0x12c:rs:t4:t0 0x12c:rt:t5:t9 0x130:rt:t7:t1 0x138:rs:t7:t1 0x138:rt:t8:t3 0x140:rt:t8:t3 0x158:rt:t9:t4 0x15c:rt:t9:t4 0x160:rt:t0:t5 0x164:rs:t0:t5 0x164:rt:t0:t5 0x168:rt:t1:t7 0x16c:rt:t0:t5 0x170:rt:t0:t5 0x178:rs:t1:t7 0x178:rt:t1:t7 0x17c:rt:t1:t7 0x180:rt:t3:t8 0x188:rs:t3:t8 0x188:rt:t4:t0 0x190:rt:t4:t0 0x19c:rt:t5:t9 0x1a0:rs:t5:t9 0x1a0:rt:t5:t9 0x1a4:rt:t8:t3 0x1a8:rd:t7:t1 0x1a8:rt:t5:t9 0x1ac:rd:t7:t1 0x1ac:rs:t7:t1 0x1ac:rt:t5:t9 0x1b0:rd:t7:t1 0x1b0:rt:t7:t1 0x1b4:rs:t8:t3 0x1b4:rt:t8:t3 0x1b8:rs:t7:t1 0x1b8:rt:t8:t3 0x1c0:rt:t0:t5 0x1c4:rt:t9:t4 0x1c8:rt:t9:t4 0x1cc:rs:t0:t5 0x1cc:rt:t0:t5 0x1e0:rt:t1:t7 0x1e4:rs:t1:t7 0x1e4:rt:t1:t7 0x1e8:rt:t4:t0 0x1ec:rd:t3:t8 0x1ec:rt:t1:t7 0x1f0:rd:t3:t8 0x1f0:rs:t3:t8 0x1f0:rt:t1:t7 0x1f4:rd:t3:t8 0x1f4:rt:t3:t8 0x1f8:rs:t4:t0 0x1f8:rt:t4:t0 0x1fc:rs:t3:t8 0x1fc:rt:t4:t0 0x200:rt:t5:t9 0x204:rt:t8:t3 0x208:rs:t5:t9 0x208:rt:t7:t1 0x20c:rt:t7:t1 0x210:rs:t8:t3 0x210:rt:t8:t3 0x218:rs:t8:t3 */
/* date2_end_step - DATE2 overlay, Address: 0x80136868, TRUE Size 552 bytes */
#include "types.h"

extern void season_bgm_set(void);
extern void CDQ_init(void);
extern void device_state_init(void);
extern s32 device_load_poll(void);
extern void param_clamp(void);
extern void return_schedule(s32);

extern u8 step_mode;
extern s32 step_counter;
extern u8 date2_mode_done_flag;
extern u8 screen_w[][56];
extern u8 girl_no;
extern s16 girl0_yuukou;
extern s16 girl0_syoushin;

s32 date2_end_step(void)
{
    s32 sel;
    s32 c;
    u32 cond;
    u32 b0;
    u32 b1;
    u32 b2;
    u8 bits;
    s32 v;

    sel = step_mode;
    if (sel == 0) {
        season_bgm_set();
        sel = (u8)(step_mode + 1);
    }
    step_mode = sel;
    if (sel == 1) {
        {
            c = step_counter;
            cond = ((u32)c < 1024U) ^ 1;
            c += 1;
            step_counter = c;
            if (cond != 0) {
                CDQ_init();
                device_state_init();
                step_mode = 0;
            }
            if (device_load_poll() == 1) {
                v = step_mode;
                step_mode = v + 1;
            } else {
                return 0;
            }
        }
    }

    v = date2_mode_done_flag;
    b0 = ((u32)(v & 1) < 1U) ^ 1;
    b1 = ((u32)(v & 2) < 1U) ^ 1;
    b2 = ((u32)(v & 4) < 1U) ^ 1;
    bits = b0 + b1 + b2;
    if (bits == 0) {
        girl0_yuukou += 12;
        *(s16 *)(screen_w[girl_no] + 454) += 7;
    } else if (bits == 1) {
        girl0_yuukou += 3;
        *(s16 *)(screen_w[girl_no] + 454) += 2;
    } else if (bits == 2) {
        *(s16 *)(screen_w[girl_no] + 450) += 3;
        girl0_syoushin += 2;
    } else {
        *(s16 *)(screen_w[girl_no] + 450) += 12;
        girl0_syoushin += 7;
    }
    param_clamp();
    return_schedule(1);
}
