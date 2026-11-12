/* @CompileFlags: -Wc,-pic0 */
/* @SymbolStart: 0x80139DC0 */
/* @SymbolSize: 348 */
/* Address: 0x80139DC8 (true entry 0x80139DC0: the step_mode read is */
/* Size: 340 bytes (85 insns) + 8 hoisted = 348 */
/* @CompileEnv: IDO52_CMOT_KILL_AVAIL=15,17 */
/* @PostProc: patch_insn_reg_field 0x1c:rs:v0:v1 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x28:rs:v0:v1 0x30:rs:v0:v1 0x38:rs:v0:v1 0xc0:rt:t8:v1 0xc4:rs:t8:v1 0xc4:rt:t8:v1 0xcc:rs:t8:v1 0xcc:rt:t9:t8 0xd4:rt:t9:t8 0x124:rt:t0:v1 0x128:rs:t0:v1 0x128:rt:t0:v1 0x130:rs:t0:v1 0x130:rt:t1:t9 0x138:rt:t1:t9 */
#include "types.h"

extern volatile u8 step_mode;
extern volatile s32 reserved_date;

extern u8 p_sei[];
extern u8 tel_date_forget_str0[];
extern u8 tel_date_forget_str1[];
extern u8 tel_date_forget_str2[];

extern void tel_girl_adjust(void);
extern void k_sub_reset(void);
extern void gnsx(s32);
extern void strcpy(s32, s32);
extern void get_g_name(s32, s32);
extern void strcat(s32, s32);
extern void sndisp(s32, s32, s32);
extern void k_sub_disp_start(s32);
extern void xa_wait(void);
extern void device_port_init(s32, s32);
extern void tel_reserved_clear(void);
extern void sub_sub_step_up(void);

void tel_date_forget(void)
{
    u8 buf2[260];
    u8 buf1[32];
    s32 pad0;
    s32 pad1;
    s32 pad2;
    s32 pad3;

    switch (step_mode) {
    case 0:
        tel_girl_adjust();
        k_sub_reset();
        gnsx((s32)p_sei);
        strcpy((s32)&buf2[0], (s32)tel_date_forget_str0);
        get_g_name((s32)&buf1[0], ((u32)(reserved_date << 3)) >> 28);
        strcat((s32)&buf2[0], (s32)&buf1[0]);
        strcat((s32)&buf2[0], (s32)tel_date_forget_str1);
        sndisp((s32)&buf2[0], 0, 31);
        k_sub_disp_start(0);
        step_mode = step_mode + 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        k_sub_reset();
        gnsx((s32)p_sei);
        device_port_init(0, 514);
        sndisp((s32)tel_date_forget_str2, 0, 31);
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
