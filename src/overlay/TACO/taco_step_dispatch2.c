/* @SymbolStart: 0x801426F0 */
/* @RodataBase: 0x8015DA90 */
/* @SymbolSize: 188 */
/* Address: 0x801426F8 */
#include "types.h"

extern u8 now_sub_sub_step;

extern void taco_result_save0(void);
extern void taco_card_save2(void);
extern void taco_resume7(void);
extern void taco_boot_data_load3(void);
extern void taco_main_data_vram_load_step(void);
extern void taco_debug_font_load_step(void);
extern void taco_cal_font_load_step(void);
extern void taco_device_init1(void);

void taco_step_dispatch2(void)
{
    u8 t6_in = now_sub_sub_step;

    switch (t6_in) {
        case 0: taco_result_save0(); break;
        case 1: taco_device_init1(); break;
        case 2: taco_card_save2(); break;
        case 3: taco_boot_data_load3(); break;
        case 4: taco_main_data_vram_load_step(); break;
        case 5: taco_debug_font_load_step(); break;
        case 6: taco_cal_font_load_step(); break;
        case 7: taco_resume7(); break;
    }
}
