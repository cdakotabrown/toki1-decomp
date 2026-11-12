/* date_cmd512_send - DATE overlay, Address: 0x8014A284, Size: 76 bytes (19 insns) */
#include "types.h"

extern void device_cmd_send(s32 a);
extern void girl_season_load(void);
extern u8 now_sub_sub_step;
extern s16 date_open_step;

void date_cmd512_send(void)
{
    device_cmd_send(512);
    girl_season_load();
    now_sub_sub_step--;
    date_open_step++;
}
