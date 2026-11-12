/* Address: 0x80137E2C (true entry 0x80137E20 -- 3-word cross-prologue head) */
/* @SymbolStart: 0x80137E20 */
/* @SymbolSize: 404 */
/* @PostProc: patch_stack_complete --old 40 --new 56 */
/* @PostProc: remap_stack_slots --from 24 --to 44 --from 32 --to 48 */
/* @PostProc: patch_insn_reg_field 0xac:rt:t7:t8 0xb8:rd:t8:t9 0xb8:rt:t7:t8 0xbc:rt:t8:t9 0xd4:rt:t0:t1 0xd8:rs:t0:t1 0xd8:rt:t0:t3 0xdc:rt:t9:t0 0xe0:rd:t1:t2 0xe0:rt:t0:t3 0xe4:rs:t9:t0 0xe4:rt:t1:t2 0xfc:rt:t3:t4 0x100:rs:t3:t4 0x100:rt:t3:t4 0x108:rs:t3:t4 0x108:rt:t2:t5 0x10c:rt:t2:t5 0x128:rt:t4:t7 0x12c:rs:t4:t7 0x144:rt:t5:t8 0x14c:rs:t5:t8 0x150:rt:t7:t6 0x154:rs:t7:t6 0x154:rt:t7:t6 0x15c:rs:t7:t6 0x15c:rt:t6:t9 0x160:rs:t6:t9 0x180:rt:t8:t1 0x188:rs:t8:t1 */
#include "types.h"

extern s32 option_talk_xa_flag;
extern s8 menu_csr;
extern s32 option_talk_xa_tbl[];
extern s32 work_fuku;
extern u8 option_talk_xa_sect_fmt[];
extern u8 now_sub_sub_step;
extern s32 pad_trig;

extern s32 get_CD_State(void);
extern void set_XAsector(s32 a0, s32 a1, s32 a2);
extern void printf(u8 *fmt, s32 a1, s32 a2);
extern void device_cmd_send(s32 a0);
extern void goto_sub_sub_step(s32 a0);
extern void option_menu_box_show(void);

void option_talk_xa_main(void)
{
    s32 st;
    s32 idx;
    s32 *e;

    st = option_talk_xa_flag;
    idx = menu_csr;

    if (st == 0) {
        if ((get_CD_State() & 4) != 0) {
            option_talk_xa_flag = 1;
        }
    }

    if (option_talk_xa_flag != 0) {
        if ((u32)work_fuku < *(u32 *)option_talk_xa_tbl[idx]) {
            if ((get_CD_State() & 2) != 0) {
                option_talk_xa_flag = 0;
                e = (s32 *)(option_talk_xa_tbl[idx] + work_fuku * 8);
                set_XAsector(e[1], e[2], idx);
                e = (s32 *)(option_talk_xa_tbl[idx] + work_fuku * 8);
                printf(option_talk_xa_sect_fmt, e[1], e[2]);
                work_fuku = work_fuku + 1;
                device_cmd_send(768);
            }
        } else {
            if ((get_CD_State() & 2) != 0) {
                work_fuku = 0;
                goto_sub_sub_step((u8)(now_sub_sub_step - 1));
            }
        }
    }

    if ((pad_trig & 0x40) != 0) {
        work_fuku = 0;
        device_cmd_send(116);
        goto_sub_sub_step((u8)(now_sub_sub_step - 1));
    }

    option_menu_box_show();
}
