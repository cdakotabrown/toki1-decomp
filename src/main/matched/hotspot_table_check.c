/* Address: 0x8005B2D8 */
/* Size: 172 bytes (43 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x8005B2D0 */
/* @SymbolSize: 180 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: patch_insn_reg_field --patch 0x6c:rt:t1:t2 --patch 0x70:rt:t9:t0 --patch 0x74:rt:t9:t0 --patch 0x74:rd:t0:t1 --patch 0x78:rs:t1:t2 --patch 0x78:rt:t1:t2 --patch 0x7c:rs:t0:t1 --patch 0x7c:rt:t1:t2 */
#include "types.h"

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 pad_trig;
extern u16 pass_days;
extern s32 hotspot_table_check_tbl0;

extern void device_cmd_send(s32);
extern void load_header_check(s32);
extern void set_XAsector(s32, s32, s32);

s32 hotspot_table_check(void) {
    s32 param;
    s16 val;
    s32 flags;
    s32 *ptr;

    param = cursor_cw_x;
    if (param < 8) {
        if (param >= -39) {
            val = cursor_cw_y;
            if (val < 36) {
                if (val >= 25) {
                    flags = pad_trig;
                    if (flags & 0x20) {
                        device_cmd_send(209);
                        load_header_check(0);
                        ptr = (s32 *)((u32)&hotspot_table_check_tbl0 + (u32)((pass_days & 7) << 3));
                        set_XAsector(ptr[0], ptr[1], 14);
                        device_cmd_send(768);
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
