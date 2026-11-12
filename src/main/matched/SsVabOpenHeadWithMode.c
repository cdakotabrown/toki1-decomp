/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: remap_stack_slots --keep-frame --map 20:28,1104:48,52:1104,63:1096,1092:56 */
/* @CompileFlags: -cckr -Wo,-loopunroll,0 -Wc,-pic1 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_DIV_POW2=aspsx IDO52_NO_CSE_SITE=mpy:1,mpy:2,mpy:5,mpy:6,mpy:9 IDO52_FORCE_WEB_COLOR_NOSPLIT=8 IDO52_FORCE_WEB_COLOR=48:t2,8:t4,14:a0,109:t0,125:v1 IDO52_CFE_FRAME_TOP_BASE=all IDO52_AS1_NO_COPY_PROP=0,8 IDO52_AS1_KEEP_SEXT=0 IDO52_AS1_NO_PASS1_PROP=10,148 IDO52_AS1_NO_JUMP_CHAIN=6 IDO52_NO_LOOP_TEST_REPLACEMENT=occ:1 IDO52_COPYPROP_VAR=-12,-1064,-8 IDO52_COPYPROP_SITE=lod:4:deny,lod:6:deny IDO52_AS1_TIE_TAKE=15,35,38,63,67,209,214,218,221 IDO52_AS1_TIE_REJECT=6,9,10,11,16,17,163 */
/* Function: SsVabOpenHeadWithMode */
/* Address: 0x8008C114 */
/* Size: 1236 bytes (309 instructions) */
/* Region: Library (0x80086000+) */
#include "types.h"

typedef struct {
    s32 form;
    s32 ver;
    s32 id;
    u32 fsize;
    u16 reserved0;
    u16 ps;
    u16 ts;
    u16 vs;
    u8 mvol;
    u8 pan;
    u8 attr1;
    u8 attr2;
    u32 reserved1;
} VabHdr;

typedef struct {
    u8 tones;
    u8 mvol;
    u8 prior;
    u8 mode;
    u8 mpan;
    s8 reserved0;
    s16 attr;
    u32 reserved1;
    u16 reserved2;
    u16 reserved3;
} ProgAtr;

extern u8 _svm_vab_used[16];
extern s16 _svm_vab_count;
extern s16 kMaxPrograms;
extern s32 _svm_vab_vh[16];
extern s32 _svm_vab_pg[16];
extern s32 _svm_vab_tn[16];
extern s32 _svm_brr_start_addr[16];
extern s32 _svm_vab_start[16];
extern s32 _svm_vab_total[16];

s32 _spu_getInTransfer();
void _spu_setInTransfer();
s32 SpuMalloc();

short SsVabOpenHeadWithMode(addr, vabid, sticky, sbaddr)
u8 *addr;
short vabid;
short sticky;
long sbaddr;
{
    VabHdr *hdr;
    ProgAtr *prog;
    u16 *vag;
    unsigned long spu;
    unsigned long total;
    int i;
    long buf[256];
    u8 numProg;
    u8 *flags;
    u16 val;
    short ch;
    int nv;

    ch = 16;
    if (_spu_getInTransfer() == 1) {
        return -1;
    }
    _spu_setInTransfer(1);
    if (vabid >= 16) {
        _spu_setInTransfer(0);
        return -1;
    }
    if (vabid == -1) {
        for (i = 0; i < 16; i++) {
            if (_svm_vab_used[i] == 0) {
                _svm_vab_used[i] = 1;
                _svm_vab_count++;
                ch = i;
                break;
            }
        }
        flags = _svm_vab_used;
    } else {
        flags = _svm_vab_used;
        if (_svm_vab_used[vabid] == 0) {
            _svm_vab_used[vabid] = 1;
            _svm_vab_count++;
            ch = vabid;
        }
    }
    if (ch >= 16) {
        flags[vabid] = 0;
        _spu_setInTransfer(0);
        _svm_vab_count--;
        return -1;
    }
    _svm_vab_vh[ch] = (s32)addr;
    hdr = (VabHdr *)addr;
    if (((u32)hdr->form >> 8) != 0x564142) {
        _svm_vab_used[vabid] = 0;
        _spu_setInTransfer(0);
        _svm_vab_count--;
        return -1;
    }
    if ((hdr->form & 0xFF) != 'p' || hdr->ver < 5) {
        kMaxPrograms = 64;
    } else {
        kMaxPrograms = 128;
    }
    nv = kMaxPrograms;
    if (nv < hdr->ps) {
        _svm_vab_used[vabid] = 0;
        _spu_setInTransfer(0);
        _svm_vab_count--;
        return -1;
    }
    prog = (ProgAtr *)(addr + 32);
    _svm_vab_pg[ch] = (s32)prog;
    vag = (u16 *)((u8 *)prog + (nv << 4));
    total = 0;
    for (i = 0; i < kMaxPrograms; i++) {
        prog[i].reserved1 = total;
        if (prog[i].tones != 0) {
            total++;
        }
    }
    i = 0;
    total = 0;
    _svm_vab_tn[ch] = (s32)vag;
    numProg = hdr->vs;
    vag = (u16 *)((u8 *)vag + (hdr->ps << 9));
    do {
        if (i <= numProg) {
            val = *vag;
            if (hdr->ver < 5) {
                buf[i] = val << 2;
            } else {
                buf[i] = val << 3;
            }
            total += buf[i];
        }
        i++;
        vag++;
    } while (i != 256);
    _svm_brr_start_addr[ch] = (s32)vag;
    if (sticky == 0) {
        spu = SpuMalloc(total);
        if (spu == -1) {
            _svm_vab_used[vabid] = 0;
            _spu_setInTransfer(0);
            _svm_vab_count--;
            return -1;
        }
    } else {
        spu = sbaddr;
    }
    if (spu + total > 0x80000) {
        _svm_vab_used[vabid] = 0;
        _spu_setInTransfer(0);
        _svm_vab_count--;
        return -1;
    }
    _svm_vab_start[ch] = spu;
    total = 0;
    i = 0;
    do {
        total += buf[i];
        if (!(i % 2)) {
            prog[i / 2].reserved2 = (spu + total) >> 3;
        } else {
            prog[i / 2].reserved3 = (spu + total) >> 3;
        }
        i++;
    } while (i <= numProg);
    _svm_vab_total[ch] = total;
    _svm_vab_used[ch] = 2;
    return ch;
}
