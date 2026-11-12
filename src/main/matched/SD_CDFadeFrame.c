/* Likely Symbol: SD_CDFadeFrame (from o.bin, offset +20 bytes from 0x80079F18) */
/* Address: 0x80079F2C */
/* Size: 100 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"
#include "tokimeki/sd.h"

extern u32 cdq_result_word_get(void);
extern void serial_fade_set(s32 val);
extern void SD_CDLevelRead(void *addr);

extern u32 gSDSerialInfo;
extern u8 gSDCDLevelInfo;
extern SD_INFO gSDInfo;

void SD_CDFadeFrame(void)
{
    u32 val;

    val = cdq_result_word_get();
    if (val >= (*(u32 *)&gSDInfo.xa_end)) {
        if (!(gSDSerialInfo & 0x100)) {
            serial_fade_set(180);
        }
    } else {
        SD_CDLevelRead(&gSDCDLevelInfo);
    }
}
