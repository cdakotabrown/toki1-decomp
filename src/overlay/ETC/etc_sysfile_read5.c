/* @SymbolStart: 0x8014196C */
/* Address: 0x80141974 */
/* Size: 308 bytes (77 instructions) */
/* @SymbolSize: 316 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x30:rs:v0:v1 0x6c:rt:t6:v1 0x70:rs:t6:v1 0x70:rt:t6:v1 0x78:rs:t6:v1 0x78:rt:t7:t6 0x80:rt:t7:t6 0x9c:rt:t0:t8 0xb4:rt:t8:v1 0xb8:rs:t8:v1 0xb8:rt:t8:v1 0xc0:rs:t8:v1 0xc0:rt:t9:t7 0xc8:rt:t9:t7 0xcc:rs:t0:t8 0xcc:rt:t0:t8 0xd0:rt:t1:t9 0xd4:rs:t0:t8 0xe0:rs:t1:t9 0xe0:rt:t1:t9 0xe8:rs:t1:t9 */
#include "types.h"

extern void k_sub_reset(void);
extern void set_kanji_string(s32, s32, s32, s32, s32);
extern void k_sub_disp_start(s32);
extern void card_step_init(void);
extern s32  card_file_read_main(void);
extern void card_state_clear(void);
extern void printf(s32);
extern void goto_sub_sub_step(s32);
extern s32  save_work_restore(s32);
extern void sub_sub_step_up(void);
extern void dec_bg_show(s32);

extern u8  step_mode;
extern s32 card_cnt;
extern u8  card_result;
extern u8  etc_sysfile_read5_str0[];
extern u8  etc_sysfile_read5_str1[];

void etc_sysfile_read5(void)
{
    s32 sel = step_mode;

    switch (sel) {
    case 0:
        k_sub_reset();
        set_kanji_string(-120, 48, 0, (s32)etc_sysfile_read5_str0, 0);
        k_sub_disp_start(2);
        step_mode++;
        card_step_init();
        break;
    case 1:
        if (card_file_read_main() == 3) {
            card_state_clear();
            printf((s32)etc_sysfile_read5_str1);
            step_mode++;
            break;
        }
        if ((u32)card_cnt < 513) {
            if (card_result != 240) {
                break;
            }
        }
        card_state_clear();
        goto_sub_sub_step(255);
        break;
    case 2:
        if (save_work_restore(0) == -1) {
            goto_sub_sub_step(253);
            break;
        }
        sub_sub_step_up();
        break;
    }
    dec_bg_show(2);
}
