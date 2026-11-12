/* @SymbolSize: 104 */
/* @PostProc: swap_branch_operands --offsets 0x4c */
/* Address: 0x80138DE4 */
/* Size: 112 bytes (28 insns including 2-insn cross-prologue tail) */
#include "types.h"

extern void func_80138664(void);

extern u8  now_girl;
extern s16 now_serifu_shugaku_in;
extern u8  shugaku_place;
extern s32 bustup_on;

void shugaku_serifu_speech_switch(void)
{
    func_80138664();

    if (now_girl == 1) {
        if (now_serifu_shugaku_in == 3) {
            if (shugaku_place == 1) {
                bustup_on = 0;
                return;
            }
        }
    }
    bustup_on = 1;
}
