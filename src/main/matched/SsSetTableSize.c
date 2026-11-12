/* Address: 0x8008B570 */
/* Size: 972 bytes (243 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 -O2 codegen, native as1 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_CONST_NOINREG_VALUES=168,672 IDO52_COPYPROP_VAR=-4:17 IDO52_LDA_NOINREG_CLASSES=4 IDO52_AS1_NO_LA_PULL=1,2 IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_UGEN_TEMP_REG=8:t9,14:t8,13:t7 IDO52_FORCE_WEB_COLOR=19:t1,29:t0,33:a3,113:t0 IDO52_NO_CSE_SITE=mpy:9,mpy:11,mpy:13,mpy:15,mpy:17,mpy:19,mpy:21,mpy:23,mpy:25,mpy:27,mpy:29,mpy:31 IDO52_AS1_NO_PASS1_PROP=15,19,20,29 IDO52_AS1_TIE_TAKE=249,254 IDO52_AS1_TIE_REJECT=38,39,209,210,211,212,216,217,218,219,222,223,224,225,226,227,228,229,230,231 */
/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_instructions 0x150 0x15c */
/* @PostProc: swap_instructions 0x154 0x158 */
/* @PostProc: swap_instructions 0x170 0x17c */
/* @PostProc: swap_instructions 0x174 0x178 */
/* @PostProc: swap_instructions 0x190 0x1a4 */
/* @PostProc: swap_instructions 0x194 0x198 */
/* @PostProc: swap_instructions 0x1a4 0x1b4 */
#include "types.h"

typedef struct {
    u8 b[168];
} Voice;

extern u32 _ss_score[];
extern s16 _snd_seq_s_max;
extern s16 _snd_seq_t_max;

void SsSetTableSize(u8 *base, s32 cols, s32 rows)
{
    s32 i;
    s32 j;
    s32 n;
    s32 rem;
    u32 *p;
    u32 *end;
    u32 a;
    u32 b;
    u32 c;
    u32 d;
    s32 m;

    cols = (s16)cols;
    rows = (s16)rows;
    _snd_seq_s_max = cols;
    _snd_seq_t_max = rows;

    i = 0;
    if (cols > 0) {
        rem = cols & 3;
        if (rem != 0) {
            p = &_ss_score[i];
            n = i + 1;
            a = (u32)&((Voice *)base)[i * rows];
            do {
                rows = rows | 0;
                *p = a;
                i = n;
                n = n + 1;
                a += rows * 168;
                p = (u32 *)(((u32)p | 0) + 4);
            } while (rem != i);
            if (i == cols) goto skip;
        }
        {
            p = &_ss_score[i];
            end = &_ss_score[cols];
            a = (u32)&((Voice *)base)[i * rows];
            b = (u32)&((Voice *)base)[(i + 1) * rows];
            c = (u32)&((Voice *)base)[(i + 2) * rows];
            d = (u32)&((Voice *)base)[(i + 3) * rows];
            do {
                rows = rows | 0;
                p[0] = a;
                p[1] = b;
                p[2] = c;
                p[3] = d;
                p = (u32 *)(((u32)p | 0) + 16);
                i += 4;
                a += rows * 672;
                rows = rows | 0;
                b += rows * 672;
                rows = rows | 0;
                c += rows * 672;
                rows = rows | 0;
                d += rows * 672;
            } while (p != end);
        }
skip:
        i = 0;
    }

    if (cols > 0) {
        m = 1;
        do {
            if (_snd_seq_t_max > 0) {
                j = 0;
                do {
                    *(u32 *)((u8 *)_ss_score[i] + j * 168 + 0x90) = 0;
                    goto L1; L1:
                    *(s8 *)((u8 *)_ss_score[i] + j * 168 + 0x00) = -1;
                    goto L2; L2:
                    *(s16 *)((u8 *)_ss_score[i] + j * 168 + 0x3C) = 0;
                    goto L3; L3:
                    *(s16 *)((u8 *)_ss_score[i] + j * 168 + 0x3E) = 0;
                    goto L4; L4:
                    *(u32 *)((u8 *)_ss_score[i] + j * 168 + 0x94) = 0;
                    goto L5; L5:
                    *(u32 *)((u8 *)_ss_score[i] + j * 168 + 0x98) = 0;
                    goto L6; L6:
                    *(s16 *)((u8 *)_ss_score[i] + j * 168 + 0x40) = 0;
                    goto L7; L7:
                    *(u32 *)((u8 *)_ss_score[i] + j * 168 + 0xA4) = 0;
                    goto L8; L8:
                    *(u32 *)((u8 *)_ss_score[i] + j * 168 + 0xA0) = 0;
                    goto L9; L9:
                    *(u32 *)((u8 *)_ss_score[i] + j * 168 + 0x9C) = 0;
                    goto L10; L10:
                    *(s16 *)((u8 *)_ss_score[i] + j * 168 + 0x42) = 0;
                    goto L11; L11:
                    *(s16 *)((u8 *)_ss_score[i] + j * 168 + 0x72) = 127;
                    goto L12; L12:
                    *(s16 *)((u8 *)_ss_score[i] + j * 168 + 0x74) = 127;

                    j = (j | 0) + 1;
                } while (j < _snd_seq_t_max);
            }
            i = m;
            m = m + 1;
        } while (i < _snd_seq_s_max);
    }
}
