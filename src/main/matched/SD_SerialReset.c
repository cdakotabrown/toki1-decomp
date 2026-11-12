/* Function: SD_SerialReset */
/* Address: 0x8007B7C4 */
/* Size: 172 bytes (43 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"
#include "tokimeki/sd.h"

extern void SsSetSerialAttr(s32, s32, s32);
extern s32 gSDInfo;
extern s32 gSDSerialInfo;
extern s16 gSDCDLevelInfo[];

void SD_SerialReset(void) {
    s32 *p;
    s32 *q;
    s32 temp1;
    s32 temp2;

    SsSetSerialAttr(0, 0, 0);
    p = &gSDInfo;
    temp1 = *p;
    temp1 &= ~0x400;
    q = &gSDSerialInfo;
    temp2 = *q;
    temp1 &= ~0x4000;
    temp2 &= ~0x200;
    temp2 &= ~0x100;
    gSDCDLevelInfo[0] = 0;
    gSDCDLevelInfo[1] = 0;
    (*(SD_INFO *)&gSDInfo).xa_start = 0;
    (*(SD_INFO *)&gSDInfo).xa_end = 0;
    (*(SD_INFO *)&gSDInfo).xa_next_start = 0;
    (*(SD_INFO *)&gSDInfo).xa_next_end = 0;
    (*(SD_INFO *)&gSDInfo).xa_resume = 0;
    (*(s8 *)&(*(SD_INFO *)&gSDInfo).xa_chan) = 0;
    (*(s8 *)&(*(SD_INFO *)&gSDInfo).xa_chan_next) = 0;
    *p = temp1;
    *q = temp2;
}
