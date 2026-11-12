/* @CompileFlags: -Wc,-pic1 */
/* @SymbolStart: 0x801335A0 */
/* @SymbolSize: 244 */
/* @PostProc: patch_stack_complete --old 72 --new 64 */
/* sd_cmd_limit_send -- TACO overlay, Address: 0x801335A8 (true entry 0x801335A0) */
#include "types.h"

extern s32 taco_view_flag;
extern u8 taco_sd_wait[];
extern u8 taco_sd_gap_tbl[];

extern s32 rand();
extern s32 device_cmd_send();

s32 sd_cmd_limit_send(s32 arg0, s32 arg1)
{
    s32 cpy;
    s32 val;
    s32 idx;
    u8 *ptr;
    u32 ret;

    cpy = arg0;
    if (taco_view_flag != 0) {
        if (arg0 == 1) {
            val = arg1 & 0xFF;
            if (val < 10) {
                idx = val & 0xF;
                ptr = &taco_sd_wait[idx];
                if (*ptr != 0) {
                    return 0;
                }
                ret = rand(ptr);
                *ptr = ret % (u32)((taco_sd_gap_tbl[idx] << 1) + 1);
            }
        }
        val = arg1 & 0xFF;
    } else {
        val = arg1 & 0xFF;
    }
    if (cpy & 1) {
        return device_cmd_send((val | 0x500) & 0xFFFF);
    } else {
        return device_cmd_send((val | 0x600) & 0xFFFF);
    }
}
