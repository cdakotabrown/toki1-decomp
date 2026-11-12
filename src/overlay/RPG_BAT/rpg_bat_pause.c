/* @SymbolStart: 0x8013BEA8 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=9 */
/* @SymbolSize: 804 */
/* @PostProc: patch_insn_reg_field 0x30:rt:v0:t7 0x34:rs:v0:t7 0x34:rt:v0:t7 0x3c:rs:v0:t7 0x3c:rt:v0:t8 0x40:rs:v0:t8 0x44:rt:v0:t8 0x50:rt:t7:t9 0x54:rs:t7:t9 0x54:rt:t7:t9 0x5c:rs:t7:t9 0x60:rt:t8:t0 0x64:rs:t8:t0 0x64:rt:t8:t0 0x6c:rs:t8:t0 0x7c:rt:t9:t1 0x80:rs:t9:t1 0x90:rt:t0:t2 0x94:rs:t0:t2 0xc4:rt:t1:t3 0xd0:rt:t1:t3 0xe4:rt:t2:t4 0xe8:rs:t2:t4 0xe8:rt:t2:t4 0xf0:rs:t2:t4 0xf4:rt:t3:t5 0xf8:rs:t3:t5 0xf8:rt:t3:t5 0x100:rs:t3:t5 0x108:rt:t4:t6 0x10c:rs:t4:t6 0x10c:rt:t4:t6 0x110:rt:t6:t8 0x114:rs:t4:t6 0x114:rt:t5:t7 0x118:rs:t5:t7 0x120:rs:t6:t8 0x120:rt:t6:t8 0x124:rt:t7:t9 0x128:rs:t6:t8 0x130:rs:t7:t9 0x130:rt:t7:t9 0x134:rt:t9:t1 0x138:rs:t7:t9 0x138:rt:t8:t0 0x13c:rs:t8:t0 0x144:rs:t9:t1 0x144:rt:t9:t1 0x148:rt:t1:t3 0x14c:rs:t9:t1 0x14c:rt:t0:t2 0x150:rs:t0:t2 0x158:rs:t1:t3 0x158:rt:t1:t3 0x15c:rt:t3:t5 0x160:rs:t1:t3 0x160:rt:t2:t4 0x164:rs:t2:t4 0x16c:rs:t3:t5 0x16c:rt:t3:t5 0x170:rt:t5:t7 0x174:rs:t3:t5 0x174:rt:t4:t6 0x178:rs:t4:t6 0x180:rs:t5:t7 0x180:rt:t5:t7 0x188:rs:t5:t7 0x18c:rt:t6:t8 0x190:rs:t6:t8 0x190:rt:t6:t8 0x194:rt:t7:t9 0x198:rs:t6:t8 0x1a0:rs:t7:t9 0x1a0:rt:t7:t9 0x1a8:rs:t7:t9 0x1b8:rt:t8:t0 0x1bc:rs:t8:t0 0x1bc:rt:t8:t0 0x1c4:rs:t8:t0 0x1d0:rt:t9:t1 0x1d4:rs:t9:t1 0x1d4:rt:t9:t1 0x1d8:rt:t0:t2 0x1dc:rs:t9:t1 0x1e8:rs:t0:t2 0x1e8:rt:t0:t2 0x1f0:rs:t0:t2 0x1f0:rt:t1:t3 0x1f4:rt:t1:t3 0x200:rt:t2:t4 0x208:rt:t2:t4 0x238:rt:t3:t5 0x244:rt:t3:t5 0x248:rt:t4:t6 0x24c:rs:t4:t6 0x24c:rt:t4:t6 0x254:rs:t4:t6 0x280:rt:t5:t7 0x28c:rt:t5:t7 0x298:rt:t6:t8 0x29c:rs:t6:t8 0x29c:rt:t6:t8 0x2a4:rs:t6:t8 0x2b0:rt:t7:t9 0x2b4:rs:t7:t9 0x2b4:rt:t7:t9 0x2b8:rt:t8:t0 0x2bc:rs:t7:t9 0x2c8:rs:t8:t0 0x2c8:rt:t8:t0 0x2d0:rs:t8:t0 0x2d0:rt:t9:t1 0x2d4:rt:t9:t1 0x2e0:rt:t0:t2 0x2e8:rt:t0:t2 0x318:rt:t1:t3 0x320:rt:t1:t3 */
/* Address: 0x8013BEB0 (true start 0x8013BEA8) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 pad_get();
extern s32 device_cmd_send();
extern s32 rpg_bat_pad_seq_poll();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_se_set();

extern s32 rpg_bat_pause_mode;
extern s32 rpg_bat_pause_cnt;
extern u32 pad_trig;
extern s32 rpg_bat_girl;
extern u8 now_sub_step;
extern s32 rpg_bat_msg_ctl;
extern s32 rpg_bat_msg_ctl_cnt;
extern s32 rpg_bat_msg_time;
extern s32 rpg_bat_pad_seq_work[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit2;
extern RPG_BAT_UNIT rpg_bat_unit3;

void rpg_bat_pause(void)
{
    s32 mode;
    s32 t;

    mode = rpg_bat_pause_mode;
    if (mode == 3) {
        rpg_bat_pad_seq_poll();
    }

    t = rpg_bat_pause_cnt - 1;
    rpg_bat_pause_cnt = t;
    if (t < 0) {
        rpg_bat_pause_cnt = 0;
    }

    while (rpg_bat_pause_mode == 1 && rpg_bat_pause_cnt == 0) {
        if ((pad_get() & 0x800) == 0) {
            for (;;) {
                if ((pad_get() & 0x800) != 0) {
                    rpg_bat_se_set(1280, 1, 0);
                    rpg_bat_pause_mode = 0;
                    rpg_bat_anim_set(28, 51, 1, 0);
                    rpg_bat_pause_cnt = 2;
                    break;
                }
                rpg_bat_pad_seq_poll();
            }
        }
    }

    if ((pad_trig & 0x800) == 0) {
        return;
    }
    if (rpg_bat_pause_cnt != 0) {
        return;
    }

    if ((rpg_bat_unit1.flag & 0xFF) == 0 && (rpg_bat_unit0.flag & 0xFF) == 0 &&
        (rpg_bat_unit2.flag & 0xFF) == 0 && (rpg_bat_unit3.flag & 0xFF) == 0 &&
        rpg_bat_girl == 9 && rpg_bat_pause_mode == 0 && now_sub_step == 9) {
        device_cmd_send(116);
        if (rpg_bat_msg_ctl >= 3 && rpg_bat_msg_ctl_cnt >= 3) {
            rpg_bat_msg_ctl_cnt = rpg_bat_msg_time - 5;
        }
        rpg_bat_se_set(1280, 1, 0);
        rpg_bat_pause_mode = 3;
        rpg_bat_pad_seq_work[3] = 0;
        rpg_bat_pad_seq_work[2] = 0;
        rpg_bat_pad_seq_work[1] = 0;
        rpg_bat_anim_set(28, 51, 1, 1);
        rpg_bat_pause_cnt = 2;
        return;
    }

    if (rpg_bat_pause_mode != 0) {
        rpg_bat_se_set(1280, 1, 0);
        rpg_bat_pause_mode = 0;
        rpg_bat_anim_set(28, 51, 1, 0);
        rpg_bat_pause_cnt = 2;
        return;
    }

    device_cmd_send(116);
    if (rpg_bat_msg_ctl >= 3 && rpg_bat_msg_ctl_cnt >= 3) {
        rpg_bat_msg_ctl_cnt = rpg_bat_msg_time - 5;
    }
    rpg_bat_se_set(1280, 1, 0);
    rpg_bat_pause_mode = 1;
    rpg_bat_pad_seq_work[3] = 0;
    rpg_bat_pad_seq_work[2] = 0;
    rpg_bat_pad_seq_work[1] = 0;
    rpg_bat_anim_set(28, 51, 1, 1);
    rpg_bat_pause_cnt = 2;
}
