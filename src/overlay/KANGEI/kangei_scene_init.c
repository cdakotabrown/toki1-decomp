/* @PostProc: patch_insn_reg_field 0x10c:rt:a0:t8 */
/* @PostProc: patch_insn_reg_field 0x110:rt:v1:t4 0x110:rs:v0:v0 */
/* @PostProc: patch_insn_reg_field 0x114:rt:a0:t8 */
/* @PostProc: patch_insn_reg_field 0x128:rt:t8:t5 0x128:rs:v1:t4 */
/* @PostProc: patch_insn_reg_field 0x12c:rd:t4:t6 0x12c:rs:t8:t3 0x12c:rt:t3:t5 */
/* @PostProc: patch_insn_reg_field 0x130:rt:t4:t6 */
/* @PostProc: patch_insn_reg_field 0x134:rt:t5:t7 */
/* @PostProc: patch_insn_reg_field 0x140:rt:t5:t7 */
/* kangei_scene_init - KANGEI overlay, Address: 0x80133600, Size: 324 bytes (81 insns) */
#include "types.h"

extern void device_load_start(s32 a0, u32 a1, u32 a2, u32 a3, s32 stackArg0, s32 stackArg1);
extern s32 device_load_poll(void);
extern void sub_step_up(void);
extern void kangei_init(void);
extern void hizuke_init(void);
extern void icon_init(void);
extern void icon_can_use_set(s32, s32);
extern void icon_cursor_move(s32);
extern void parameter_show_init(void);

extern u8 cdq_footer_chk;

extern u8 kangei_touzyou_done_flag;
extern u8 kangei_icon_flag;
extern u8 kangei_birth_done;

extern u8 now_day;

extern u8 fade_bri;

extern u16 basyo_pack;

extern u8 screen_w[2048];

void
kangei_scene_init(void)
{
    u32 val;
    u8 byte;
    u32 temp;
    s32 result;
    s32 i;
    u16 sprite;
    u32 *entry;
    u32 idx;

    cdq_footer_chk = 0;

    device_load_start(1, 0xC07D, 0xC073, 0xCC41, 0xCBF1, 0xCBE7);

    result = device_load_poll();
    if (result == 1) {
        sub_step_up();
    }

    kangei_touzyou_done_flag = 0;
    kangei_icon_flag = 0;
    kangei_birth_done = 0;

    kangei_init();

    now_day = now_day - 1;
    hizuke_init();

    now_day = now_day + 1;
    icon_init();

    i = 0;
    do {
        icon_can_use_set(i, 0);
        i = i + 1;

        i = (s16)i;
    } while (i < 15);

    icon_cursor_move(1);
    parameter_show_init();
    parameter_show_init();

    sprite = basyo_pack;
    idx = ((u32)sprite) >> 12;

    entry = (u32 *)&screen_w[idx * 4 + 1644];
    val = entry[0];
    byte = screen_w[idx * 4 + 1647];

    temp = (val << 5) >> 30;
    temp = temp + 1;
    temp = (temp << 1) & 0x0006;
    byte = (byte & 0xFFF9) | temp;
    screen_w[idx * 4 + 1647] = byte;

    fade_bri = 128;
    sub_step_up();
}
