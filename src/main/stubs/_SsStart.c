/* Address: 0x8008BB24 */
/* Size: 680 bytes (170 instructions) */
/* Region: IDO */
#include "types.h"

extern void VSyncCallback(void);
extern void EnterCriticalSection(void);
extern void ResetRCnt(void);
extern void SetRCnt(void);
extern void _wait(void);
extern void StartRCnt(void);
extern void InterruptCallback(void);
extern void ExitCriticalSection(void);

void _SsStart(void)
{

}
