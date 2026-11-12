/* Address: 0x8007C0C0 */
/* Size: 736 bytes (184 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 SpuIsTransferCompleted();
extern s32 SsVabTransCompleted();
extern void SpuSetTransferStartAddr();
extern void SpuRead();
extern void SD_CDLevelPeak();
extern void SD_CDLevelAvg();

extern u8 gSDCDLevelTbl[];
extern u8 spu_read_buf[];
extern u32 gSDSerialInfo;
extern u32 gSDInfo;

void SD_CDLevelRead(info)
    u16 *info;
{
    u32 x;
    u32 y;
    u32 sh;

    info[0] = 0;
    info[1] = 0;
    if (gSDSerialInfo >> 24) {
        return;
    }
    if (gSDInfo & 0x4000) {
        return;
    }
    if (SpuIsTransferCompleted(0) != 1) {
        return;
    }
    if (SsVabTransCompleted(0) != 1) {
        return;
    }
    SpuSetTransferStartAddr(0);
    SpuRead(gSDCDLevelTbl, 512);
    SpuIsTransferCompleted(1);
    SpuSetTransferStartAddr(1024);
    SpuRead(spu_read_buf, 512);
    SpuIsTransferCompleted(1);
    switch (info[4] & 0xF) {
    case 1:
        SD_CDLevelPeak(info, info + 1, 0, info[3]);
        break;
    case 2:
        SD_CDLevelAvg(info, info + 1, 0, info[3]);
        break;
    }
    x = info[0];
    y = info[1];
    if ((info[4] & 0xF0) == 0x20) {
        x = x * x >> 15;
        y = y * y >> 15;
    }
    switch (info[4] & 0xF00) {
    case 0x100:
        if (x < 0x3333) {
            x = x << 2;
        } else {
            x = (x >> 2) + 0xC000;
        }
        if (y < 0x3333) {
            y = y << 2;
        } else {
            y = (y >> 2) + 0xC000;
        }
        break;
    case 0x400:
        if (x < 0x9999) {
            x = (x << 2) / 6;
        } else {
            x = (x * 6 >> 2) - 0x8001;
        }
        if (y < 0x9999) {
            y = (y << 2) / 6;
        } else {
            y = (y * 6 >> 2) - 0x8001;
        }
        break;
    case 0x800:
        if (x < 0xB333) {
            x = (x << 1) / 7;
        } else {
            x = (x << 3) / 3 - 0x1AAAB;
        }
        if (y < 0xB333) {
            y = (y << 1) / 7;
        } else {
            y = (y << 3) / 3 - 0x1AAAB;
        }
        break;
    }
    sh = 16 - info[2];
    info[0] = (x >> sh) & 0xFFFF;
    info[1] = (y >> sh) & 0xFFFF;
}
