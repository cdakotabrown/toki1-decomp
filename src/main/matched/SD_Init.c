/* Likely Symbol: SD_VABAddrSet (from o.bin, offset +208 bytes from 0x8007B9A4) */
/* Address: 0x8007B8D4 */
/* Size: 180 bytes (45 instructions) */
/* Region: GCC (< 0x80081000, OR encoding) */
/* Compiler: IDO 5.2 via ido-static-recomp */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void SsInit(void);
extern void SsSetTickMode(s32 arg0);
extern void SsSetReservedVoice(s32 arg0);
extern void SsSetTableSize(s32 *arg0, s32 arg1, s32 arg2);
extern void SsSetAutoKeyOffMode(s32 arg0);
extern void SsUtSetReverbType(s32 arg0);
extern void SpuClearReverbWorkArea(s32 arg0);
extern void SsUtReverbOn(void);
extern void SsUtSetReverbDepth(s32 arg0, s32 arg1);
extern void SsSetMVol(s32 arg0, s32 arg1);
extern void SD_StereoModeSet(s32 arg0);
extern void SD_StateInit(void);
extern void SD_BufferTableInit(void);
extern void SD_SerialVolInit(void);
extern void SD_WorkInit(void);
extern void cmd_que_clear(void);
extern void SsStart2(void);

extern s32 gSDSsTable;

void SD_Init(void)
{
    SsInit();
    SsSetTickMode(1);
    SsSetReservedVoice(24);
    SsSetTableSize(&gSDSsTable, 1, 1);
    SsSetAutoKeyOffMode(1);
    SsUtSetReverbType(3);
    SpuClearReverbWorkArea(3);
    SsUtReverbOn();
    SsUtSetReverbDepth(60, 60);
    SsSetMVol(127, 127);
    SD_StereoModeSet(2);
    SD_StateInit();
    SD_BufferTableInit();
    SD_SerialVolInit();
    SD_WorkInit();
    cmd_que_clear();
    SsStart2();
}
