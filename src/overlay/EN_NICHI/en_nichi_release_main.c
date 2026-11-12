/* @CompileEnv: IDO52_PRECOLOR_WEB=a2:17 */
/* @PostProc: swap_instructions --offset1 0xd8 --offset2 0xe0 */
/* @PostProc: swap_instructions --offset1 0x110 --offset2 0x114 */
/* @PostProc: swap_branch_operands --offsets 0x120,0x144,0x168,0x18c,0x1ac,0x1d0,0x1f4,0x218 */
/* @PostProc: patch_insn_reg_field 0xe8:rt:t0:t1 0xf8:rs:t0:t1 0xf8:rt:t0:t1 0x110:rt:t1:t2 0x114:rt:t2:t0 0x118:rt:v1:v0 0x120:rt:v1:v0 0x128:rt:t1:t2 0x138:rd:v1:v0 0x138:rs:t0:t1 0x13c:rs:v1:v0 0x13c:rt:v0:v1 0x144:rs:t2:t0 0x144:rt:v0:v1 0x148:rs:v0:v1 0x164:rs:v1:v0 0x168:rt:v1:v0 0x170:rt:t1:t2 0x180:rd:v1:v0 0x180:rs:t0:t1 0x184:rs:v1:v0 0x184:rt:v0:v1 0x18c:rs:t2:t0 0x18c:rt:v0:v1 0x190:rs:v0:v1 0x1a0:rs:v1:v0 0x1b4:rt:t1:t2 0x1c4:rd:v1:v0 0x1c4:rs:t0:t1 0x1c8:rs:v1:v0 0x1c8:rt:v0:v1 0x1d0:rs:t2:t0 0x1d0:rt:v0:v1 0x1d4:rs:v0:v1 0x1f0:rs:v1:v0 0x1fc:rt:t1:t2 0x20c:rd:v1:v0 0x20c:rs:t0:t1 0x210:rs:v1:v0 0x210:rt:v0:v1 0x218:rs:t2:t0 0x218:rt:v0:v1 0x21c:rs:v0:v1 0x22c:rs:v1:v0 */
/* en_nichi_release_main - EN_NICHI overlay, Address: 0x80133708, Size: 596 bytes (149 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern void en_nichi_cursor_move();
extern void en_nichi_marker_add();
extern void en_nichi_cmd_send_once();
extern void en_nichi_near_set();

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 pad_data;
extern u8 en_nichi_obj_wk[];
extern s32 en_nichi_poi_wear;
extern s32 en_nichi_action_step;
extern volatile s32 en_nichi_action_timer;
extern s32 en_nichi_get_cnt;
extern s32 en_nichi_rare_cnt;
extern u8 en_nichi_near_tbl[10];
extern CLASS_W class_w[];

s32 en_nichi_release_main(void)
{
    s32 v0;
    s32 a2;
    s32 cnt;
    s32 ctrl;
    s32 a0v;
    s32 a1v;
    s32 v1;
    s32 i;
    u32 fv;

    en_nichi_cursor_move();
    ctrl = pad_data;
    v0 = en_nichi_action_timer + 1;
    if (v0 >= 25) {
        cnt = en_nichi_poi_wear + 1;
        en_nichi_poi_wear = cnt;
        if (cnt >= 80) {
            en_nichi_action_timer = 0;
            en_nichi_action_step = 3;
            v0 = 0;
            return v0;
        }
        v0 = 0;
    }

    en_nichi_action_timer = v0;
    if (ctrl & 0x20) {
        return;
    }
    en_nichi_action_timer = v0;
    if (v0 < 4) {
        return;
    }

    en_nichi_marker_add(cursor_cw_x, cursor_cw_y);
    en_nichi_cmd_send_once(1282);
    en_nichi_action_step = 0;
    class_w[152].prio = 3;
    en_nichi_action_timer = 0;
    en_nichi_near_set();

    a0v = en_nichi_rare_cnt;
    a1v = en_nichi_get_cnt;
    a2 = en_nichi_poi_wear;
    for (i = 0; i < 10; i++) {
        fv = en_nichi_near_tbl[i];
        if (fv == 2U) {
            v1 = *(u32 *)&en_nichi_obj_wk[i * 48];
            en_nichi_action_step = 2;
            if (v1 == 4) {
                a2 += 80;
                a0v += 1;
            } else {
                a2 += 16;
                a1v += 1;
            }
            *(u32 *)&en_nichi_obj_wk[i * 48] = v1 | 0x80;
        } else if (fv == 1U) {
            v1 = *(u32 *)&en_nichi_obj_wk[i * 48];
            en_nichi_action_step = 2;
            if (v1 == 4) {
                a0v += 1;
            } else {
                a1v += 1;
            }
            *(u32 *)&en_nichi_obj_wk[i * 48] = v1 | 0x80;
        }
    }
    en_nichi_poi_wear = a2;
    en_nichi_get_cnt = a1v;
    en_nichi_rare_cnt = a0v;
}
