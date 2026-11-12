/* @SymbolStart: 0x8013754C */
/* @SymbolSize: 244 */
/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x9c */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:t6 0x14:rs:v1:t6 0x14:rt:v1:t6 0x20:rt:a1:a0 0x24:rs:v1:t6 0x24:rt:t6:t7 0x2c:rt:t6:t7 0x30:rs:a1:a0 0x30:rt:a1:a0 0x38:rt:a2:a1 0x3c:rt:a2:a1 0x44:rd:t9:t1 0x48:rt:t9:t1 0x50:rd:t7:t8 0x54:rs:a1:a0 0x54:rt:t7:t8 0x58:rt:a0:t9 0x60:rs:a0:t9 0x60:rt:t8:t0 0x68:rt:t8:t0 0x6c:rt:v0:t3 0x70:rs:v0:t3 0x70:rt:v0:t3 0x74:rt:a3:a2 0x78:rs:v0:t3 0x80:rt:t1:t4 0x8c:rs:t1:t4 0x8c:rt:t1:t4 0x94:rs:t1:t4 0x94:rt:t2:t5 0x98:rt:t2:t5 0xa0:rt:a2:a1 0xa8:rd:t5:t9 0xac:rt:t5:t9 0xb4:rd:t3:t6 0xb8:rs:a1:a0 0xb8:rt:t3:t6 0xbc:rt:a0:t7 0xc4:rd:t4:t8 0xc4:rs:a0:t7 0xc4:rt:a3:a2 0xcc:rt:t4:t8 0xd0:rt:t7:t1 0xdc:rt:t7:t1 */
/* kangei_class_fade_out - KANGEI overlay, Address: 0x80137554, Size: 236 bytes (59 insns). */
#include "types.h"

extern void prim_init(void);
extern void device_port_init(s32, s32);
extern void sub_sub_step_up(void);

extern volatile u8 cal_cw_bright;
extern u8 cal_cw_disp;
extern u8 update_anim_state_tbl4;

extern u8 class_w[];

void kangei_class_fade_out(void)
{
    s32 t6_in = cal_cw_bright;

    u8 counter;
    s16 i;
    u8 *entry;
    u8 b7;
    u8 b3;

    cal_cw_bright = (u8)(t6_in - 8);

    i = 97;
    do {
        entry = class_w + (s32)i * 68;
        b7 = entry[7];
        entry[7] = (u8)(b7 - 8);
        i++;
    } while (i < 116);

    counter = cal_cw_bright;
    if ((u32)counter < 9U) {
        cal_cw_bright = 0;
        cal_cw_disp = (u8)(cal_cw_disp & 0xFF7F);

        i = 97;
        do {
            entry = class_w + (s32)i * 68;
            b3 = entry[3];
            entry[7] = 0;
            entry[3] = (u8)(b3 & ~0x80);
            i++;
        } while (i < 116);

        update_anim_state_tbl4 = 15;
        prim_init();
        device_port_init(0, 512);
        sub_sub_step_up();
    }
}
