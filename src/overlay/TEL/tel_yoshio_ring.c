/* @CompileEnv: IDO52_FORCE_WEB_COLOR=4:a0 */
/* @SymbolStart: 0x80132138 */
/* @SymbolSize: 308 */
/* @PostProc: patch_insn_reg_field 0x50:rt:t6:v1 0x54:rs:t6:v1 0x54:rt:t7:v1 0x5c:rt:t7:v1 0x64:rt:t8:a0 0x6c:rt:t8:a0 0x74:rs:a0:v0 0x74:rt:v0:a0 0x78:rt:t9:t6 0x7c:rs:t9:t6 0x7c:rt:t9:t6 0x84:rs:t9:t6 0x84:rt:t0:t7 0x88:rt:t0:t7 0xb4:rt:v0:v1 0xb8:rs:v0:v1 0xb8:rt:v0:v1 0xc0:rs:v0:v1 0xc8:rs:v0:v1 0xc8:rt:v0:v1 0xcc:rs:v0:v1 0xd0:rt:v0:v1 0xdc:rt:t1:v1 0xe0:rs:t1:v1 0xe0:rt:t1:v1 0xe4:rt:t2:t8 0xe8:rs:t1:v1 0xf0:rs:t2:t8 0xf0:rt:t2:t8 0xf8:rs:t2:t8 0xf8:rt:t3:t9 0xfc:rs:t3:t9 */
/* tel_yoshio_ring - TEL overlay, Address: 0x80132138 (head) / 0x80132140, Size: 308 bytes (77 insns) */
#include "types.h"

extern u8 step_mode;
extern s32 work_fuku;
extern volatile s32 work_fuku_v;
extern s32 step_counter;
extern s32 pad_trig;
extern u8 tel_yoshio_ring_str[];
extern u8 tel_yoshio_ring_again_str[];

extern void sndi(s32, s32, s32);
extern void device_cmd_send(s32);
extern void k_sub_disp_start(s32);
extern s32 rand(void);
extern void k_sub_reset(void);
extern void sub_sub_step_up(void);
extern void menu_bar_show(s32);
extern void cal_base_show(void);
extern void k_disp_inc(void);

void tel_yoshio_ring(void)
{
    s32 a0 = step_mode;
    s32 v1;

    if (a0 != 0) {
        goto L22;
    }

    sndi((s32)tel_yoshio_ring_str, 0, 47);
    device_cmd_send(1288);
    k_sub_disp_start(0);
    v1 = rand();
    v1 = (v1 & 3) + 1;
    work_fuku = v1;
    step_mode = 1;
    goto L65;

L22:
    if (a0 != 1) {
        goto L65;
    }
    if ((step_counter & 0x7F) != 1) {
        goto L49;
    }

    k_sub_reset();
    sndi((s32)tel_yoshio_ring_again_str, 0, 47);
    k_sub_disp_start(0);

    v1 = work_fuku_v;
    if (v1 != 0) {
        v1 = v1 - 1;
    }
    work_fuku = v1;
    if (v1 > 0) {
        device_cmd_send(1288);
    }

L49:
    v1 = work_fuku;
    if (v1 <= 0) {
        goto L59;
    }
    if ((pad_trig & 0x40) == 0) {
        goto L65;
    }

L59:
    device_cmd_send(5384);
    device_cmd_send(1291);
    sub_sub_step_up();

L65:
    menu_bar_show(0);
    cal_base_show();
    k_disp_inc();
}
