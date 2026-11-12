/* Function: xa_set_m (xa_wait_m) */
/* Address: 0x80052480 */
/* Size: 252 bytes (63 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: remap_stack_slots --map '24:40' */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 voice_level;
extern s32 xa_m_tbl;
extern s32 xa_m_tbl2;
extern s32 xa_m_idx;
extern s32 xa_m_mode;

extern void set_XAsector(s32, s32, s32);
extern s32 device_cmd_send(s32);

void xa_set_m(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 mask;

    if (arg2 == -1) {
        mask = arg3 & 0xF;
        if (mask != 0xF) {
            if (voice_level != 0) {
                set_XAsector(arg0, arg1, arg3);
                device_cmd_send(0x300);
            }
        }
    } else {
        mask = arg3 & 0xF;
        if (mask != 0xF) {
            if (voice_level != 0) {
                set_XAsector(((s32 *)arg0)[arg2 * 2],
                              ((s32 *)arg0)[arg2 * 2 + 1], arg3);
                device_cmd_send(0x300);
            }
        }
    }
    xa_m_tbl = arg0;
    xa_m_tbl2 = arg1;
    xa_m_idx = arg2;
    if (mask == 0xF) {
        xa_m_mode = 2;
    } else {
        xa_m_mode = 0;
    }
}
