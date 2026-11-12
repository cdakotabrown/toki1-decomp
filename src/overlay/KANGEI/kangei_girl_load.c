/* @SymbolSize: 116 */
/* kangei_girl_load - KANGEI overlay, Address: 0x80135EA4, Size: 108 bytes (27 insns) */
#include "types.h"

extern void cd_read_start2(s32, void *, s32);
extern void kangei_girl10_desc_set(void);
extern void kangei_girl_desc_set(void);
extern void device_port_init(s32, s32);
extern void sub_sub_step_up(void);
extern u8 now_girl;

void kangei_girl_load(void)
{
    s32 t6_param = now_girl;

    if (t6_param == 10) {
        cd_read_start2(9, (void *)0x801D0000, 0xBCD6);
        kangei_girl10_desc_set();
    } else {
        cd_read_start2(9, (void *)0x801D0000, 0xBDA3);
        kangei_girl_desc_set();
    }
    device_port_init(1, 0);
    sub_sub_step_up();
}
