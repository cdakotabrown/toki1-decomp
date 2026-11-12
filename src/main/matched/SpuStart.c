/* Address: 0x80097688 */
/* Size: 120 bytes (30 instructions) */
/* Debug Symbol: none (a proximity o.bin name was retracted 2026-10-09) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern s32 libspu_started_flag;
extern s32 _spu_fd;
extern s32 _spu_EVdma;
extern void EnterCriticalSection(void);
extern s32 _spu_open(s32, s32);
extern s32 OpenEvent(u32, s32, s32, s32);
extern void EnableEvent(s32);
extern void ExitCriticalSection(void);

void SpuStart(void) {
    if (libspu_started_flag == 0) {
        libspu_started_flag = 1;
        EnterCriticalSection();
        _spu_fd = _spu_open(0, 3);
        _spu_EVdma = OpenEvent(0xF0000009, 32, 8192, 0);
        EnableEvent(_spu_EVdma);
        ExitCriticalSection();
    }
}
