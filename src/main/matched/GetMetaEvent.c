/* Address: 0x80094648 (recorded 0x80094660 is the PROLOGUE; the two s16 */
/* Size: 980 bytes (245 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 -O2 (pic0) */
/* @SymbolStart: 0x80094648 */
/* @SymbolSize: 980 */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=10,60,168,600 IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_NO_CSE_SITE=mpy:5,mpy:7,mpy:9,mpy:11,mpy:13 IDO52_AS1_DIV_MACRO=aspsx IDO52_AS1_DIV_MACRO_MUL_PASS=all IDO52_AS1_DIV_MACRO_QUEUE=all IDO52_AS1_DIV_MACRO_SPLIT_CMPBR=all IDO52_AS1_NO_PASS1_PROP=all IDO52_UGEN_TEMP_REG=24:v1,28:v0,29:t5,30:t4,36:t4 IDO52_UGEN_TEMP_REG_INPLACE=occ:24,28 IDO52_AS1_MUL_CONST=aspsx IDO52_AS1_XBB_SKIP_CAND=1 IDO52_AS1_FTF_BACKWARD=occ:0 IDO52_AS1_TIE_TAKE=231,234 IDO52_FORCE_WEB_COLOR_NOSPLIT=3,9 IDO52_UGEN_NO_REGION_MOVE=all IDO52_FORCE_WEB_COLOR=3:t2,9:t1,0:a3,7:v1,14:t0,18:a0,25:a1,28:a2 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
      s8 vab_id;
      u8 pad01[3];
      u8 *data;
      u8 *loop_start;
      u8 *loop_ptr;
      u8 pad10[23];
      u8 f39;
      u8 pad28[3];
      u8 f43;
      u8 pad2C[24];
      s16 l_count;
      s16 l_current;
      s16 resolution;
      u8 pad4A[34];
      s16 f108;
      s16 f110;
      u8 pad70[16];
      s32 f128;
      s32 f132;
      s32 f136;
      s32 tempo;
      s32 flags;
      u8 pad94[20];
} SeqStruct;

extern SeqStruct *_ss_score[];
extern s32 VBLANK_MINUS;

extern s32 ReadDeltaValue();
extern void Snd_nextseq();
extern void SpuVmSeqKeyOff();

void GetMetaEvent(s16 sep_no, s16 seq_no, u8 cmd)
{
    s32 unused[4];
    SeqStruct *ch;
    u8 b0, b1, b2;
    s32 tempo;
    s16 res;

    ch = *(SeqStruct **)((s32)_ss_score + sep_no * 4) + seq_no;

    goto dispatch;

tempo_evt:
    res = ch->resolution;
    b0 = *ch->data++;
    b1 = *ch->data++;
    b2 = *ch->data++;
    tempo = 60000000 / (s32)(((u32)b0 << 16) | ((u32)b1 << 8) | b2);
    ch->tempo = tempo;
    if ((u32)(res * tempo * 10) < (u32)(VBLANK_MINUS * 60)) {
        ch->f108 = (u32)(VBLANK_MINUS * 600) / (u32)(res * ch->tempo);
        ch->f110 = ch->f108;
    } else {
        ch->f108 = -1;
        ch->f110 = (u32)(res * ch->tempo * 10) / (u32)(VBLANK_MINUS * 60);
        if ((u32)(VBLANK_MINUS * 60) / 2 < (u32)(res * ch->tempo * 10) % (u32)(VBLANK_MINUS * 60)) {
            ch->f110++;
        }
    }
    ch->f136 = ReadDeltaValue(sep_no, seq_no);
    goto end;

eot_evt:
    ch->l_current++;
    if (ch->l_count == 0) {
        ch->f128 = 0;
        ch->f39 = 0;
        ch->f136 = 0;
        ch->data = ch->loop_start;
    } else if (ch->l_count != 0) {
        if (ch->l_current < ch->l_count) {
            ch->f128 = 0;
            ch->f39 = 0;
            ch->f136 = 0;
            ch->data = ch->loop_start;
            ch->loop_ptr = ch->loop_start;
        } else {
            (*(SeqStruct **)((s32)_ss_score + sep_no * 4))[seq_no].flags &= ~1;
            goto F1; F1:
            (*(SeqStruct **)((s32)_ss_score + sep_no * 4))[seq_no].flags &= ~8;
            goto F2; F2:
            (*(SeqStruct **)((s32)_ss_score + sep_no * 4))[seq_no].flags &= ~2;
            goto F3; F3:
            (*(SeqStruct **)((s32)_ss_score + sep_no * 4))[seq_no].flags |= 0x200;
            goto F4; F4:
            (*(SeqStruct **)((s32)_ss_score + sep_no * 4))[seq_no].flags |= 4;
            ch->loop_ptr = ch->loop_start;
            ch->f43 = 0;
            if (ch->vab_id != -1) {
                Snd_nextseq(ch->vab_id, seq_no);
            }
            SpuVmSeqKeyOff((seq_no << 8) | sep_no, seq_no);
            ch->f136 = ch->f110;
        }
    }
    goto end;

dispatch:
    switch ((u32)cmd) {
    case 0x2F: goto eot_evt;
    case 0x51: goto tempo_evt;
    }
end:
    ;
}
