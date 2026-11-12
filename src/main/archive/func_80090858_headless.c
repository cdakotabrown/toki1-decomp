/* Function: func_80090858 */
/* Address: 0x80090858 */
/* Size: 140 bytes (35 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: scheduler_normalizer */
#include "types.h"

typedef struct {
    u8 pad1[118];
    u8 field118;
    u8 pad2;
    u8 field120;
    u8 pad3[23];
    u32 flags;
} SoundStruct;

extern SoundStruct *_ss_score[];
extern void SpuVmGetSeqVol(s32, u8 *, u8 *);

void func_80090858(s32 index) {
    SoundStruct *ptr;

    ptr = _ss_score[index];
    SpuVmGetSeqVol(index, &ptr->field118, &ptr->field120);
    ptr = _ss_score[index];
    ptr->flags &= ~1;
    ptr = _ss_score[index];
    ptr->flags &= ~8;
    ptr = _ss_score[index];
    ptr->flags |= 2;
}
