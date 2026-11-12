/* @SymbolStart: 0x8013A220 */
/* @SymbolSize: 284 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x30:rs:v0:v1 0x38:rs:v0:v1 0x80:rt:t6:v1 0x84:rs:t6:v1 0x84:rt:t6:v1 0x8c:rs:t6:v1 0x8c:rt:t7:t6 0x94:rt:t7:t6 0xe4:rt:t8:v1 0xe8:rs:t8:v1 0xe8:rt:t8:v1 0xf0:rs:t8:v1 0xf0:rt:t9:t7 0xf8:rt:t9:t7 */
/* tel_basyo_miss - TEL overlay, Address: 0x8013A228, Size: 276 bytes (69 insns), Semi-matched */
#include "types.h"

extern u8 step_mode;
extern u8 p_sei;
extern u8 tel_basyo_miss_str0;
extern u8 tel_basyo_miss_str1;

extern void tel_girl_adjust(void);
extern void k_sub_reset(void);
extern void gnsx(s32);
extern void sndisp(s32, s32, s32);
extern void k_sub_disp_start(s32);
extern void xa_wait(void);
extern void device_port_init(s32, s32);
extern void tel_reserved_clear(void);
extern void sub_sub_step_up(void);

void tel_basyo_miss(void) {
    u8 hidden_v1;

    hidden_v1 = step_mode;
    switch (hidden_v1) {
        case 0:
            tel_girl_adjust();
            k_sub_reset();
            gnsx((s32)&p_sei);
            sndisp((s32)&tel_basyo_miss_str0, 0, 31);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            return;
        case 1:
            xa_wait();
            return;
        case 2:
            k_sub_reset();
            gnsx((s32)&p_sei);
            device_port_init(0, 514);
            sndisp((s32)&tel_basyo_miss_str1, 0, 31);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            return;
        case 3:
            xa_wait();
            return;
    }
    tel_reserved_clear();
    sub_sub_step_up();
}
