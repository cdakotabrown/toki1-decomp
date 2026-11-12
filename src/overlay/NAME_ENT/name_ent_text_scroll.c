/* @CompileEnv: IDO52_AS1_NO_SCHED_LOAD_HOIST=strict:7,8 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x178 --offset2 0x17c */
/* @PostProc: swap_instructions --offset1 0x17c --offset2 0x180 */
/* @PostProc: swap_instructions --offset1 0x180 --offset2 0x184 */
/* @PostProc: swap_instructions --offset1 0x188 --offset2 0x18c */
/* @PostProc: patch_insn_reg_field 0x20:rt:v1:a0 0x24:rs:v1:a0 0x24:rt:v1:a0 0x28:rt:a2:v1 0x2c:rs:v1:a0 0x38:rs:a2:v1 0x38:rt:a2:v1 0x40:rs:a2:v1 0x4c:rs:v1:a0 0x60:rt:v1:a1 0x64:rs:v1:a1 0x64:rt:v1:a1 0x6c:rs:v1:a1 0x6c:rt:v1:a0 0x70:rs:v0:a0 0x70:rt:v1:v0 0x80:rt:v1:a0 0xa0:rs:a2:v1 0xd4:rt:t4:t3 0xe0:rt:t1:t0 0xe4:rt:t1:t0 0xe8:rt:t2:t1 0xf0:rs:t2:t1 0xf0:rt:t3:t2 0xf4:rt:t3:t2 0xf8:rs:t4:t3 0xf8:rt:t4:t3 0x100:rs:t4:t3 0x100:rt:t5:t4 0x108:rt:t5:t4 0x118:rs:a2:v1 0x134:rt:t2:t0 0x138:rt:t6:t5 0x13c:rt:t3:t1 0x140:rs:t6:t5 0x140:rt:t7:t6 0x144:rt:t7:t6 0x148:rt:t8:t7 0x14c:rt:t4:t2 0x150:rs:t8:t7 0x150:rt:t9:t8 0x154:rt:t9:t8 0x158:rt:t1:t9 0x15c:rt:t6:t4 0x160:rs:t1:t9 0x16c:rt:t2:t0 0x170:rt:t3:t1 0x174:rs:t4:t2 0x174:rt:t4:t2 0x17c:rs:t4:t2 0x17c:rt:t5:t3 0x180:rt:t5:t3 0x184:rs:t6:t4 0x184:rt:t6:t4 0x18c:rs:t6:t4 0x18c:rt:t7:t5 0x190:rs:t7:t5 0x194:rt:t7:t5 0x1c0:rt:t8:t6 0x1c4:rs:t8:t6 0x1c8:rt:t9:t7 0x1d0:rt:t9:t7 0x1dc:rt:t1:t8 0x1e0:rs:t1:t8 0x1e0:rt:t1:t8 0x1e8:rs:t1:t8 0x1e8:rt:t2:t9 0x1ec:rs:t2:t9 0x1f0:rt:t3:t0 0x1f4:rs:t3:t0 0x1f4:rt:t3:t0 0x1fc:rs:t3:t0 0x20c:rt:a2:a1 0x210:rt:t4:t1 0x214:rs:t4:t1 0x21c:rt:a1:t2 0x220:rs:a1:t2 0x220:rt:a3:t2 0x224:rt:t6:t4 0x228:rd:t5:t3 0x228:rt:a3:t2 0x22c:rs:t6:t4 0x22c:rt:t6:t4 0x230:rt:t7:t5 0x234:rd:v1:t5 0x234:rt:t6:t3 0x238:rd:t7:v1 0x238:rs:t7:t3 0x238:rt:t5:t4 0x240:rs:t7:t5 0x240:rt:t7:t5 0x244:rt:t9:t7 0x248:rs:a0:t5 0x248:rt:t7:a0 0x250:rs:a2:a1 0x250:rt:t8:t6 0x258:rt:t8:t6 0x25c:rt:a3:t2 0x280:rs:t9:t7 0x284:rt:t1:t8 0x288:rs:t1:t8 0x288:rt:t1:t8 0x290:rs:t1:t8 0x290:rt:t2:t9 0x294:rt:t2:t9 0x2ac:rt:t3:t0 0x2b4:rs:t3:t0 0x2b8:rt:t4:t1 0x2bc:rs:t4:t1 0x2bc:rt:t4:t1 0x2c4:rs:t4:t1 0x2c4:rt:t5:t3 0x2c8:rt:t5:t3 0x2e0:rt:t6:t4 0x2e4:rs:t6:t4 0x2e4:rt:t6:t4 0x2e8:rt:t7:t5 0x2ec:rs:t6:t4 0x2fc:rs:t7:t5 0x2fc:rt:t7:t5 0x304:rs:t7:t5 0x304:rt:t8:t6 0x308:rt:t8:t6 */
/* 3. `@CompileEnv: IDO52_AS1_NO_SCHED_LOAD_HOIST=strict:7,8` (Hook 7). */
#include "types.h"

