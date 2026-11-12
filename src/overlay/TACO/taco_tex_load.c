/* @SymbolSize: 204 */
#include "types.h"

extern u8 taco_load_no;

extern void taco_drop(s32 a0, s32 a1);
extern void device_port_init2(s32 a0, s32 a1);
extern void image_upload_queue_add_flags(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5,
                          s32 a6);
extern void sub_sub_step_up(void);

void taco_tex_load(void)
{
    taco_drop(1, 1);

    if (taco_load_no != 0) {
        return;
    }

    device_port_init2(0, 515);
    image_upload_queue_add_flags(0x80162000, 14, 1, 0, 0, 256, 256);
    image_upload_queue_add_flags(0x80172000, 12, 1, 0, 0, 256, 256);
    image_upload_queue_add_flags(0x80182000, 10, 1, 0, 0, 128, 128);
    sub_sub_step_up();
}
