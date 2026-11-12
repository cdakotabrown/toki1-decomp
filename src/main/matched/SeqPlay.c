/* Function: SeqPlay */
/* Address: 0x80092AA0 */
/* Size: 268 bytes (67 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @CompileFlags: -Wo,-do_opt_saved_regs */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_XBB_SKIP_CAND=0,1,2 IDO52_AS1_NO_FALLTHRU_FILL=occ:1,2 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 *_ss_score[];
extern void GetSeqData(s32, s32);

void SeqPlay(s16 arg0, s16 arg1) {
    u8 *base;
    s32 field136;
    s16 field110;
    s16 field108;
    s32 diff;

    base = _ss_score[arg0] + (arg1 * 21) * 8;

    field136 = *(s32 *)(base + 136);
    field110 = *(s16 *)(base + 110);

    diff = field136 - field110;

    if (diff > 0) {
        field108 = *(s16 *)(base + 108);
        if (field108 > 0) {
            *(s16 *)(base + 108) = field108 - 1;
        } else if (field108 == 0) {
            *(s16 *)(base + 108) = field110;
            *(s32 *)(base + 136) = field136 - 1;
        } else {
            *(s32 *)(base + 136) = diff;
        }
    } else if (field110 >= field136) {
        s32 accumulator = field136;
        do {
            GetSeqData(arg0, arg1);
            field136 = *(s32 *)(base + 136);
            if (field136 != 0) {
                field110 = *(s16 *)(base + 110);
                accumulator += field136;
                if (accumulator >= field110) {
                    break;
                }
            }
        } while (1);
        *(s32 *)(base + 136) = accumulator - field110;
    }
}
