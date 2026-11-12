/* Function: SetControlChange */
/* Address: 0x80092F8C (recorded symbol 0x80092FBC is the prologue; the */
/* Size: 1028 bytes (257 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 (native as1, K&R, -pic1 like its */
/* @SymbolStart: 0x80092F8C */
/* @SymbolSize: 1028 */
/* @CompileFlags: -cckr -Wc,-pic1 */
/* @CompileEnv: IDO52_CFE_SWITCH_CHAIN=0 IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_FORCE_WEB_COLOR=2:t1,7:t2,17:v1 IDO52_FORCE_WEB_COLOR_NOSPLIT=2,7 IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_FTF_BACKWARD=occ:0,1,2,5,8,9,12 IDO52_AS1_NO_COPY_PROP=all IDO52_AS1_KEEP_DEAD_RELOAD=all IDO52_AS1_TIE_TAKE=172,175 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: remap_stack_slots --keep-frame --map 44:28,42:40,41:44 */
#include "types.h"

typedef struct {
    u8  pad0[4];
    u8 *rd;
    u8  pad8[10];
    u8  chan;
    u8  pad19[4];
    u8  pan[21];
    u8  prog[21];
    u8  pad65[9];
    s16 bend;
    s16 vol[21];
    u8  pad118[18];
    s32 slot;
    u8  pad140[28];
} SeqRec;

extern SeqRec *_ss_score[];

extern s32  ReadDeltaValue();
extern void ContModulation();
extern void ContPortaTime();
extern void ContDataEntry();
extern void ContPortamento();
extern void ContNrpn1();
extern void ContNrpn2();
extern void ContRpn1();
extern void ContRpn2();
extern void ContResetAll();
extern s32  SpuVmGetProgVol();
extern void SsUtSetReverbDepth();
extern void SpuVmDamperOff();
extern void SpuVmDamperOn();

void SetControlChange(ch, seq, ctl)
short ch;
short seq;
u8 ctl;
{
    SeqRec *rec;
    short idx;
    u8 d;
    short t;

    rec = &_ss_score[ch][seq];
    idx = rec->chan;
    d = *rec->rd++;
    switch (ctl) {
    case 0:
        rec->bend = d;
        rec->slot = ReadDeltaValue(ch, seq);
        break;
    case 1:
        ContModulation(ch, seq, d);
        break;
    case 5:
        ContPortaTime(ch, seq, d);
        break;
    case 6:
        ContDataEntry(ch, seq, d);
        break;
    case 7:
        rec->vol[idx] = d;
        rec->slot = ReadDeltaValue(ch, seq);
        break;
    case 10:
        rec->pan[idx] = d;
        rec->slot = ReadDeltaValue(ch, seq);
        break;
    case 11:
        t = SpuVmGetProgVol(rec->bend, rec->prog[idx]);
        rec->vol[idx] = (t * d) / 127;
        rec->slot = ReadDeltaValue(ch, seq);
        break;
    case 64:
        if (d < 64) {
            SpuVmDamperOff();
        } else {
            SpuVmDamperOn();
        }
        rec->slot = ReadDeltaValue(ch, seq);
        break;
    case 65:
        ContPortamento(ch, seq, d);
        break;
    case 91:
        SsUtSetReverbDepth((short)d, (short)d);
        rec->slot = ReadDeltaValue(ch, seq);
        break;
    case 98:
        ContNrpn1(ch, seq, d);
        break;
    case 99:
        ContNrpn2(ch, seq, d);
        break;
    case 100:
        ContRpn1(ch, seq, d);
        break;
    case 101:
        ContRpn2(ch, seq, d);
        break;
    case 121:
        ContResetAll(ch, seq, d);
        break;
    default:
        rec->slot = ReadDeltaValue(ch, seq);
        break;
    }
}
