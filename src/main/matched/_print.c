/* Function: _print   (CD entry-table debug dump) */
/* Address: 0x80098530  (0x80098538 is the PROLOGUE; the window starts 8 B earlier) */
/* Size: 196 bytes (49 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */

/* @SymbolStart: 0x80098530 */
/* @SymbolSize: 196 */
/* @CompileFlags: -Wo,-do_opt_saved_regs */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=0:s1,7:s0 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */













































































#include "types.h"

typedef struct {
    u32 field_0;
    u32 field_4;
} CDEntry;


extern CDEntry *_SpuMemList;

extern u8 libspu_memlist_print_fmt[];

extern volatile s32 _spu_AllocBlockNum;


extern void printf(u8 *, s32, u32, u32, u32);

#line 20
void _print(void) {
#line 21
    CDEntry *entry;
#line 22
    s32 i;
#line 30
    i = 0;
#line 30
    if (_spu_AllocBlockNum < 0) {
#line 30
        return;
#line 30
    }
#line 30
    entry = _SpuMemList;
#line 30
    do {
#line 30
        printf(libspu_memlist_print_fmt, i, entry->field_0, entry->field_4, entry->field_4);
#line 30
        entry = _SpuMemList;
#line 30
        entry += i;
#line 30
        if (entry->field_0 & 0x40000000) {
#line 30
            return;
#line 30
        }
#line 30
        i++;
#line 30
        entry++;
#line 30
    } while (i <= _spu_AllocBlockNum);
}
