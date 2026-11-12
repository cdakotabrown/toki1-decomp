/* @SymbolStart: 0x801338FC */
/* @CompileFlags: -Wc,-pic1 */
/* @SymbolSize: 296 */
/* @PostProc: patch_insn_reg_field 0x64:rt:t7:v0 0x6c:rs:t7:v0 0xa8:rt:t8:t7 0xb0:rt:t8:t7 0xc4:rt:t9:t8 0xc8:rt:t0:t9 0xcc:rt:t1:t0 0xd0:rt:t2:t1 0xd4:rt:t9:t8 0xd8:rt:t0:t9 0xdc:rt:t1:t0 0xe0:rt:t2:t1 0xf8:rt:t3:t2 0xfc:rt:t3:t2 */
/* kangei_scene_exit - KANGEI overlay, Address: 0x80133904, Size: 288 bytes (72 insns) */
#include "types.h"

extern void season_bgm_set(void);
extern s32 CDQ_init(void);
extern void device_state_init(void);
extern s32 device_load_poll(void);
extern void dec_bg_reset(void);
extern void ClearImage(s16 *, s32, s32, s32);
extern void hizuke_advance(void);
extern u32 return_schedule(s32);

extern volatile u8 step_mode;

extern s32 step_counter;

u32
kangei_scene_exit(void)
{
    s32 v;
    s16 rect[4];

    v = step_mode;
    if (v == 0) {
        season_bgm_set();
        v = (u8)(step_mode + 1);
    }

    step_mode = v;

    if (v != 1) {
        goto state_machine;
    }

    v = ((u32)step_counter < 1024) ^ 1;
    step_counter = step_counter + 1;
    if (v != 0) {
        CDQ_init();
        device_state_init();
        step_mode = 0;
    }

    if (device_load_poll() == 1) {
        v = step_mode;
        step_mode = v + 1;
        goto state_machine;
    }
    return 0;

state_machine:
    dec_bg_reset();

    rect[0] = 320;
    rect[1] = 256;
    rect[2] = 320;
    rect[3] = 248;
    ClearImage(rect, 8, 0, 0);

    rect[0] = 704;
    rect[1] = 0;
    ClearImage(rect, 8, 0, 0);

    hizuke_advance();

    return return_schedule(1);
}
