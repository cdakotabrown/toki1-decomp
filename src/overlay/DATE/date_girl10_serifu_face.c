/* @SymbolSize: 92 */
/* @PostProc: patch_insn_reg_field 0x30:rt:t6:v0 0x38:rs:t6:v0 0x40:rt:t7:t6 0x44:rs:t7:t6 0x44:rt:t7:t6 0x4c:rs:t7:t6 */
/* Function: date_girl10_serifu_face */
/* Overlay: DATE */
/* Address: 0x8015A104 */
/* Size: 92 bytes (23 instructions) */
#include "types.h"

extern void func_8014C530(void);
extern void bustup_face_change(s32 a);

extern s32 step_counter;
extern u16 now_serifuev_in;

void date_girl10_serifu_face(void)
{
    s32 old;
    s32 isOne;

    func_8014C530();
    old = step_counter;
    isOne = (u32)(old ^ 1) < 1U;
    step_counter = old + 1;
    if (isOne != 0) {
        if (now_serifuev_in == 3) {
            bustup_face_change(1);
        }
    }
}
