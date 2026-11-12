/* @PostProc: patch_stack_complete --old 56 --new 64 */
/* @PostProc: remap_stack_slots --from 48 --to 52 --from 50 --to 54 --from 52 --to 56 --from 54 --to 58 */
/* @PostProc: patch_insn_reg_field 0x38:rt:v1:v0 0x3c:rs:t7:v0 0x3c:rt:v1:t7 0x40:rt:a0:v1 0x44:rs:a0:v1 0x44:rt:a0:v1 0x4c:rs:v1:v0 0x4c:rt:a0:v1 0x54:rs:a0:v1 0x88:rt:a0:v1 0x8c:rs:a0:v1 0xb8:rt:t2:t3 0xc0:rt:t2:t3 0xcc:rt:t3:t4 0xd0:rs:t3:t4 0xd0:rt:t3:t4 0xdc:rs:t3:t4 0xdc:rt:t4:t5 0xe0:rt:t4:t5 */
/* kangei_bustup_serifu_run - KANGEI overlay, Address: 0x80135014, Size: 220 bytes (55 insns) */
#include "types.h"

extern void func_80133F08(void);
extern void StoreImage(s16 *, s32);
extern void DrawSync(s32);
extern s32 get_g_zyotai_h(u8 a0);
extern void bustup_sprite_init(s32, s32, s32, s32, s32);
extern void bustup_kuchi_ani(void);

extern s16 kangei_now_s_in;
extern s32 step_counter;
extern u8 now_girl;
extern s32 bustup_check_init_tbl2;
extern s32 bustup_check_init_tbl1;
extern s32 bustup_check_init_tbl0;
extern s32 bustup_check_init_tbl3;

void
kangei_bustup_serifu_run(void)
{
    s16 rect[4];
    s32 counter;
    s32 v0;
    u8 charIdx;

    func_80133F08();

    if (kangei_now_s_in != 2) {
        goto increment;
    }

    if (step_counter != 1) {
        goto increment;
    }

    charIdx = now_girl;
    if (charIdx != 1 && charIdx != 3) {
        goto increment;
    }

    rect[0] = 320;
    rect[1] = 128;
    rect[2] = 384;
    rect[3] = 128;

    StoreImage(rect, 0x80180000);
    DrawSync(0);

    charIdx = now_girl;
    v0 = get_g_zyotai_h(charIdx);

    bustup_sprite_init(bustup_check_init_tbl3, bustup_check_init_tbl0, bustup_check_init_tbl1, bustup_check_init_tbl2, v0 & 0x7F);
    bustup_kuchi_ani();

increment:
    counter = step_counter;
    step_counter = counter + 1;
}
