/* @SymbolStart: 0x80133A60 */
/* @SymbolSize: 132 */
/* Address: 0x80133A68, Size: 124 bytes (31 insns) */
#include "types.h"

extern u8 date2_mode_no;

extern u8 now_year;

void date2_intro_script2(void);
void date2_intro_script(void);
void date2_scene_dispatch(void);
void date2_spot_light_main(void);

void date2_mode_dispatch(void)
{
    u8 v0;

    v0 = date2_mode_no;
    switch (v0) {
    case 0:
        date2_intro_script2();
        return;
    case 1:
        if ((u32)now_year >= 97U) {
            date2_intro_script();
            return;
        }
        date2_scene_dispatch();
        return;
    default:
        date2_spot_light_main();
        return;
    }
}
