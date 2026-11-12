/* Address: 0x8008A004 */
/* Size: 500 bytes (125 instructions) */
/* Region: Library (GCC 2.6.0, Konami CD bring-up -- sibling of cd_init_retry) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -fno-schedule-insns --fill-load-delay --fill-earliest --store-macro-split */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0xa8:rt:a3:a0 0xac:rt:a3:a0 0xac:rs:a3:a0 0xc8:rd:a3:a0 0xc8:rt:a3:a0 0xd8:rd:a3:a0 0xd8:rs:a3:a0 0xe4:rs:a3:a0 */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xc8 */
/* @PostProc: swap_instructions --offset1 0xbc --offset2 0xcc */
/* @PostProc: swap_instructions --offset1 0xc0 --offset2 0xd0 */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xd4 */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xd8 */
/* @PostProc: swap_instructions --offset1 0xcc --offset2 0xd0 */
/* @PostProc: swap_instructions --offset1 0xd0 --offset2 0xdc */
/* @PostProc: swap_instructions --offset1 0xd4 --offset2 0xe4 */
/* @PostProc: swap_instructions --offset1 0xd8 --offset2 0xe4 */
/* @PostProc: swap_instructions --offset1 0xdc --offset2 0xe4 */
/* @PostProc: swap_instructions --offset1 0xe0 --offset2 0xe4 */
#include "types.h"

extern s32 VSync(s32 mode);
extern s32 CheckCallback(void);
extern void callback(void);
extern void CD_init(void);
extern void printf(const char *fmt, ...);

extern s32 libcd_timeout_deadline;
extern s32 libcd_timeout_spin_count;
extern const char *libcd_cur_func_name;
extern const char libcd_ready_name_str[];
extern const char libcd_timeout_fmt[];

extern u8 libcd_last_cmd;
extern s32 cd_status;
extern volatile s32 libcd_intr_code;
extern s32 libcd_dataend_event;
extern const char *CD_intstr[];
extern const char *CD_comstr[];

extern u8 libcd_dataend_result_buf[];
extern u8 Result[];

s32 CD_ready(s32 mode, u8 *result)
{
    s32 r;
    s32 ev;
    s32 i;
    const char **t1;
    const char **t2;
    u8 *src;
    u8 *dst;
    s32 i1;
    s32 i2;
    s32 i3;

    libcd_timeout_deadline = VSync(-1) + 480;
    libcd_timeout_spin_count = 0;
    libcd_cur_func_name = libcd_ready_name_str;

    t1 = CD_intstr;
    t2 = CD_comstr;
    for (;;) {
        if (libcd_timeout_deadline < VSync(-1) || libcd_timeout_spin_count++ > 0xF00000) {
            i1 = libcd_last_cmd;
            i2 = cd_status;
            i3 = libcd_intr_code;
            printf(libcd_timeout_fmt, libcd_cur_func_name, t2[i1], t1[i2], t1[i3]);
            CD_init();
            r = -1;
        } else {
            r = 0;
        }
        if (r != 0) {
            return -1;
        }
        if (CheckCallback() != 0) {
            callback();
        }
        ev = libcd_dataend_event;
        if (ev) {
            libcd_dataend_event = 0;
            src = libcd_dataend_result_buf;
            if (result) {
                dst = result;
                for (i = 7; i != -1; i--) {
                    *dst++ = *src++;
                }
            }
            return ev;
        }
        ev = libcd_intr_code;
        if (ev) {
            libcd_intr_code = 0;
            src = Result;
            dst = result;
            if (dst) {
                for (i = 7; i != -1; i--) {
                    *dst++ = *src++;
                }
            }
            return ev;
        }
        if (mode != 0) {
            return 0;
        }
    }
}
