/* Address: 0x800927B0 */
/* Size: 752 bytes (188 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_CONST_NOINREG_VALUES=168 IDO52_NO_CSE_OPCODES=mpy IDO52_AS1_NO_LA_PULL=all IDO52_FORCE_WEB_COLOR=5:v1,35:t0 IDO52_COPYPROP_VAR=-24 IDO52_AS1_FORCE_BOPT=1,1 IDO52_AS1_NO_PASS1_PROP=25,74 IDO52_AS1_DIV_MACRO=aspsx IDO52_AS1_NO_FALLTHRU_FILL=all */
/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern u8 *_ss_score[];
extern u32 VBLANK_MINUS;

void Snd_tempo(s16 channel, s16 voice)
{
    u8 **slot;
    u8 *rec;
    s32 step;
    u32 cur;
    u32 tgt;
    u32 old;
    u32 n;

    slot = &_ss_score[channel];
    rec = *slot + voice * 168;
    goto L0;
L0:
    step = *(s16 *)(rec + 66);
    *(volatile s32 *)(rec + 160) = *(volatile s32 *)(rec + 160) - 1;
    n = *(volatile u32 *)(rec + 160);

    if (step > 0) {
        if (n % step != 0) {
            return;
        }
        cur = *(volatile u32 *)(rec + 140);
        tgt = *(u32 *)(rec + 164);
        old = cur;
        goto L1;
L1:
        if (tgt < old) {
            *(volatile u32 *)(rec + 140) = cur - 1;
            cur = *(volatile u32 *)(rec + 140);
        } else if (old < tgt) {
            *(volatile u32 *)(rec + 140) = cur + 1;
            cur = *(volatile u32 *)(rec + 140);
        }
        *(volatile s16 *)(rec + 110) = (*(s16 *)(rec + 72) * cur * 10) / (VBLANK_MINUS * 60);
        if (*(volatile s16 *)(rec + 110) <= 0) {
            *(volatile s16 *)(rec + 110) = 1;
        }
        if (*(volatile s32 *)(rec + 160) != 0) {
            if (*(u32 *)(rec + 164) != *(volatile u32 *)(rec + 140)) {
                return;
            }
        }
        rec = *slot + voice * 21 * 8;
        *(s32 *)(rec + 144) &= ~64;
        rec = *slot + voice * 8 * 21;
        *(s32 *)(rec + 144) &= ~128;
    } else {
        cur = *(volatile u32 *)(rec + 140);
        tgt = *(u32 *)(rec + 164);
        old = cur;
        goto L2;
L2:
        if (tgt < old) {
            *(volatile u32 *)(rec + 140) = (s32)cur + step;
            cur = *(volatile u32 *)(rec + 140);
            if (cur < tgt) {
                *(volatile u32 *)(rec + 140) = tgt;
                cur = *(volatile u32 *)(rec + 140);
            }
        } else if (old < tgt) {
            *(volatile u32 *)(rec + 140) = cur - step;
            cur = *(volatile u32 *)(rec + 140);
            if (tgt < cur) {
                *(volatile u32 *)(rec + 140) = tgt;
                cur = *(volatile u32 *)(rec + 140);
            }
        }
        *(volatile s16 *)(rec + 110) = (*(s16 *)(rec + 72) * cur * 10) / (VBLANK_MINUS * 60);
        if (*(volatile s16 *)(rec + 110) <= 0) {
            *(volatile s16 *)(rec + 110) = 1;
        }
        if (*(volatile s32 *)(rec + 160) != 0) {
            if (*(u32 *)(rec + 164) != *(volatile u32 *)(rec + 140)) {
                return;
            }
        }
        rec = *slot + voice * 21 * 8;
        *(s32 *)(rec + 144) &= ~64;
        rec = *slot + voice * 8 * 21;
        *(s32 *)(rec + 144) &= ~128;
    }
}
