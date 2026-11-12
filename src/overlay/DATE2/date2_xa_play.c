/* @PostProc: swap_instructions --offset1 0xd4 --offset2 0xe0 */
/* @PostProc: patch_insn_reg_field 0x50:rt:a0:a1 0x54:rt:a0:a1 0x58:rt:a1:a0 0x5c:rs:a1:a0 0x5c:rt:a1:a0 0x68:rt:t0:t1 0x6c:rd:v1:t8 0x70:rd:v0:v1 0x70:rs:a1:a0 0x70:rt:v1:t8 0x74:rs:v0:v1 0x74:rt:t8:t9 0x7c:rs:t8:t9 0x7c:rt:t9:t0 0x80:rs:v0:v1 0x80:rt:t9:t0 0x84:rs:t0:t1 0x84:rt:t0:t1 0x8c:rs:t0:t1 0x8c:rt:a0:a1 0x90:rd:v1:t2 0x94:rd:v0:v1 0x94:rs:a1:a0 0x94:rt:v1:t2 0x98:rs:v0:v1 0x98:rt:t1:t3 0xa0:rs:t1:t3 0xa0:rt:t2:t4 0xa8:rs:v0:v1 0xa8:rt:t2:t4 0xb4:rt:t3:t5 0xb8:rs:t3:t5 0xb8:rt:t3:t5 0xc4:rd:t4:t6 0xc4:rs:t3:t5 0xc8:rs:t4:t6 0xcc:rt:t5:t7 0xd0:rs:t5:t7 0xd0:rt:t5:t7 0xd8:rs:t5:t7 0xf0:rt:v1:t8 0xf4:rt:t6:t9 0xf8:rs:t6:t9 0xf8:rt:t6:t9 0xfc:rs:v1:t8 0xfc:rt:t7:t0 0x100:rs:v1:t8 0x100:rt:t9:t2 0x104:rs:t9:t2 0x104:rt:t9:t2 0x108:rs:t7:t0 0x108:rt:t7:t0 0x10c:rd:t0:t3 0x10c:rt:t9:t2 0x110:rd:t8:t1 0x110:rs:t6:t9 0x110:rt:t7:t0 0x114:rd:t1:t4 0x114:rs:t8:t1 0x114:rt:t0:t3 0x118:rs:t1:t4 0x118:rt:t2:t5 0x124:rt:t2:t5 0x138:rt:t4:t7 0x13c:rs:t4:t7 0x13c:rt:t4:t7 0x140:rt:a0:a1 0x144:rs:t4:t7 0x144:rt:a0:a1 0x148:rt:a1:a0 0x14c:rs:a1:a0 0x14c:rt:a1:a0 0x150:rt:t3:t6 0x158:rt:t3:t6 0x15c:rd:v1:t8 0x160:rd:v0:v1 0x160:rs:a1:a0 0x160:rt:v1:t8 0x164:rs:v0:v1 0x164:rt:t5:t9 0x16c:rs:t5:t9 0x16c:rt:t6:t0 0x174:rs:v0:v1 0x174:rt:t6:t0 0x180:rt:t7:t2 0x184:rs:t7:t2 0x18c:rt:t9:t1 0x190:rs:t9:t1 0x190:rt:t9:t1 0x198:rs:t9:t1 0x198:rt:t8:t3 0x19c:rt:t8:t3 */
/* Address: 0x80138BEC, Size: 512 bytes (128 insns) */
#include "types.h"

extern volatile u8 now_girl;
extern u8 screen_w[];
extern s32 pad_trig;
extern s32 serifu_no;
extern s16 date2_xa_timer;
extern s16 date2_song_now_s;
extern u8 date2_sched_no;

extern s32 get_CD_l_com(void);
extern void sub_sub_step_up(void);
extern void param_clamp(void);
extern s32 get_g_zyotai_s(s32 arg0);
extern void k_reset(s32 arg0);
extern void device_cmd_send(s32 arg0);
extern s32 get_CD_State(void);
extern void date2_sched_show2(void);
extern void date2_sched_show0(void);
extern void date2_sched_show3(void);
extern void date2_sched_show1(void);

void date2_xa_play(void)
{
    u32 off;
    s32 r;

    if (get_CD_l_com() == 9 && date2_xa_timer >= 121) {
        sub_sub_step_up();
        serifu_no = 0;
        off = (u32)now_girl * 56;
        *(s16 *)(screen_w + off + 446) =
            *(s16 *)(screen_w + off + 446) + 2;
        off = (u32)now_girl * 56;
        *(s16 *)(screen_w + off + 450) =
            *(s16 *)(screen_w + off + 450) + 1;
        param_clamp();
    } else if ((pad_trig & 0x00400040) != 0 && date2_xa_timer >= 121) {
        r = get_g_zyotai_s((s32)now_girl) & 0x7F;
        date2_song_now_s = (s16)(date2_song_now_s + (r == 2) + (r == 3) * 2 + 1);
        sub_sub_step_up();
        k_reset(1);
        device_cmd_send(116);
        serifu_no = 1;
        off = (u32)now_girl * 56;
        *(s16 *)(screen_w + off + 450) =
            *(s16 *)(screen_w + off + 450) + 1;
        param_clamp();
    }

    if ((get_CD_State() & 4) != 0) {
        date2_xa_timer = (s16)(date2_xa_timer + 1);
    }

    switch (date2_sched_no) {
    case 2:
        date2_sched_show2();
        break;
    case 0:
        date2_sched_show0();
        break;
    case 3:
        date2_sched_show3();
        break;
    default:
        date2_sched_show1();
        break;
    }
}
