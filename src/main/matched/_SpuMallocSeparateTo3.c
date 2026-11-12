/* Address: 0x80097D74 */
/* Size: 848 bytes */
/* Region: Library (0x80086000+), IDO 5.2 -O2 */
/* appears under ido52-gas).  Under `@Compiler: ido52-gas` with -Wc,-pic2 */
/* @SymbolStart: 0x80097D74 */
/* @SymbolSize: 848 */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_NO_FALLTHRU_FILL=1,2,3 IDO52_AS1_KEEP_DEAD_RELOAD=0,1,2 IDO52_AS1_NO_PASS1_PROP=64 IDO52_FORCE_WEB_COLOR=0:t3,7:t1,43:t0 IDO52_AS1_TIE_TAKE=111,240,120,207,210 IDO52_AS1_TIE_REJECT=250 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 24 --new 32 */
/* @PostProc: remap_stack_slots --map 20:28,28:36 */
/* @PostProc: swap_instructions 0x308 0x314 */
/* @PostProc: swap_instructions 0x314 0x320 */
#include "types.h"

typedef struct {
    u32 addr;
    u32 size;
} SpuMemEntry;

extern s32 _spu_rev_reserve_wa;
extern s32 _spu_rev_offsetaddr;
extern s32 spu_mem_mode;
static s32 _spu_AllocBlockNum;
static s32 _spu_AllocLastNum;
static SpuMemEntry *_SpuMemList;

extern void _gcSPU(void);

s32 _SpuMallocSeparateTo3(s32 idx, s32 addr, s32 req)
{
    s32 tag;
    s32 size;
    s32 adjust;
    s32 tmask;
    s32 off;
    s32 rest;
    s32 avail;

    tag = _SpuMemList[idx].addr;
    size = _SpuMemList[idx].size;

    if (_spu_rev_reserve_wa == 0) {
        adjust = 0;
    } else {
        adjust = (0x10000 - _spu_rev_offsetaddr) << spu_mem_mode;
    }

    if (tag & 0x40000000) {
        tmask = tag & 0x0FFFFFFF;
        off = addr - tmask;
        rest = size - off;
        avail = rest - adjust;
    } else {
        tmask = tag & 0x0FFFFFFF;
        off = addr - tmask;
        rest = size - off;
        avail = rest;
    }

    if (avail < req) {
        return -1;
    }

    if (tag & 0x40000000) {
        if (off > 0) {
            if (_spu_AllocBlockNum - 2 < _spu_AllocLastNum) {
                return -1;
            }
            _SpuMemList[_spu_AllocLastNum].addr = _SpuMemList[_spu_AllocLastNum].addr & 0x0FFFFFFF;
            _SpuMemList[_spu_AllocLastNum].addr = _SpuMemList[_spu_AllocLastNum].addr | 0x80000000;
            _SpuMemList[_spu_AllocLastNum].size = off;
            _spu_AllocLastNum++;
            _SpuMemList[_spu_AllocLastNum].addr = addr;
            _SpuMemList[_spu_AllocLastNum].size = req;
        } else {
            if (!(_spu_AllocLastNum < _spu_AllocBlockNum)) {
                return -1;
            }
            _SpuMemList[_spu_AllocLastNum].addr = _SpuMemList[_spu_AllocLastNum].addr & 0x0FFFFFFF;
            _SpuMemList[_spu_AllocLastNum].size = req;
        }
        _SpuMemList[_spu_AllocLastNum + 1].addr = (tmask + off + req) | 0x40000000;
        _SpuMemList[_spu_AllocLastNum + 1].size = rest - req;
        _spu_AllocLastNum++;
    } else {
        if (off > 0) {
            if (avail == req) {
                if (_spu_AllocBlockNum - 2 < _spu_AllocLastNum) {
                    return -1;
                }
            } else {
                if (!(_spu_AllocLastNum < _spu_AllocBlockNum)) {
                    return -1;
                }
            }
            _SpuMemList[idx].size = off;
            _SpuMemList[_spu_AllocLastNum + 1].addr = _SpuMemList[_spu_AllocLastNum].addr;
            _SpuMemList[_spu_AllocLastNum + 1].size = _SpuMemList[_spu_AllocLastNum].size;
            _SpuMemList[_spu_AllocLastNum].addr = addr;
            _SpuMemList[_spu_AllocLastNum].size = req;
            _spu_AllocLastNum++;
            if (avail < req) {
            } else {
                _SpuMemList[_spu_AllocLastNum + 1].addr = _SpuMemList[_spu_AllocLastNum].addr;
                _SpuMemList[_spu_AllocLastNum + 1].size = _SpuMemList[_spu_AllocLastNum].size;
                _SpuMemList[_spu_AllocLastNum].addr = (tmask + off + req) | 0x80000000;
                _SpuMemList[_spu_AllocLastNum].size = avail - req;
                _spu_AllocLastNum++;
            }
        } else {
            if (req >= avail) {
            } else {
                if (!(_spu_AllocLastNum < _spu_AllocBlockNum)) {
                    return -1;
                }
            }
            _SpuMemList[idx].addr = _SpuMemList[idx].addr & 0x0FFFFFFF;
            _SpuMemList[idx].size = req;
            if (req < avail) {
                _SpuMemList[_spu_AllocLastNum + 1].addr = _SpuMemList[_spu_AllocLastNum].addr;
                _SpuMemList[_spu_AllocLastNum + 1].size = _SpuMemList[_spu_AllocLastNum].size;
                _SpuMemList[_spu_AllocLastNum].addr = (tmask + req) | 0x80000000;
                _SpuMemList[_spu_AllocLastNum].size = avail - req;
                _spu_AllocLastNum++;
            }
        }
    }

    _gcSPU();
    return addr;
}
