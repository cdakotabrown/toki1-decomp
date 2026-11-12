/* Function: ContDataEntry (registered alias event_kuchi_ani_no) */
/* Address: 0x80093A88 */
/* Size: 1520 bytes (380 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 (t-register temps, `lui`/`addiu` */
/* @CompileFlags: -cckr -Wo,-do_opt_saved_regs -Wc,-pic1 */
/* @CompileEnv: IDO52_UGEN_MOVE_LOOP_SHIFT=2 IDO52_LDA_NOINREG_CLASSES=1:2:7 IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_NO_FALLTHRU_FILL=9,13 IDO52_AS1_KEEP_DEAD_RELOAD=0 IDO52_AS1_NO_PASS1_PROP=193,265 IDO52_AS1_NO_COPY_PROP=17 IDO52_UGEN_TEMP_REG=26:t0,31:t2,36:t3,41:t0,42:t3 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: remap_stack_slots --map 80:88,84:92,88:96,92:100,116:108,124:116 */
#include "types.h"

#define R8(rec, off)   (((u8 *)(rec))[(off)])
#define R16(rec, off)  (*(s16 *)((u8 *)(rec) + (off)))
#define R32(rec, off)  (*(s32 *)((u8 *)(rec) + (off)))

extern u8 *_ss_score[];

extern void SsUtGetProgAtr(s16 ch, u8 patch, u8 *countOut);
extern s32  ReadDeltaValue(s16 ch, s16 seq);
extern void SsUtGetVagAtr(s16 ch, u8 patch, s32 idx, u8 *scratch);
extern void SsUtSetVagAtr(s16 ch, u8 patch, s32 idx, u8 *scratch);
typedef struct { u8 b[32]; } SA;
typedef struct { s16 h[9]; } SB;
extern s32  Snd_setVabAttr(s16 ch, u8 patch, s32 mode, SA a, SB b, u8 f21, u8 val);

void ContDataEntry(ch, seq, val)
s16 ch;
s16 seq;
u8 val;
{
    u8  pad[8];
    u8 *rec;
    u8 *sub;
    u32 i;
    u32 off;
    u16 pitch;
    u8  unused;
    u8  count;
    SA  scratch;
    SB  other;

    rec = (u8 *)_ss_score[ch] + seq * 168;
    off = R8(rec, 18);
    sub = rec + off;

    SsUtGetProgAtr(R16(rec, 74), sub[44], &count);

    if (R8(rec, 39) == 1 && R8(rec, 16) == 0) {
        R8(rec, 40) = val;
        R8(rec, 16) = 1;
        R32(rec, 136) = ReadDeltaValue(ch, seq);
        return;
    }

    if (R8(rec, 22) != 30 && R8(rec, 22) != 20) {
        R8(rec, 21) = val;
        R8(rec, 42) = R8(rec, 42) + 1;
        R32(rec, 136) = ReadDeltaValue(ch, seq);
        return;
    }

    if (R8(rec, 41) == 2) {
        if (R8(rec, 19) == 0 && R8(rec, 20) == 0) {
            i = 0;
            if (count == 0) goto a1_done;
        loop1:
            SsUtGetVagAtr(R16(rec, 74), sub[44], i, scratch.b);
            scratch.b[13] = val & 0x7F;
            scratch.b[12] = val & 0x7F;
            SsUtSetVagAtr(R16(rec, 74), sub[44], i, scratch.b);
            i++;
            if (i < count) goto loop1;
        a1_done: ;
        }
        if (R8(rec, 19) == 1 && R8(rec, 20) == 0) {
            if (val >= 65 && val < 128) {
                pitch = ((val * 100) >> 13) << 13;
            } else {
                pitch = 0;
            }
            i = 0;
            if (count == 0) goto a2_done;
            do {
                SsUtGetVagAtr(R16(rec, 74), sub[44], i, scratch.b);
                scratch.b[5] = scratch.b[5] + pitch;
                SsUtSetVagAtr(R16(rec, 74), sub[44], i, scratch.b);
                i++;
            } while (i < count);
        a2_done: ;
        }
        if (R8(rec, 19) == 2 && R8(rec, 20) == 0) {
            if (val >= 64 && val < 128) {
                pitch = (val * 25) << 8;
            } else {
                pitch = 0;
            }
            i = 0;
            if (count == 0) goto a3_done;
            do {
                SsUtGetVagAtr(R16(rec, 74), sub[44], i, scratch.b);
                scratch.b[4] = scratch.b[4] + pitch;
                SsUtSetVagAtr(R16(rec, 74), sub[44], i, scratch.b);
                i++;
            } while (i < count);
        a3_done: ;
        }
        R32(rec, 136) = ReadDeltaValue(ch, seq);
        R8(rec, 41) = 0;
        return;
    }

    if (R8(rec, 42) == 2) {
        if (R8(rec, 22) == 16) {
            for (i = 0; i < count; i++) {
                Snd_setVabAttr(R16(rec, 74), sub[44], i, scratch, other, R8(rec, 21), val);
            }
            R32(rec, 136) = ReadDeltaValue(ch, seq);
        } else {
            Snd_setVabAttr(R16(rec, 74), sub[44], (s16)R8(rec, 22), scratch, other, R8(rec, 21), val);
            R32(rec, 136) = ReadDeltaValue(ch, seq);
        }
        R8(rec, 42) = 0;
    } else {
        R32(rec, 136) = ReadDeltaValue(ch, seq);
    }
}
