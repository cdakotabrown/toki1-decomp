/* @SymbolStart: 0x8013513C */
/* @SymbolSize: 76 */
/* kangei_yuukou_set50 - KANGEI overlay, Address: 0x80135144, Size: 68 bytes (17 insns) */
#include "types.h"

extern void device_cmd_send(s32);
extern void kangei_girl_flag_set(void);

extern u8 girl0_yuukou[];
extern u8 now_girl;

void kangei_yuukou_set50(void)
{
    s32 t7_in = now_girl;

    *(s16 *)(girl0_yuukou + (u32)t7_in * 56) = 50;
    device_cmd_send(36);
    kangei_girl_flag_set();
}
