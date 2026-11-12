/* @Compiler: gcc281 */
/* @SymbolSize: 76 */
/* Address: 0x80148AA0 */
/* Size: 76 bytes (19 insns) */
/* @SymbolSize: 76 windows the real body (the +76 word is zero padding). */
/* Sole @Compiler: gcc281 file in src/overlay/. Requires a working preprocessor */
#include "types.h"

typedef struct {
    s16 field_0;
    s16 pad;
    s32 field_4;
    u8 field_8;
    u8 field_9;
    u8 field_A;
} SoundParams;

extern void SetDQA(s16);
extern void SetDQB(s32);
extern void SetFarColor(u8, u8, u8);

void taiiku_depth_set(SoundParams *params)
{
    SetDQA(params->field_0);
    SetDQB(params->field_4);
    SetFarColor(params->field_8, params->field_9, params->field_A);
}
