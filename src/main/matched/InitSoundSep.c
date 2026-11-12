/* Address: 0x800915A8 */
/* Size: 1080 bytes (recorded) */
/* Region: Library (0x80086000+), IDO 5.2 (native as1 standing in for ASPSX) */
/* @SymbolStart: 0x80091570 */
/* @SymbolSize: 1132 */
/* @CompileFlags: -Wc,-pic1 -Wo,-varref,1000 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=60 IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_DIV_MACRO=aspsx:0,3,4,5 IDO52_AS1_DIV_MACRO_MFFIRST_FLUSH=occ:0 IDO52_AS1_DIV_MACRO_MUL_PASS=all IDO52_AS1_DIV_MACRO_QUEUE=all IDO52_AS1_DIV_MACRO_SPLIT_CMPBR=all IDO52_UGEN_CMPBR_RIGHT_FIRST=occ:1 IDO52_AS1_MUL_CONST=aspsx IDO52_AS1_DIV_POW2=aspsx IDO52_AS1_NO_PASS1_PROP=all IDO52_AS1_KEEP_SEXT=all IDO52_FORCE_WEB_COLOR=2:t5,7:t4,66:a3,72:t0,83:t1,86:t3 IDO52_FORCE_WEB_COLOR_NOSPLIT=2,7,114,117,120,123 IDO52_UGEN_TEMP_REG=28:t9,31:t9,32:t6,35:t8,49:v0,50:t7,54:t9 IDO52_UGEN_TEMP_REG_INPLACE=occ:49 IDO52_AS1_TIE_REJECT=295,343,528,531,533 IDO52_AS1_TIE_TAKE=342 IDO52_AS1_NO_FALLTHRU_FILL=1 IDO52_AS1_NO_ADDIU_COMBINE=all */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --keep-frame --map 76:24,68:28,47:72,46:68,45:64,44:60 */
#include "types.h"

typedef struct {
    u8 pad0[4];
    u8 *ptr;
    u8 *ptr2;
    u8 *ptr3;
    u8 b10;
    u8 b11;
    u8 b12;
    u8 b13;
    u8 b14;
    u8 b15;
    u8 b16;
    u8 pan[16];
    u8 b27;
    u8 b28;
    u8 b29;
    u8 b2A;
    u8 b2B;
    u8 prog[16];
    u8 pad3C[10];
    s16 h46;
    s16 division;
    s16 h4A;
    s16 vol[16];
    s16 h6C;
    s16 h6E;
    s16 h70;
    u8 pad72[4];
    u16 h76;
    u16 h78;
    u8 pad7A[2];
    s32 w7C;
    s32 w80;
    s32 tempo;
    s32 w88;
    s32 w8C;
    u8 pad90[24];
} SeqAttr;

extern SeqAttr *_ss_score[];
extern u32 VBLANK_MINUS;

extern s32 ReadDeltaValue();

s32 InitSoundSep(ch, vo, arg2, data)
    s16 ch;
    s16 vo;
    s16 arg2;
    u8 *data;
{
    SeqAttr *vs;
    s32 i;
    s32 n;
    u32 a;
    u32 b;
    u32 c;
    u32 d;
    u32 e;
    u8 c0;
    u8 c1;
    u8 c2;
    u8 c3;
    s32 t;
    u32 prod;

    vs = &_ss_score[ch][vo];
    ((volatile SeqAttr *)vs)->division = 0;
    ((volatile SeqAttr *)vs)->b13 = 0;
    ((volatile SeqAttr *)vs)->b14 = 0;
    ((volatile SeqAttr *)vs)->b29 = 0;
    ((volatile SeqAttr *)vs)->b15 = 0;
    ((volatile SeqAttr *)vs)->b16 = 0;
    ((volatile SeqAttr *)vs)->b2A = 0;
    ((volatile SeqAttr *)vs)->b12 = 0;
    ((volatile SeqAttr *)vs)->w7C = 0;
    ((volatile SeqAttr *)vs)->w80 = 0;
    ((volatile SeqAttr *)vs)->tempo = 0;
    ((volatile SeqAttr *)vs)->h70 = 0;
    ((volatile SeqAttr *)vs)->h46 = 0;
    ((volatile SeqAttr *)vs)->b2B = 0;
    ((volatile SeqAttr *)vs)->w88 = 0;
    ((volatile SeqAttr *)vs)->b27 = 0;
    ((volatile SeqAttr *)vs)->b28 = 0;
    ((volatile SeqAttr *)vs)->b10 = 0;
    ((volatile SeqAttr *)vs)->b11 = 0;
    ((volatile SeqAttr *)vs)->h4A = arg2;
    for (i = 0; i < 16; i++) {
        vs->prog[i] = i;
        vs->pan[i] = 0x40;
        vs->vol[i] = 0x7F;
    }
    vs->ptr = data;
    vs->h6C = 1;
    vs->h76 = 0x7F;
    vs->h78 = 0x7F;
    if (vo == 0) {
        vs->ptr += 8;
        n = 8;
    } else {
        vs->ptr += 2;
        n = 2;
    }
    a = *vs->ptr++;
    b = *vs->ptr++;
    vs->division = ((s16)a << 8) | b;
    n += 2;
    c = *vs->ptr++;
    d = *vs->ptr++;
    e = *vs->ptr++;
    n += 3;
    t = (c << 16) | (d << 8) | e;
    vs->tempo = t;
    if (60000000 % t > t / 2) {
        vs->tempo = 60000000 / vs->tempo + 1;
    } else {
        vs->tempo = 60000000 / vs->tempo;
    }
    vs->ptr += 2;
    vs->w8C = vs->tempo;
    n += 2;
    c0 = *vs->ptr++;
    c1 = *vs->ptr++;
    c2 = *vs->ptr++;
    c3 = *vs->ptr++;
    n += 4;
    vs->w7C = ReadDeltaValue(ch, vo);
    vs->w88 = vs->w7C;
    vs->ptr2 = vs->ptr;
    vs->ptr3 = vs->ptr;
    prod = vs->division * vs->tempo;
    if (prod * 10 < VBLANK_MINUS * 60) {
        vs->h6C = (VBLANK_MINUS * 600) / prod;
        vs->h6E = vs->h6C;
    } else {
        vs->h6C = -1;
        vs->h6E = (prod * 10) / (VBLANK_MINUS * 60);
        if ((prod * 10) % (VBLANK_MINUS * 60) > (VBLANK_MINUS * 60) >> 1) {
            vs->h6E++;
        }
    }
    vs->h70 = vs->h6E;
    t = (c0 << 24) + (c1 << 16) + (c2 << 8) + c3;
    return n + t;
}
