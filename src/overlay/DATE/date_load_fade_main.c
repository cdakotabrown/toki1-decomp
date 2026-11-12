/* @SymbolStart: 0x801362A4 */
/* @RodataBase: 0x8015AECC */
/* Address: 0x801362AC */
/* Size: 292 bytes (73 insns) */
/* @SymbolSize: 300 */
#include "types.h"

extern void date_load2_tbl(void);
extern s32  get_CD_State2(void);
extern void date_girl_cdread(void);
extern void date_load3_tbl(void);
extern void device_cmd_send(s32);
extern void fade_in_step(void);

extern u8 now_sub_sub_step;
extern u8 step_mode;
extern u8 now_basyo;

void date_load_fade_main(void) {
    switch (step_mode) {
    case 0:
        date_load2_tbl();
        now_sub_sub_step = now_sub_sub_step - 1;
        step_mode = 1;
        break;
    case 1:
    case 3:
    case 5:
        if (get_CD_State2() & 1) {
            step_mode++;
        }
        break;
    case 2:
        date_girl_cdread();
        now_sub_sub_step = now_sub_sub_step - 1;
        step_mode = 3;
        break;
    case 4:
        date_load3_tbl();
        now_sub_sub_step = now_sub_sub_step - 1;
        step_mode = 5;
        break;
    case 6:
        if (now_basyo == 14) {
            device_cmd_send(1280);
        }
        step_mode++;
        break;
    case 7:
        fade_in_step();
        break;
    }
}
