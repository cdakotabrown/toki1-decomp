/* Address: 0x8008B1D0  Size: 92 bytes (23 insns) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: scheduler_normalizer --target disks/SLPS_000.65 --address 0x8008B1D0 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --map '20:16' */
#include "types.h"

extern u32 StRingSize;
extern u32 StRingIdx1;
extern u32 StRingIdx2;
extern u32 StRingIdx3;
extern u32 StFinalSector;
extern u32 Stframe_no;
extern u16 Stsector_offset;
extern u32 StCdIntrFlag;

extern void init_ring_status(s32 arg0, u32 arg1);

void StClearRing(void) {
    u32 tmp;
    StRingIdx3 = 0;
    StRingIdx2 = 0;
    StRingIdx1 = 0;
    tmp = StRingSize;
    StFinalSector = 0;
    init_ring_status(0, tmp);
    StCdIntrFlag = 0;
    Stsector_offset = 0;
    Stframe_no = 0;
}
