/* Address: 0x80090CC8   (true entry 0x80090CC0 -- see WINDOW below) */
/* Size: 128 bytes (32 instructions) */
/* Region: Library (0x80086000-0x800BFFFF) */
/* Compiler: IDO 5.2 */
/* @SymbolStart: 0x80090CC0 */
/* @SymbolSize: 128 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_AS1_NO_LA_PULL=all IDO52_AS1_NO_FALLTHRU_FILL=1 IDO52_FORCE_WEB_COLOR=0:v1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: remap_stack_slots --map '24:28' --keep-frame */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern u8 _svm_vab_used[];
extern s16 _svm_vab_count;
extern s32 _svm_vab_start[];

extern void SpuFree(s32 handle);

void SsVabClose(s16 id)
{
    u8 *p;

    if (id >= 0) {
        if (id < 16) {
            p = &_svm_vab_used[id];
            if (*p == 1) {
                SpuFree(_svm_vab_start[id]);
                *p = 0;
                _svm_vab_count--;
            }
        }
    }
}
