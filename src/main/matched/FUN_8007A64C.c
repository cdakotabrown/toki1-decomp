/* Address: 0x8007A64C */
/* Size: 92 bytes (23 instructions) */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"
#include "tokimeki/sd.h"

extern s32 gSDInfo;

extern void serial_cmd(s32);
extern void SD_SerialClear(void);
extern void SD_SerialVolSend(s32);

void FUN_8007A64C(void) {
    serial_cmd(116);

    if ((gSDInfo & 0x4000) == 0) {
        SD_SerialClear();
        if ((*(SD_INFO *)&gSDInfo).xa_start != 0) {
            SD_SerialVolSend((*(SD_INFO *)&gSDInfo).xa_start);
        }
    }
}
