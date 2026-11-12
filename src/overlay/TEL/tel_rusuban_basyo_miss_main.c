/* @SymbolStart: 0x801398FC */
/* @RodataBase: 0x8013FE18 */
/* @SymbolSize: 216 */
/* tel_rusuban_basyo_miss_main - TEL overlay, Address: 0x80139904, Size: 208 bytes (52 insns) */
#include "types.h"

extern u8 now_sub_sub_step;

extern s32 reserved_date;

extern void goto_sub_sub_step(s32);
extern void tel_rusuban_init(void);
extern void tel_reserved_date_step(void);
extern void tel_basyo_miss(void);
extern void tel_goto_step31(void);
extern void tel_home_ring_init(void);
extern void tel_yoshio_dialog(void);
extern void sub_sub_step_up(void);

void tel_rusuban_basyo_miss_main(void) {
    u8 hidden_t6;

            s32 val;
    hidden_t6 = now_sub_sub_step;
    switch (hidden_t6) {
        case 0:
        {
            val = reserved_date;
            if (((u32)(val << 3) >> 28) == 10) {
                goto_sub_sub_step(10);
                return;
            }
        }

            tel_rusuban_init();
            return;
        case 1:
            tel_reserved_date_step();
            return;
        case 2:
            tel_basyo_miss();
            return;
        case 3:
            tel_goto_step31();
            return;
        case 10:
            tel_home_ring_init();
            return;
        case 11:
            tel_yoshio_dialog();
            return;
        case 7:
        default:
            sub_sub_step_up();
            break;
    }
}
