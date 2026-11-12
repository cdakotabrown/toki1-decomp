/* Function: SpuQuit */
/* Address: 0x80097700 */
/* Size: 108 bytes (27 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern void EnterCriticalSection(void);
extern void _spu_close(s32 arg0);
extern void CloseEvent(s32 arg0);
extern void DisableEvent(s32 arg0);
extern void ExitCriticalSection(void);

extern s32 _spu_fd;
extern s32 _spu_EVdma;
extern s32 libspu_started_flag;

void SpuQuit(void) {
    s32 flag = libspu_started_flag;
    if (flag == 1) {
        libspu_started_flag = 0;
        EnterCriticalSection();
        _spu_close(_spu_fd);
        CloseEvent(_spu_EVdma);
        DisableEvent(_spu_EVdma);
        ExitCriticalSection();
    }
}
