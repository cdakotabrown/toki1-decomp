/* Function: GsSetLightMatrix */
/* Address: 0x8009AE74 */
/* Size: 148 bytes (37 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: maspsx --aspsx-version=2.67 */
/* `@PostProc: custom_script custom_func_8009AE74.py`, now retired and deleted. */
#include "types.h"

typedef struct {
    s32 data[8];
} SndData;

extern SndData GsLIGHTWSMATRIX;
extern void PushMatrix(void);
extern void MulMatrix(SndData *, s32);
extern void PopMatrix(void);
extern void SetLightMatrix(SndData *);

void GsSetLightMatrix(s32 arg0) {
    SndData local;
    local = GsLIGHTWSMATRIX;
    PushMatrix();
    MulMatrix(&local, arg0);
    PopMatrix();
    SetLightMatrix(&local);
}
