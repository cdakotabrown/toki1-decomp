/* Address: 0x80042C80 */
/* Size: 200 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 pad_type_check(void);
extern void mouse_pad_set(void);
extern void pad_scan(void);

extern u8 pad_type;
extern u8 pad_main_tbl0;
extern u32 pad_trig;
extern u32 pad_data;
extern u8 bustup_sw;

void pad_main(void)
{
    if (pad_type_check() != 0) {
        return;
    }

    if (pad_type == 0) {
        mouse_pad_set();
    } else if (pad_type != 255) {
        pad_scan();
    }

    if (pad_main_tbl0 != 0) {
        if ((pad_trig & 0xF9DF) != 0) {
            pad_main_tbl0 = 0;
        }
    }

    if ((pad_data & 0x00F00000) == 0x00F00000) {
        pad_main_tbl0 = 1;
    }

    if ((s32)(pad_trig << 4) < 0) {
        bustup_sw = 1 - bustup_sw;
    }
}
