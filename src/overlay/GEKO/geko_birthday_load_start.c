/* geko_birthday_load_start -- GEKO overlay, Address: 0x801346C4, Size: 116 bytes (29 insns) */
#include "types.h"

extern void cd_read_start2(s32 a0, s32 a1, u32 a2);
extern void geko_ptr_table_init1(void);
extern void device_cmd_send(s32 a0);
extern s32 set_kanji_string(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void sub_sub_step_up(void);

extern u8 day_flag;
extern s32 geko_load4_str0;

void geko_birthday_load_start(void)
{
    cd_read_start2(3, 0x801B0000, 0xAF43);
    geko_ptr_table_init1();
    if ((day_flag & 0x8) != 0) {
        goto label0;
    }
    device_cmd_send(1539);
    set_kanji_string(-40, 64, 2, (s32)&geko_load4_str0, 0);

label0:
    sub_sub_step_up();
}
