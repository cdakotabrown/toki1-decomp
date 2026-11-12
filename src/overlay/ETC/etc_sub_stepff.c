/* @RodataBase: 0x8014D050 */
/* @SymbolStart: 0x80140FD8 */
/* @SymbolSize: 380 */
/* etc_sub_stepff - ETC overlay, Address: 0x80140FF4, Size: 352 bytes (88 insns) */
#include "types.h"

extern s32 step_counter;
extern u8 now_sub_sub_step;
extern s32 etc_card_format_ask();
extern s32 etc_card_init0();
extern s32 etc_card_bg_load1();
extern s32 etc_album_save_ask2();
extern s32 etc_album_save_wait3();
extern s32 etc_card_check4();
extern s32 etc_sysfile_read5();
extern s32 etc_card_save();
extern s32 etc_sysfile_make_f0();
extern s32 etc_card_notfound_ff();
extern s32 etc_card_full_fe();
extern s32 etc_sysfile_remake_fd();
extern s32 etc_card_shade_disp();

void etc_sub_stepff(void)
{
    u32 a0;

    step_counter = step_counter + 1;
    a0 = now_sub_sub_step;
    switch (a0) {
    case 0: etc_card_init0(); break;
    case 1: etc_card_bg_load1(); break;
    case 2: etc_album_save_ask2(); break;
    case 3: etc_album_save_wait3(); break;
    case 4: etc_card_check4(); break;
    case 5: etc_sysfile_read5(); break;
    case 6: etc_card_save(); break;
    case 160: etc_card_format_ask(); break;
    case 240: etc_sysfile_make_f0(); break;
    case 253: etc_sysfile_remake_fd(); break;
    case 254: etc_card_full_fe(); break;
    case 255: etc_card_notfound_ff();
    default: break;
    }
    etc_card_shade_disp();
}
