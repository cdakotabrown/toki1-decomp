/* Address: 0x80084ED8 */
/* Size: 108 bytes (27 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Likely Symbol: basyo_step_init (from o.bin) */
/* @SymbolStart: 0x80084ED0 */
/* @SymbolSize: 116 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 basho_num;
extern u8 cw16_disp;

extern void hizuke_advance(void);
extern void hizuke_init(void);
extern void hizuke_disp_switch(s32);
extern void hizuke_show(void);
extern void sub_sub_step_up(void);

void basyo_step_init(void) {
    s32 unused;
    u8 val;

    val = cw16_disp;
    basho_num = 0;
    hizuke_advance();
    hizuke_init();
    if (val & 0x80) {
        hizuke_disp_switch(1);
    } else {
        hizuke_disp_switch(0);
    }
    hizuke_show();
    sub_sub_step_up();
}
