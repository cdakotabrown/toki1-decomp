/* @PostProc: strip_leading_instructions --count 4 */
/* Function: SD_VabLoadCmd */
/* Address: 0x8007A2E4 */
/* Size: 256 bytes (64 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void SD_VABAddrSet(u16);
extern void SD_VABClose(s32);
extern void SD_VabLoadReq(s32);

void SD_VabLoadCmd(u16 arg0) {
    arg0 = arg0 & 0xFF;
    SD_VABAddrSet(arg0);

    switch (arg0 - 17) {
        case 0:
            SD_VABClose(0);
            SD_VabLoadReq(0);
            SD_VABClose(1);
            SD_VabLoadReq(1);
            break;
        case 1:
            SD_VABClose(2);
            SD_VabLoadReq(2);
            SD_VABClose(3);
            SD_VabLoadReq(3);
            break;
        case 2:
            SD_VABClose(0);
            SD_VabLoadReq(0);
            break;
        case 3:
            SD_VABClose(1);
            SD_VabLoadReq(1);
            break;
        case 4:
            SD_VABClose(2);
            SD_VabLoadReq(2);
            break;
        case 5:
            SD_VABClose(3);
            SD_VabLoadReq(3);
            break;
    }
}
