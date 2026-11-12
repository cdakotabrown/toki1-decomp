/* Address: 0x800980D8 (recorded; the prologue) */
/* Size: 816 bytes */
/* Region: Library (0x80086000+), IDO 5.2 -O2, native as1. */
/* @SymbolStart: 0x800980C4 */
/* @SymbolSize: 836 */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_AS1_TIE_TAKE=75,93,115 IDO52_AS1_TIE_REJECT=105,180 IDO52_AS1_TIE_SRC_ORDER=bb:13 IDO52_UGEN_TEMP_REG=18:t6,23:t9,24:t8,31:t7,33:t7 IDO52_PRECOLOR_WEB=v0:110 IDO52_FORCE_WEB_COLOR=27:t1,104:t0,117:a3 IDO52_AS1_NO_BRANCH_FOLD=all IDO52_AS1_NO_COPY_PROP=0,6,9,10,22 IDO52_AS1_KEEP_DEAD_RELOAD=0,1 IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_NO_FALLTHRU_FILL=all */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_insn_reg_field 0x26c:rd:t7:a0 0x270:rs:t7:a0 */
#include "types.h"

extern s32 _spu_rev_reserve_wa;
extern s32 _spu_rev_offsetaddr;
extern s32 _spu_AllocBlockNum;
extern s32 _spu_AllocLastNum;
extern s32 spu_mem_mode;
typedef struct {
    u32 addr;
    u32 size;
} SpuMemEntry;

static SpuMemEntry *_SpuMemList;

extern s32 _gcSPU(void);
extern s32 _SpuMallocSeparateTo3(s32 idx, s32 addr, s32 size);

s32 SpuMallocWithStartAddr(s32 addr, s32 size)
{
    u32 w;
    s32 base;
    s32 shift;
    s32 a;
    s32 ret;
    s32 avail;
    s32 reserve;
    s32 unit;
    s32 nmask;
    s32 len;
    s32 i;

    shift = spu_mem_mode;
    unit = 1 << shift;
    if (_spu_rev_reserve_wa == 0) {
        reserve = 0;
    } else {
        reserve = (0x10000 - _spu_rev_offsetaddr) << shift;
    }
    unit--;
    nmask = ~unit;
    size += (size & nmask) ? unit : 0;
    size >>= shift;
    size <<= shift;
    a = addr;
    a += (a & nmask) ? unit : 0;
    a >>= shift;
    a <<= shift;
    if (a < 0x1010) {
        return -1;
    }
    if ((0x10000 << shift) - reserve < a + size) {
        return -1;
    }
    if (_SpuMemList[0].addr & 0x40000000) {
        avail = _SpuMemList[0].size - a - size + 0x1010;
        if (a > 0x1010) {
            if (_spu_AllocBlockNum - 2 < _spu_AllocLastNum) {
                return -1;
            }
            _SpuMemList[1].addr = a;
            _SpuMemList[1].size = size;
            _SpuMemList[2].addr = (a + size) | 0x40000000;
            _SpuMemList[0].addr = 0x80001010;
            _SpuMemList[0].size = a - 0x1010;
            _SpuMemList[2].size = avail;
            _spu_AllocLastNum = 2;
        } else {
            if (_spu_AllocLastNum >= _spu_AllocBlockNum) {
                return -1;
            }
            _SpuMemList[0].addr = a;
            _SpuMemList[0].size = size;
            _SpuMemList[1].addr = (a + size) | 0x40000000;
            _SpuMemList[1].size = avail;
            _spu_AllocLastNum = 1;
        }
        _gcSPU();
        return a;
    }
    _gcSPU();
    i = 0;
    for (;;) {
        w = _SpuMemList[i].addr;
        if (w & 0x80000000) {
            base = w & 0x0FFFFFFF;
            if ((u32)a >= (u32)base) {
                len = _SpuMemList[i].size;
                if ((u32)a >= (u32)(base + len)
                    && !(_SpuMemList[i + 1].addr & 0x40000000)) {
                    i++;
                    continue;
                }
                avail = len - a + base;
                if (avail < size) {
                    i++;
                    continue;
                }
                ret = _SpuMallocSeparateTo3(i, a, size);
                break;
            }
            avail = _SpuMemList[i].size;
            if (avail < size) {
                i++;
                continue;
            }
            ret = _SpuMallocSeparateTo3(i, base, size);
            break;
        }
        if (!(w & 0x40000000)) {
            i++;
            continue;
        }
        avail = _SpuMemList[i].size - reserve;
        if (avail < size) {
            return -1;
        }
        ret = _SpuMallocSeparateTo3(i, w & 0x0FFFFFFF, size);
        break;
    }
    _gcSPU();
    return ret;
}
