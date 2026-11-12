/* Function: SpuVmAlloc */
/* Address: 0x8008C810 */
/* Size: 408 bytes (102 instructions)  [TRUE extent: 0x8008C800..0x8008C9A8, */
/* Region: Library (0x80086000+) */
/* @SymbolStart: 0x8008C800 */
/* @SymbolSize: 424 */
/* @CompileFlags: -Wc,-pic0 -cckr */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=24 IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_XBB_SKIP_CAND=2,3 IDO52_AS1_FTF_BACKWARD=occ:0 IDO52_FORCE_WEB_COLOR=47:t5 IDO52_AS1_TIE_REJECT=16,45,47,48 IDO52_AS1_TIE_TAKE=61 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    s16 unk0;
    s16 ctr;
    s16 unk4;
    u16 f6;
    u8 unk8[12];
    s16 pri;
    s8 unk16;
    s8 busy;
} Slot;

extern Slot _svm_voice[];
extern s8 spuVmMaxVoice;
extern s8 libsnd_cur_priority;

u8 SpuVmAlloc(a0)
    s32 a0;
{
    u8 result;
    u16 bestF6;
    u8 matchCnt;
    u16 bestCtr;
    u8 bestSlot;
    u8 i;
    u16 priVal;
    s32 tmp;
    Slot *base;
    u8 n;

    bestF6 = 0xFFFF;
    result = 99;
    a0 = 0;
    n = 0;
    priVal = libsnd_cur_priority;
    matchCnt = 0;
    bestCtr = 0;
    bestSlot = 99;

    i = 0;
    if (i >= spuVmMaxVoice) {
        base = _svm_voice;
        goto done;
    }
    base = _svm_voice;
    do {
        if (base[i].busy == 0 && base[i].f6 == 0) {
            result = i;
            break;
        }
        tmp = base[i].pri;
        if (tmp < priVal) {
            bestF6 = base[i].f6;
            bestCtr = base[i].ctr;
            priVal = tmp;
            bestSlot = i;
            n = 1;
            matchCnt = 1;
        } else if (priVal == tmp) {
            matchCnt++;
            tmp = base[i].f6;
            if (tmp < bestF6) {
                bestCtr = base[i].ctr;
                bestF6 = tmp;
                bestSlot = i;
                n = 1;
            } else if (bestF6 == tmp) {
                tmp = base[i].ctr;
                n++;
                if (bestCtr < tmp) {
                    bestCtr = tmp;
                    bestSlot = i;
                }
            }
        }
        i++;
    } while (i < spuVmMaxVoice);

done:
    if (result == 99) {
        if (!matchCnt) {
            result = spuVmMaxVoice;
        } else {
            result = bestSlot;
        }
    }
    if (result < spuVmMaxVoice) {
        for (i = 0; i < spuVmMaxVoice; i++) {
            _svm_voice[i].ctr++;
        }
        base[result].ctr = 0;
        base[result].pri = libsnd_cur_priority;
    }
    return result;
}