extern u8 now_girl;
extern u8 k_cnt;
extern u8 k_no;
extern u8 k_mode;
extern s32 step_counter;
extern u8 step_mode;

extern u8 name_ent_serifu_no;
extern s32 name_ent_voice_on;
extern u8 name_ent_serifu_girl_tbl[];
extern s32 name_ent_serifu_xa_tbl[];
extern s32 name_ent_serifu_xa_tbl2[];

extern s32 get_now_k(void);
extern s32 k_disp_inc(void);
extern void k_disp_switch(s32, s32);
extern u8 *get_k_work(s32);
extern s32 get_CD_State(void);
extern s32 *get_XAsector(void);
extern void set_XAsector(s32, s32, s32);
extern void device_cmd_send(s32);

void name_ent_text_scroll(void)
{
    s32 v;
    s32 mode;
    s32 sel;
    u8 *p;
    u8 *q;
    s32 *pad;
    u32 idx;
    s32 a;
    s32 *e;

    sel = 0;
    v = get_now_k();

    if ((k_mode & 0x80) != 0) {
        mode = k_mode & 0x7F;
        if ((u32)(k_no + 2) >= (u32)v) {
            sel = k_disp_inc();
            v = get_now_k();
            if (k_no + 3 == v &&
                (u32)k_cnt >= (u32)(k_no + 3)) {
                k_disp_switch(v, 0);
            }
        } else {
            if (mode == 0) {
                k_disp_switch(k_no, 0);
                p = get_k_work(k_no + 1);
                q = get_k_work(k_no + 2);
                *(s16 *)(p + 2) -= 2;
                *(s16 *)(q + 2) -= 2;
                k_mode = k_mode + 1;
            } else if (mode != 0) {
                p = get_k_work(k_no + 1);
                q = get_k_work(k_no + 2);
                *(s16 *)(p + 2) -= 2;
                *(s16 *)(q + 2) -= 2;
                if (*(s16 *)(p + 2) < 51) {
                    *(s16 *)(p + 2) = 50;
                    *(s16 *)(q + 2) = 66;
                    k_mode = k_mode - 1;
                    k_no = k_no + 1;
                    k_disp_switch(k_no + 2, 1);
                }
            }
        }
    } else {
        sel = k_disp_inc();
        get_now_k();
    }

    if ((get_CD_State() & 4) != 0) {
        name_ent_voice_on = 1;
    }

    pad = get_XAsector();

    if ((now_girl & 0xF) != 15) {
        if (name_ent_voice_on != 0) {
            if ((get_CD_State() & 2) != 0) {
                idx = name_ent_serifu_no;
                e = &name_ent_serifu_xa_tbl2[idx * 2];
                a = e[0];
                if (name_ent_serifu_xa_tbl[idx * 2] != a && a != pad[2]) {
                    set_XAsector(a, e[1], name_ent_serifu_girl_tbl[idx]);
                    device_cmd_send(768);
                } else if (sel == 1) {
                    step_counter = step_counter + 1;
                    k_mode = 0;
                }
            } else {
                step_counter = 0;
            }
        }
    } else if (sel == 1) {
        step_counter = step_counter + 1;
        k_mode = 0;
    } else {
        step_counter = 0;
    }

    if ((u32)step_counter >= 61) {
        step_counter = 0;
        step_mode = step_mode + 1;
    }
}
