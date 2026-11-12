/* Address: 0x8006CD84 */
/* Size: 48 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 device_init_flag_get(void);
extern void device_port_init(s32 a0, s32 a1);

void restore_bgm(void) {
    if (device_init_flag_get() == 0) {
        device_port_init(0, 0);
    }
}
