/* Address: 0x80092BAC (recorded symbol 0x80092BDC is the prologue; the */
/* Size: 652 bytes (163 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 (native as1, K&R, -pic1 like its */
/* @SymbolStart: 0x80092BAC */
/* @SymbolSize: 652 */
/* @CompileFlags: -cckr -Wc,-pic1 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_UGEN_NO_REGION_MOVE=all IDO52_AS1_NO_JUMP_INVERT=all IDO52_AS1_KEEP_ANDI=1 IDO52_FORCE_WEB_COLOR=0:t0,50:v1 IDO52_AS1_NO_PASS1_PROP=45 IDO52_AS1_NO_COPY_PROP=0,1,2,3 IDO52_AS1_XBB_SKIP_CAND=0,2,4,5,6,9 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: remap_stack_slots --keep-frame --map 20:28,44:32,42:44,41:40,43:60,48:64,52:68 */
/* @PostProc: patch_stack_complete --old 48 --new 64 */
#include "types.h"

typedef struct {
    u8  pad0[4];
    u8 *rd;
    u8  pad8[9];
    u8  status;
    u8  chan;
    u8  pad19[117];
    s32 slot;
    u8  pad140[28];
} SeqRec;

extern SeqRec *_ss_score[];

extern s32  ReadDeltaValue();
extern void NoteOn();
extern void SetControlChange();
extern void SetProgramChange();
extern void SetPitchBend();
extern void GetMetaEvent();

void GetSeqData(ch, seq)
short ch;
short seq;
{
    SeqRec *rec;
    u8 ev;
    u8 d1;
    u8 d2;
    u8 hi;
    u8 cur;
    u8 st;

    rec = &_ss_score[ch][seq];
    ev = *rec->rd++;
    if (ev & 0x80) {
        hi = ev & 0xF0;
        rec->chan = ev & 0x0F;
        goto sw1;
    sw1:
        switch (hi) {
        case 0x90:
            rec->status = 0x90;
            d1 = *rec->rd++;
            d2 = *rec->rd++;
            rec->slot = ReadDeltaValue(ch, seq);
            NoteOn(ch, seq, d1, d2);
            break;
        case 0xB0:
            rec->status = 0xB0;
            cur = *rec->rd++;
            SetControlChange(ch, seq, cur);
            break;
        case 0xC0:
            rec->status = 0xC0;
            cur = *rec->rd++;
            SetProgramChange(ch, seq, cur);
            break;
        case 0xE0:
            rec->status = 0xE0;
            cur = *rec->rd++;
            SetPitchBend(ch, seq, cur);
            break;
        case 0xF0:
            rec->status = 0xFF;
            cur = *rec->rd++;
            GetMetaEvent(ch, seq, cur);
            break;
        }
    } else {
        st = rec->status;
        goto test;
    c90:
        d2 = *rec->rd++;
        rec->slot = ReadDeltaValue(ch, seq);
        NoteOn(ch, seq, ev, d2);
        goto end;
    cB0:
        SetControlChange(ch, seq, ev);
        goto end;
    cC0:
        SetProgramChange(ch, seq, ev);
        goto end;
    cE0:
        SetPitchBend(ch, seq, ev);
        goto end;
    cFF:
        GetMetaEvent(ch, seq, ev);
        goto end;
    test:
        switch (st) {
        case 0x90: goto c90;
        case 0xB0: goto cB0;
        case 0xC0: goto cC0;
        case 0xE0: goto cE0;
        case 0xFF: goto cFF;
        }
    }
end:;
}
