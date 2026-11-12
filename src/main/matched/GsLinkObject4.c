/* Function: GsLinkObject4 */
/* Address: 0x8009B3C0 */
/* Size: 304 bytes (76 instructions) */
/* Region: Library (0x80086000+), Psy-Q GCC */
/* @Compiler: gcc260 */
/* @CompileFlags: -O1 --fill-epilogue */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x38 */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x3c */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x10c --offset2 0x124 */
/* @PostProc: swap_instructions --offset1 0x110 --offset2 0x120 */
/* @PostProc: swap_instructions --offset1 0x114 --offset2 0x11c */
#include "types.h"

extern void printf(char* msg, s32 val);
extern char libgs_gpu_code_unassigned_msg[];

void GsLinkObject4(u8* arg0, u8** arg1, s32 arg2) {
    u8* primPtr;
    u32 primCount;
    u8* curPrim;
    u8* groupStart;
    u8 prevType;
    u32 curType;
    u32 i;
    s16 groupCount;
    s32 pad[2];

    curType = 0;
    primPtr = arg0 + arg2 * 28;
    *(arg1 + 2) = primPtr;
    primCount = *(u32*)(primPtr + 20);
    curPrim = *(u8**)(primPtr + 16);

    groupCount = 0;
    i = 0;
    groupStart = curPrim;

    if (primCount == 0) {
        goto done;
    }

    do {
        prevType = curType;
        curType = curPrim[3];

        if (prevType != 0) {
            if (curType != prevType) {
                *(s16*)groupStart = groupCount;
                groupCount = 0;
                groupStart = curPrim;
            }
        }

        switch (curType & 0xFD) {
            case 0x28:
            case 0x30:
                curPrim += 20;
                break;
            case 0x20:
            case 0x21:
            case 0x29:
                curPrim += 16;
                break;
            case 0x25:
            case 0x34:
            case 0x39:
                curPrim += 28;
                break;
            case 0x2C:
            case 0x2D:
                curPrim += 32;
                break;
            case 0x3D:
                curPrim += 44;
                break;
            case 0x35:
            case 0x3C:
                curPrim += 36;
                break;
            case 0x24:
            case 0x31:
            case 0x38:
                curPrim += 24;
                break;
            default:
                printf(libgs_gpu_code_unassigned_msg, curType);
                break;
        }

        i++;
        groupCount++;
    } while (i < primCount);

done:
    *(s16*)groupStart = groupCount;
}
