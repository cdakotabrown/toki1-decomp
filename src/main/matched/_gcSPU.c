/* Address: 0x80097778 */
/* Size: 608 bytes (152 instructions) */
/* Region: Library (0x80086000+) */
/* `@Compiler: ido52-gas` + `--ido-fill-load-delay` takes the body from 169 */
/* @SymbolStart: 0x80097770 */
/* @SymbolSize: 708 */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_NO_FALLTHRU_FILL=all IDO52_NO_LOOP_TEST_REPLACEMENT=1 IDO52_AS1_TIE_TAKE=5,29 IDO52_FORCE_WEB_COLOR=113:t5,114:s1,58:t1,64:t2,19:t2 IDO52_COPYPROP_VAR=-8 IDO52_AS1_NO_PASS1_PROP=30,93,134,101 IDO52_AS1_NO_COPY_PROP=2,10 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u32 addr;
    u32 size;
} SpuMemEntry;

static s32 _spu_AllocLastNum;
static SpuMemEntry *_SpuMemList;

void _gcSPU(void)
{
    s32 i;
    s32 j;
    SpuMemEntry *p;
    SpuMemEntry *q;
    u32 a;
    u32 s;

    i = 0;
    if (_spu_AllocLastNum < 0) {
        goto z1;
z1:     ;
    } else {
        do {
            if (_SpuMemList[i].addr & 0x80000000) {
                for (j = i + 1; 1; j++) {
                    if (_SpuMemList[j].addr != 0x2FFFFFFF) {
                        break;
                    }
                }
                if (_SpuMemList[j].addr & 0x80000000) {
                    if ((_SpuMemList[i].addr & 0x0FFFFFFF) + _SpuMemList[i].size
                        == (_SpuMemList[j].addr & 0x0FFFFFFF)) {
                        _SpuMemList[j].addr = 0x2FFFFFFF;
                        _SpuMemList[i].size = _SpuMemList[j].size
                                             + _SpuMemList[i].size;
                        continue;
                    }
                }
            }
            i++;
        } while (i <= _spu_AllocLastNum);
    }
    i = 0;
    if (_spu_AllocLastNum >= 0) {
        q = _SpuMemList;
        do {
            if (q->size == 0) {
                q->addr = 0x2FFFFFFF;
            }
            i++;
            q++;
        } while (i <= _spu_AllocLastNum);
    }
    i = 0;
    if (_spu_AllocLastNum < 0) {
        goto z3;
z3:     ;
    } else {
        do {
            if (_SpuMemList[i].addr & 0x40000000) {
                break;
            }
            j = i + 1;
            if (i < _spu_AllocLastNum) {
                p = (SpuMemEntry *)((s32)_SpuMemList + (j << 3));
                do {
                    if (p->addr & 0x40000000) {
                        break;
                    }
                    if ((p->addr & 0x0FFFFFFF)
                        < (_SpuMemList[i].addr & 0x0FFFFFFF)) {
                        a = _SpuMemList[i].addr;
                        _SpuMemList[i].addr = p->addr;
                        s = _SpuMemList[i].size;
                        _SpuMemList[i].size = p->size;
                        p->addr = a;
                        p->size = s;
                    }
                    j++;
                    p++;
                } while (j <= _spu_AllocLastNum);
            }
            i++;
        } while (i <= _spu_AllocLastNum);
    }
    for (i = 0; i <= _spu_AllocLastNum; i++) {
        if (_SpuMemList[i].addr & 0x40000000) {
            break;
        }
        if (_SpuMemList[i].addr == 0x2FFFFFFF) {
            _SpuMemList[i].addr = _SpuMemList[_spu_AllocLastNum].addr;
            _SpuMemList[i].size = _SpuMemList[_spu_AllocLastNum].size;
            _spu_AllocLastNum = i;
            break;
        }
    }
    i = _spu_AllocLastNum - 1;
    if (_spu_AllocLastNum <= 0) {
        goto z5;
z5:     ;
    } else {
        do {
            if (_SpuMemList[i].addr & 0x80000000) {
                _SpuMemList[i].addr = _SpuMemList[i].addr & 0x0FFFFFFF;
                _SpuMemList[i].addr = _SpuMemList[i].addr | 0x40000000;
                _SpuMemList[i].size = _SpuMemList[_spu_AllocLastNum].size
                                     + _SpuMemList[i].size;
                _spu_AllocLastNum = i;
            } else {
                break;
            }
            i--;
        } while (i >= 0);
    }
}
