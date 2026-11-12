/* @SymbolStart: 0x80143DD4 */
/* @SymbolSize: 164 */
/* @PostProc: patch_insn_reg_field 0x24:rt:a0:v0 0x28:rs:a0:v0 0x28:rt:a0:v0 0x30:rs:a0:v0 0x38:rs:a0:v0 0x40:rs:a0:v0 0x48:rs:a0:v0 0x70:rt:a0:t7 0x74:rs:a0:t7 0x74:rt:a0:t7 0x78:rt:t0:t9 0x7c:rd:t7:t8 0x7c:rt:a0:t7 0x80:rs:t7:t8 0x80:rt:a0:t7 0x84:rd:t9:t8 0x88:rs:t0:t9 0x88:rt:t0:t9 0x8c:rd:v1:v0 0x8c:rs:t9:t8 0x8c:rt:t0:t9 0x90:rs:v1:v0 0x90:rt:t1:t0 0x98:rs:t1:t0 0x98:rt:t2:t1 0xa0:rs:v1:v0 0xa0:rt:t2:t1 */
/* geko_serifu_face_step -- GEKO overlay, Address: 0x80143DDC, Size: 156 bytes (39 insns) */
#include "types.h"

extern void bustup_face_step4(void);
extern void bustup_face_step3(void);
extern void sub_sub_step_up(void);

extern u8 now_girl;
extern s8 screen_w[];
extern s32 serifu_no;

void geko_serifu_face_step(void)
{
    s32 t6 = serifu_no;

    u8 val;
    s16 *ptr;
    s32 offset;

    if (t6 != 0) {
        val = now_girl;
        if (val == 2 || val == 7 || val == 9 || val == 10) {
            bustup_face_step4();
        } else {
            bustup_face_step3();
        }
        return;
    }

    val = now_girl;
    offset = val;
    offset = (offset << 3) - offset;
    offset = offset << 3;
    ptr = (s16 *)((u8 *)&screen_w[offset + 0x1c2]);
    (*ptr)++;
    sub_sub_step_up();
}
