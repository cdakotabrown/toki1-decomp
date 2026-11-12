/* Address: 0x80091160 */
/* Size: 1040 bytes (260 instructions) */
/* Region: Library */
/* @CompileFlags: -Wo,-varref,1000 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=60 IDO52_FORCE_WEB_COLOR=2:t3 IDO52_FORCE_WEB_COLOR_NOSPLIT=2 IDO52_AS1_FORCE_BOPT=2 IDO52_AS1_NO_FALLTHRU_FILL=3 IDO52_UGEN_CMPBR_RIGHT_FIRST=occ:1 IDO52_UGEN_TEMP_REG=25:t6,40:v0,41:t4,42:t9 IDO52_UGEN_TEMP_REG_INPLACE=occ:40 IDO52_AS1_TIE_REJECT=136,206 IDO52_AS1_TIE_TAKE=145,277 IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_DIV_MACRO=aspsx:0,3,4,5 IDO52_AS1_DIV_MACRO_MFFIRST_FLUSH=occ:0 IDO52_AS1_DIV_MACRO_MUL_PASS=all IDO52_AS1_DIV_MACRO_QUEUE=all IDO52_AS1_DIV_MACRO_SPLIT_CMPBR=all IDO52_AS1_MUL_CONST=aspsx IDO52_AS1_DIV_POW2=aspsx IDO52_AS1_NO_PASS1_PROP=all IDO52_AS1_KEEP_SEXT=all */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
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
} SeqAttr;

extern SeqAttr *_ss_score[];
extern u32 VBLANK_MINUS;
extern u8 libsnd_not_seq_data_msg[];
extern u8 libsnd_old_seq_format_msg[];

extern s32 ReadDeltaValue();
extern void printf();

void InitSoundSeq(arg0, arg1, arg2)
    s16 arg0;
    s16 arg1;
    u8 *arg2;
{
    s32 pad[11];
    SeqAttr *vs;
    s32 i;
    u32 a;
    u32 ver;
    u32 b;
    u32 c;
    u32 d;
    u32 e;
    s32 r;
    s32 t;
    u32 prod;

    vs = _ss_score[arg0];
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
    vs->h4A = arg1;
    ((volatile SeqAttr *)vs)->b10 = 0;
    ((volatile SeqAttr *)vs)->b11 = 0;
    for (i = 0; i < 16; i++) {
        vs->prog[i] = i;
        vs->pan[i] = 0x40;
        vs->vol[i] = 0x7F;
    }
    vs->h6C = 1;
    vs->h76 = 0x7F;
    vs->h78 = 0x7F;
    vs->ptr = arg2;
    if (*arg2 == 'S' || *arg2 == 'p') {
        vs->ptr += 7;
        ver = *vs->ptr++;
        if (ver != 1) {
            printf(libsnd_not_seq_data_msg);
            return;
        }
    } else {
        printf(libsnd_old_seq_format_msg);
    }
    a = *vs->ptr++;
    b = *vs->ptr++;
    vs->division = (a << 8) | b;
    c = *vs->ptr++;
    d = *vs->ptr++;
    e = *vs->ptr++;
    t = (c << 16) | (d << 8) | e;
    vs->tempo = t;
    if (60000000 % t > t / 2) {
        vs->tempo = 60000000 / vs->tempo + 1;
    } else {
        vs->tempo = 60000000 / vs->tempo;
    }
    vs->w8C = vs->tempo;
    vs->ptr += 2;
    r = ReadDeltaValue(arg0, 0);
    vs->w7C = r;
    vs->w88 = r;
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
}
