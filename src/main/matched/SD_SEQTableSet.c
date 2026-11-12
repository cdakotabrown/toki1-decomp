/* Function: SD_SEQTableSet */
/* Address: 0x8007BC90 (true entry 0x8007BC78 -- see BOUNDARY below) */
/* Size: 276 bytes (69 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @SymbolStart: 0x8007BC78 */
/* @SymbolSize: 276 */
/* @PostProc: strip_leading_instructions --count auto */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=36:t0,45:t1,55:t2,65:t3 IDO52_AS1_TIE_TAKE=327,330 */
#include "types.h"

extern u32 gSDSeqAdrs[][16];
extern u16 gSDSeqReverb[][16];

void SD_SEQTableSet(u8 *tbl, u16 idx)
{
    u32 *outPtrs;
    u16 *outIds;
    s32 j;
    u32 offs;

    outPtrs = gSDSeqAdrs[idx];
    outIds = gSDSeqReverb[idx];

    for (j = 0; j < 16; j++) {
        offs = *(u32 *)(j * 4 + (u32)tbl);
        if (offs == 0) {
            offs = *(u32 *)tbl;
        }
        outPtrs[j] = (u32)(tbl + offs + 16);
        outIds[j] = tbl[64 + j];
    }
}
