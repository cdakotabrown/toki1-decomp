/* @RodataBase: 0x8015BA1C */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0x184 --offset2 0x18c */
/* @PostProc: swap_instructions --offset1 0x190 --offset2 0x194 */
/* @PostProc: swap_instructions --offset1 0x1d4 --offset2 0x1e4 */
/* @PostProc: swap_instructions --offset1 0x1d8 --offset2 0x1dc */
/* @PostProc: swap_instructions --offset1 0x1e0 --offset2 0x1e4 */
/* @PostProc: patch_insn_reg_field 0xa8:rt:v0:t9 0xac:rs:v0:t9 0xac:rt:v0:t9 0xb4:rt:v0:t9 0xb8:rt:t9:t0 0xbc:rs:t9:t0 0xbc:rt:t9:t0 0xc4:rt:t9:t0 0xc8:rt:t0:t1 0xcc:rs:t0:t1 0xcc:rt:t0:t1 0xd4:rt:t0:t1 0xdc:rt:t1:t2 0xe0:rt:t1:t2 0xe8:rt:t2:t3 0xec:rt:t2:t3 0xf4:rt:t3:t4 0xf8:rt:t3:t4 0x100:rt:t4:t5 0x104:rt:t4:t5 0x10c:rt:t5:t6 0x110:rt:t5:t6 0x114:rs:v0:t9 0x124:rt:t6:t8 0x128:rs:t6:t8 0x128:rt:t6:t8 0x130:rs:t6:t8 0x13c:rt:t7:t9 0x140:rs:t7:t9 0x140:rt:t7:t9 0x148:rs:t7:t9 0x154:rt:t8:t0 0x158:rs:t8:t0 0x158:rt:t8:t0 0x160:rs:t8:t0 0x16c:rt:t9:t1 0x170:rs:t9:t1 0x170:rt:t9:t1 0x178:rs:t9:t1 0x184:rt:a3:a1 0x188:rt:a2:a0 0x18c:rt:a1:v1 0x190:rt:v1:v0 0x194:rt:a0:a2 0x198:rs:a1:v1 0x198:rt:a1:v1 0x19c:rs:a2:a0 0x19c:rt:a2:a0 0x1a0:rs:a3:a1 0x1a0:rt:a3:a1 0x1a4:rs:a0:a2 0x1a4:rt:a0:a2 0x1a8:rs:v1:v0 0x1a8:rt:v1:v0 0x1ac:rs:v1:v0 0x1ac:rt:v1:v0 0x1b0:rs:v1:v0 0x1b0:rt:a0:a2 0x1b4:rs:v1:v0 0x1b4:rt:a2:a0 0x1b8:rs:v1:v0 0x1b8:rt:a3:a1 0x1c0:rs:v1:v0 0x1c0:rt:a1:v1 0x1c4:rt:t0:t2 0x1c8:rs:t0:t2 0x1c8:rt:t0:t2 0x1d0:rs:t0:t2 0x1d4:rt:v1:v0 0x1d8:rt:a3:a1 0x1dc:rt:a2:a0 0x1e0:rt:a1:v1 0x1e4:rt:a0:a3 0x1e8:rs:a1:v1 0x1e8:rt:a1:v1 0x1ec:rs:a2:a0 0x1ec:rt:a2:a0 0x1f0:rs:a3:a1 0x1f0:rt:a3:a1 0x1f4:rs:a0:a3 0x1f4:rt:a0:a3 0x1f8:rs:v1:v0 0x1f8:rt:v1:v0 0x1fc:rt:v0:a2 0x200:rs:v1:v0 0x200:rt:v1:v0 0x204:rs:v1:v0 0x204:rt:a1:v1 0x208:rs:v1:v0 0x208:rt:a2:a0 0x20c:rs:v1:v0 0x20c:rt:a3:a1 0x210:rs:v1:v0 0x210:rt:a0:a3 0x214:rs:v1:v0 0x214:rt:v0:a2 0x228:rt:t1:t3 0x238:rt:t1:t3 0x2ac:rt:t2:t4 0x2b0:rs:t2:t4 */
/* @SymbolStart: 0x8013A034 */
/* @SymbolSize: 704 */
/* rpg_bat_load - RPG_BAT overlay, Address: 0x8013A03C, Size: 696 bytes (174 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern s32 device_cmd_send();
extern s32 get_CD_State2();
extern s32 get_CD_State();
extern s32 set_XAsector();
extern s32 cd_read_start2();
extern s32 load_tpage_buf_lock();
extern s32 rpg_bat_seq_next();
extern s32 rpg_bat_sub_next();

extern s32  rpg_bat_pause_cnt;
extern s32 *rpg_bat_res_hdr;
extern s32 *rpg_bat_res_img;
extern s32 *rpg_bat_res_anim;
extern u8   class_w[];
extern s32  rpg_bat_girl;
extern s32  rpg_bat_res_img_5;
extern s32  rpg_bat_res_anim_5;
extern s32  rpg_bat_res_hdr_5;
extern u8   now_girl;
extern s32 tpage_buf[];
extern RPG_BAT_UNIT rpg_bat_unit3;

void rpg_bat_load(void)
{
    u8 *p;
    u8 *lim;
    s32 *q0;
    s32 *q1;
    s32 *q2;
    s32 w0;
    s32 w1;
    s32 w2;

    switch ((u32)rpg_bat_unit3.sub) {
    case 0:
        cd_read_start2(43, 0x801B1000, 16012);
        rpg_bat_sub_next();
        break;

    case 1:
        if (rpg_bat_pause_cnt == 0) {
            rpg_bat_seq_next();
        }
        if ((get_CD_State2() & 1) != 0) {
            rpg_bat_pause_cnt = 0;
            rpg_bat_sub_next();
        }
        break;

    case 2:
        rpg_bat_res_hdr = (s32 *)0x801C5DC8;
        rpg_bat_res_img = (s32 *)0x801C5DDC;
        rpg_bat_res_anim = (s32 *)0x801C5EE4;
        tpage_buf[21] = -1;
        tpage_buf[22] = -1;
        tpage_buf[23] = -1;
        tpage_buf[24] = -1;
        tpage_buf[25] = -1;
        load_tpage_buf_lock(21, rpg_bat_res_hdr[0], 1);
        load_tpage_buf_lock(22, rpg_bat_res_hdr[1], 1);
        load_tpage_buf_lock(23, rpg_bat_res_hdr[2], 1);
        load_tpage_buf_lock(24, rpg_bat_res_hdr[3], 1);
        load_tpage_buf_lock(25, rpg_bat_res_hdr[4], 1);
        q0 = rpg_bat_res_hdr;
        q1 = rpg_bat_res_img;
        q2 = rpg_bat_res_anim;
        lim = (&((CLASS_W *)class_w)[72].active);
        p = class_w;
        do {
            p += 68;
            *(s32 **)(p + 4296) = q1;
            *(s32 **)(p + 4300) = q2;
            *(s32 **)(p + 4336) = q0;
        } while (p < lim);
        if (rpg_bat_girl != 14) {
            w0 = rpg_bat_res_img_5;
            w1 = rpg_bat_res_anim_5;
            w2 = rpg_bat_res_hdr_5;
            lim = (&((CLASS_W *)class_w)[31].active);
            p = (&((CLASS_W *)class_w)[29].active);
            do {
                p += 68;
                *(s32 *)(p + 6472) = w0;
                *(s32 *)(p + 6476) = w1;
                *(s32 *)(p + 6512) = w2;
                *(u8 *)(p + 6527) = 28;
            } while (p != lim);
        }
        rpg_bat_sub_next();
        break;

    case 3:
        now_girl = 14;
        set_XAsector(0x1C0000A8, 0x0F0000B7, 14);
        device_cmd_send(768);
        rpg_bat_sub_next();
        break;

    case 4:
        device_cmd_send(228);
        rpg_bat_sub_next();
        break;

    case 5:
        rpg_bat_unit3.wait = rpg_bat_unit3.wait + 1;
        if (rpg_bat_unit3.wait >= 181) {
            rpg_bat_seq_next();
        }
        if ((get_CD_State() & 4) != 0) {
            rpg_bat_seq_next();
        }
        break;
    }
}
