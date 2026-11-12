/* @SymbolStart: 0x8013A158 */
/* @RodataBase: 0x8013FFA4 */
/* @SymbolSize: 200 */
/* tel_rusuban_date_forget_main - TEL overlay, Address: 0x8013A160, Size: 192 bytes (48 insns) */
#include "types.h"

extern u8 now_sub_sub_step;

extern s32 reserved_date;

extern void goto_sub_sub_step(s32);
extern void tel_rusuban_init(void);
extern void tel_reserved_date_step(void);
extern void tel_date_forget(void);
extern void tel_goto_step31(void);
extern void tel_home_ring_init(void);
extern void tel_yoshio_dialog(void);

void tel_rusuban_date_forget_main(void) {
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
            tel_date_forget();
            return;
        case 3:
            tel_goto_step31();
            return;
        case 10:
            tel_home_ring_init();
            return;
        case 11:
            tel_yoshio_dialog();
            break;
    }
}
