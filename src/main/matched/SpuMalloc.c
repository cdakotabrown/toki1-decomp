/* @SymbolStart: 0x80097AA0 */
/* @SymbolSize: 724 */
/* Function: SpuMalloc */
/* Address: 0x80097AB4 */
/* Size: 704 bytes (176 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @CompileFlags: -cckr -Wc,-pic0 */
/* @CompileEnv: IDO52_AS1_NO_BRANCH_FOLD=all IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_NO_FALLTHRU_FILL=0,4,5,6 IDO52_AS1_KEEP_DEAD_RELOAD=0 IDO52_AS1_NO_COPY_PROP=0 IDO52_FORCE_WEB_COLOR=36:t0,54:t2,82:t1,81:a1,121:a0 IDO52_AS1_TIE_TAKE=150,169 IDO52_AS1_TIE_REJECT=152 */
#include "types.h"

extern s32 _spu_rev_reserve_wa;
extern s32 _spu_rev_offsetaddr;
static s32 _spu_AllocBlockNum;
static s32 _spu_AllocLastNum;
static s32 spu_mem_mode;
typedef struct {
    u32 addr;
    u32 size;
} SpuMemEntry;

static SpuMemEntry *_SpuMemList;

extern s32 _gcSPU(void);

#define BLK(x) ((SpuMemEntry *)((s32)_SpuMemList + ((x) << 3)))

s32 SpuMalloc(size)
s32 size;
{
    s32 i;
    s32 best;
    s32 shift;
    s32 unit;
    s32 mask;
    s32 extra;
    s32 nmask;
    s32 n;
    SpuMemEntry *q;
    u32 rest;

    shift = spu_mem_mode;
    unit = 1 << shift;
    i = 0;
    best = -1;
    mask = unit;
    if (_spu_rev_reserve_wa == 0) {
        extra = 0;
    } else {
        extra = (0x10000 - _spu_rev_offsetaddr) << shift;
    }
    mask--;
    nmask = ~mask;
    size += (size & nmask) ? mask : 0;
    size >>= shift;
    size <<= shift;
    if (_SpuMemList[0].addr & 0x40000000) {
        best = 0;
    } else {
        _gcSPU();
        if (i >= _spu_AllocBlockNum) {
            goto zt;
zt:         ;
        } else {
            do {
                if (!(_SpuMemList[i].addr & 0x40000000)) {
                    if ((_SpuMemList[i].addr & 0x80000000)
                        && _SpuMemList[i].size >= (u32)size) {
                        best = i;
                        break;
                    }
                } else {
                    best = i;
                    break;
                }
                i++;
            } while (i < _spu_AllocBlockNum);
        }
    }
    if (best == -1) {
        return -1;
    }
    if (BLK(best)->addr & 0x40000000) {
        if (best < _spu_AllocBlockNum) {
            if (BLK(best)->size - extra >= (u32)size) {
                _spu_AllocLastNum = best + 1;
                q = BLK(_spu_AllocLastNum);
                q->addr = ((BLK(best)->addr & 0x0FFFFFFF) + size)
                          | 0x40000000;
                q->size = BLK(best)->size - size;
                BLK(best)->size = size;
                BLK(best)->addr &= 0x0FFFFFFF;
                _gcSPU();
                return _SpuMemList[best].addr;
            }
            return -1;
        }
        return -1;
    }
    if (((u32)size < BLK(best)->size) && (_spu_AllocLastNum < _spu_AllocBlockNum)) {
        n = BLK(best)->addr + size;
        rest = BLK(best)->size - size;
        _SpuMemList[_spu_AllocLastNum + 1].addr = _SpuMemList[_spu_AllocLastNum].addr;
        _SpuMemList[_spu_AllocLastNum + 1].size = _SpuMemList[_spu_AllocLastNum].size;
        _SpuMemList[_spu_AllocLastNum].addr = n | 0x80000000;
        _SpuMemList[_spu_AllocLastNum].size = rest;
        _spu_AllocLastNum++;
    }
    BLK(best)->addr &= 0x0FFFFFFF;
    BLK(best)->size = size;
    _gcSPU();
    return _SpuMemList[best].addr;
}
