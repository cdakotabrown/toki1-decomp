/* @SymbolStart: 0x80136448 */
/* @SymbolSize: 1280 */
/* @RodataBase: 0x8014C3F0 */
/* Address: 0x80136450 (window starts 0x80136448, see CROSS-PROLOGUE below) */
/* Size: 1280 bytes (320 instructions) */
#include "types.h"

extern void etc_ev0_scene_init(void);
extern void etc_ev2_scene_init(void);
extern void etc_ev3_scene_init(void);
extern void etc_ev4_scene_init(void);
extern void etc_ev5_scene_init(void);
extern void etc_ev6_scene_init(void);
extern void etc_ev7_scene_init(void);
extern void etc_ev8_scene_init(void);
extern void etc_ev9_scene_init(void);
extern void etc_main0a(void);
extern void etc_main0b(void);
extern void etc_main0c(void);
extern void etc_main0d(void);
extern void etc_main0e(void);
extern void etc_main0f(void);
extern void etc_ev10_scene_init(void);
extern void etc_ev11_scene_init(void);
extern void etc_ev12_scene_init(void);
extern void etc_ev13_scene_init(void);
extern void etc_ev15_scene_init(void);
extern void etc_ev16_scene_init(void);
extern void etc_ev17_scene_init(void);
extern void etc_ev18_scene_init(void);
extern void etc_ev19_scene_init(void);
extern void etc_step_main1a(void);
extern void etc_step_main1b(void);
extern void etc_step_main1c(void);
extern void etc_step_main1d(void);
extern void etc_step_main1e(void);
extern void etc_main1f(void);
extern void etc_ev20_scene_init(void);
extern void etc_ev21_scene_init(void);
extern void etc_ev23_scene_init(void);
extern void etc_ev24_scene_init(void);
extern void etc_ev25_scene_init(void);
extern void etc_ev26_scene_init(void);
extern void etc_ev27_scene_init(void);
extern void etc_ev28_scene_init(void);
extern void etc_ev29_scene_init(void);
extern void etc_main2a(void);
extern void etc_main2b(void);
extern void etc_main2c(void);
extern void etc_main2d(void);
extern void etc_main2e(void);
extern void etc_main2f(void);
extern void etc_ev30_scene_init(void);
extern void etc_ev31_scene_init(void);
extern void etc_ev32_scene_init(void);
extern void etc_ev34_scene_init(void);
extern void etc_ev36_scene_init(void);
extern void etc_ev37_scene_init(void);
extern void etc_ev38_scene_init(void);
extern void etc_ev39_scene_init(void);
extern void etc_main3a(void);
extern void etc_main3b(void);
extern void etc_main3c(void);
extern void etc_main3e(void);
extern void etc_main3f(void);
extern void etc_ev40_scene_init(void);
extern void etc_ev41_scene_init(void);
extern void etc_ev42_scene_init(void);
extern void etc_ev43_scene_init(void);
extern void etc_ev45_scene_init(void);
extern void etc_ev46_scene_init(void);
extern void etc_ev47_scene_init(void);
extern void etc_ev48_scene_init(void);
extern void etc_ev49_scene_init(void);
extern void etc_main4b(void);
extern void etc_main4d(void);
extern void etc_bg_init50(void);
extern void etc_sprite_init0e(void);
extern void etc_sprite_init46(void);
extern s32 etc_rec_kind;
extern s32 etc_rec_id;

s32 etc_step_main(void) {
    s32 mode;

    mode = etc_rec_kind;
    if (mode == 2) {
        switch ((u32)etc_rec_id) {
        case 0: etc_ev0_scene_init(); break;
        case 2: etc_ev2_scene_init(); break;
        case 3: etc_ev3_scene_init(); break;
        case 4: etc_ev4_scene_init(); break;
        case 5: etc_ev5_scene_init(); break;
        case 6: etc_ev6_scene_init(); break;
        case 7: etc_ev7_scene_init(); break;
        case 8: etc_ev8_scene_init(); break;
        case 9: etc_ev9_scene_init(); break;
        case 10: etc_main0a(); break;
        case 11: etc_main0b(); break;
        case 12: etc_main0c(); break;
        case 13: etc_main0d(); break;
        case 14: etc_main0e(); break;
        case 15: etc_main0f(); break;
        case 16: etc_ev10_scene_init(); break;
        case 17: etc_ev11_scene_init(); break;
        case 18: etc_ev12_scene_init(); break;
        case 19: etc_ev13_scene_init(); break;
        case 21: etc_ev15_scene_init(); break;
        case 22: etc_ev16_scene_init(); break;
        case 23: etc_ev17_scene_init(); break;
        case 24: etc_ev18_scene_init(); break;
        case 25: etc_ev19_scene_init(); break;
        case 26: etc_step_main1a(); break;
        case 27: etc_step_main1b(); break;
        case 28: etc_step_main1c(); break;
        case 29: etc_step_main1d(); break;
        case 30: etc_step_main1e(); break;
        case 31: etc_main1f(); break;
        case 32: etc_ev20_scene_init(); break;
        case 33: etc_ev21_scene_init(); break;
        case 35: etc_ev23_scene_init(); break;
        case 36: etc_ev24_scene_init(); break;
        case 37: etc_ev25_scene_init(); break;
        case 38: etc_ev26_scene_init(); break;
        case 39: etc_ev27_scene_init(); break;
        case 40: etc_ev28_scene_init(); break;
        case 41: etc_ev29_scene_init(); break;
        case 42: etc_main2a(); break;
        case 43: etc_main2b(); break;
        case 44: etc_main2c(); break;
        case 45: etc_main2d(); break;
        case 46: etc_main2e(); break;
        case 47: etc_main2f(); break;
        case 48: etc_ev30_scene_init(); break;
        case 49: etc_ev31_scene_init(); break;
        case 50: etc_ev32_scene_init(); break;
        case 52: etc_ev34_scene_init(); break;
        case 54: etc_ev36_scene_init(); break;
        case 55: etc_ev37_scene_init(); break;
        case 56: etc_ev38_scene_init(); break;
        case 57: etc_ev39_scene_init(); break;
        case 58: etc_main3a(); break;
        case 59: etc_main3b(); break;
        case 60: etc_main3c(); break;
        case 62: etc_main3e(); break;
        case 63: etc_main3f(); break;
        case 64: etc_ev40_scene_init(); break;
        case 65: etc_ev41_scene_init(); break;
        case 66: etc_ev42_scene_init(); break;
        case 67: etc_ev43_scene_init(); break;
        case 69: etc_ev45_scene_init(); break;
        case 70: etc_ev46_scene_init(); break;
        case 71: etc_ev47_scene_init(); break;
        case 72: etc_ev48_scene_init(); break;
        case 73: etc_ev49_scene_init(); break;
        case 75: etc_main4b(); break;
        case 77: etc_main4d(); break;
        case 80: etc_bg_init50(); break;
        }
    } else if (mode == 8) {
        switch (etc_rec_id) {
        case 14: etc_sprite_init0e(); break;
        case 70: etc_sprite_init46(); break;
        }
    }
}
