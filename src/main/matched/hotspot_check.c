/* Address: 0x8005B1B8, Size: 160 bytes */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 pad_trig;
extern void device_cmd_send(s32 a);
extern void load_header_check(s32 a);
extern void set_XAsector(s32 a, s32 b, s32 c);
s32 hotspot_check(void) {
    s32 v;

    v = (s32)(s16)cursor_cw_x;
    if (v >= 34) goto no;
    if (v < -13) goto no;
    v = (s32)(s16)cursor_cw_y;
    if (v >= 8) goto no;
    if (v < -11) goto no;
    if ((pad_trig & 0x20) == 0) goto no;
    device_cmd_send(177);
    load_header_check(0);
    set_XAsector(0x1D000185, 0x17000397, 0);
    device_cmd_send(768);
    return 1;
no:
    return 0;
}
