/* Address: 0x8007A9B4 */
/* Size: 104 bytes (26 instructions) */
/* Region: Game (IDO 5.2 -O2, -Wc,-pic1) */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern void SD_SerialVolInit(void);
extern void cd_command_issue(s32 a0, u32 a1, u8* a2);

typedef struct {
    u32 flags;
    u8 pad[0x40];
    u8 c8;
    u8 c9;
    u8 ca;
    u8 cb;
    u8 cc;
    u8 cd;
    u8 ce;
    u8 cf;
} SaveBlk;

extern SaveBlk gSDSerialInfo;

void SD_SerialVolSend(u32 arg0)
{
    u32 temp;

    SD_SerialVolInit();

    gSDSerialInfo.c8 = gSDSerialInfo.ce;
    gSDSerialInfo.c9 = gSDSerialInfo.cc;

    temp = gSDSerialInfo.flags;
    gSDSerialInfo.flags = temp | 0x200;

    cd_command_issue(13, arg0, &gSDSerialInfo.c8);
}
