/* @PostProc: patch_insn_reg_field 0x68:rt:t6:t8 0x8c:rt:t0:t1 0x94:rs:t0:t1 0x94:rt:t1:t2 0x98:rt:t1:t2 0x9c:rt:t3:t4 0xa0:rs:t3:t4 0xa0:rt:t3:t4 0xa8:rt:t2:t3 0xac:rt:t3:t4 0xb0:rt:t2:t3 0xb4:rt:t4:t5 0xb8:rs:t4:t5 0xb8:rt:t4:t6 0xc0:rs:t4:t6 0xc0:rt:t5:t7 0xc8:rt:t5:t7 0xcc:rs:t6:t8 0xcc:rt:t6:t8 0xd4:rs:t6:t8 0xdc:rt:t7:t9 0x104:rs:t7:t9 0x104:rt:t7:t9 0x10c:rs:t7:t9 0x128:rt:t8:t0 0x12c:rs:t8:t0 0x12c:rt:t8:t1 0x134:rs:t8:t1 0x134:rt:t9:t2 0x138:rt:t9:t2 */
/* @SymbolSize: 408 */
/* Address: 0x80147D60 */
/* Size: 408 bytes (102 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_port_init(s32, s32);
extern void k_sub_reset_point_set(void);
extern void k_sub_disp_start(s32);
extern void gnsx(void *);
extern void sndisp(void *, s32, s32);
extern void set_flag_b3_high_bit(s32);
extern void etc_panel_disp(void);
extern void etc_shade_disp(void);
extern void etc_box_shade_show(void);

extern volatile u8 now_day;
extern volatile u8 cal_cell_tbl_1;
extern volatile u8 byouki_state;
extern volatile s32 step_counter;
extern s32 etc_rank_low;
extern u8 p_sei;
extern u8 etc_week_result_disp_str0;
extern u8 etc_week_result_disp_str1;
extern u8 etc_week_result_disp_str2;
extern u8 etc_week_result_disp_str3;
extern s32 work_fuku[];
extern s32 etc_rank_juni[];

void etc_week_result_disp(void) {
    etc_panel_disp();
    byouki_state = (u8)(byouki_state & 0xFFFD);
    if ((u32)step_counter >= 121) {
        gnsx(&p_sei);
        if (work_fuku[9] >= 3) {
            sndisp(&etc_week_result_disp_str0, 0, 31);
            device_port_init(1, 515);
            byouki_state |= 1;
            (&cal_cell_tbl_1)[now_day] = 23;
            byouki_state |= 2;
        } else if (etc_rank_juni[13] < 2) {
            sndisp(&etc_week_result_disp_str1, 0, 31);
            device_port_init(1, 516);
        } else if (etc_rank_low != 0) {
            sndisp(&etc_week_result_disp_str2, 0, 31);
            byouki_state |= 2;
            device_port_init(1, 512);
        } else {
            sndisp(&etc_week_result_disp_str3, 0, 31);
            device_port_init(1, 513);
        }
        k_sub_reset_point_set();
        k_sub_disp_start(0);
        set_flag_b3_high_bit(0);
        sub_sub_step_up();
    }
    etc_shade_disp();
    etc_box_shade_show();
}
