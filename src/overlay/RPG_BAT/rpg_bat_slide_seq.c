/* @SymbolStart: 0x80135E80 */
/* @RodataBase: 0x8015AE7C */
/* rpg_bat_slide_seq -- RPG_BAT overlay, Address: 0x80135E88, Size: 476 bytes */
/* @SymbolSize: 484 */
/* @PostProc: scheduler_normalizer --const-fp --store-fp --branch-fp */
/* @PostProc: patch_insn_reg_field 0x68:rt:v0:a3 0x6c:rs:v0:a3 0x6c:rt:v0:a3 0x78:rs:v0:a3 0x78:rt:v0:a3 0x7c:rt:v0:a3 0x80:rt:v0:a3 0x90:rt:t8:a3 0x94:rs:t8:a3 0x94:rt:t8:a3 0x9c:rs:t8:a3 0x168:rt:v0:a3 0x16c:rs:v0:a3 0x16c:rt:v0:a3 0x174:rs:v0:a3 0x188:rt:t9:a3 0x18c:rs:t9:a3 0x18c:rt:t9:a3 0x194:rs:t9:a3 0x1d8:rt:t0:t8 0x1e0:rt:t0:t8 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"


extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern void rpg_bat_act_next(void);
extern void rpg_bat_sprite_pos_set(s32, s32, s32);
extern void rpg_bat_act_wait(s32);
extern void strcpy(u8 *, u8 *);
extern void rpg_bat_koe_set(s32, s32, s32);
extern void rpg_bat_msg_set(u8 *, s32, s32);
extern void rpg_bat_act_gate(s32);
extern void rpg_bat_reset(void);

extern u8  rpg_bat_msg_buf3[];
extern u8  rpg_bat_slide_seq_str0[];
extern u8  rpg_bat_msg_tbl3[];
extern RPG_BAT_UNIT rpg_bat_unit2;

void rpg_bat_slide_seq(void)
{
    s32 n;

    switch (rpg_bat_unit2.step) {
    case 0:
        rpg_bat_anim_set(29, 1, 1, 1);
        rpg_bat_act_next();
        break;
    case 1:
        n = rpg_bat_unit2.cnt + 1;
        rpg_bat_unit2.cnt = n;
        rpg_bat_sprite_pos_set(29, 64 - n, 0);
        if (rpg_bat_unit2.cnt >= 65) {
            rpg_bat_sprite_pos_set(29, 0, 0);
            rpg_bat_anim_set(29, 0, 1, 1);
            rpg_bat_act_next();
        }
        break;
    case 2:
        rpg_bat_act_wait(20);
        break;
    case 3:
        strcpy(rpg_bat_msg_buf3, rpg_bat_slide_seq_str0);
        rpg_bat_koe_set(0x1F00008A, 0x1900008C, 0);
        rpg_bat_msg_set(rpg_bat_msg_tbl3, 1, 70);
        rpg_bat_act_next();
        break;
    case 4:
        rpg_bat_act_wait(10);
        break;
    case 5:
        rpg_bat_anim_set(29, 6, 1, 1);
        rpg_bat_act_next();
        break;
    case 6:
        n = rpg_bat_unit2.cnt + 1;
        rpg_bat_unit2.cnt = n;
        rpg_bat_sprite_pos_set(29, n, 0);
        if (rpg_bat_unit2.cnt >= 65) {
            rpg_bat_act_next();
        }
        break;
    case 7:
        rpg_bat_act_gate(0);
        break;
    case 8:
        rpg_bat_act_wait(20);
        break;
    case 9:
        rpg_bat_reset();
        rpg_bat_unit2.flag = 0x40000000;
        break;
    }
}
