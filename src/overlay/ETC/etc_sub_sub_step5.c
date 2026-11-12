/* @SymbolStart: 0x8013C8DC */
/* @SymbolSize: 904 */
/* @PostProc: swap_instructions --offset1 0x300 --offset2 0x304 */
/* @PostProc: patch_insn_reg_field 0x90:rt:a1:t8 0x9c:rs:a1:t8 0x9c:rt:a1:t8 0xbc:rt:a1:t8 0x108:rt:a1:t1 0x10c:rd:t8:t9 0x110:rs:t8:t9 0x110:rt:t9:t0 0x114:rs:a1:t1 0x114:rt:a1:t1 0x118:rt:t9:t0 0x120:rs:t9:t0 0x134:rt:a1:t1 0x178:rt:a1:t4 0x17c:rd:t0:t2 0x180:rs:t0:t2 0x180:rt:t1:t3 0x184:rs:a1:t4 0x184:rt:a1:t4 0x188:rt:t1:t3 0x190:rs:t1:t3 0x1a4:rt:a1:t4 0x1e8:rt:a1:t7 0x1ec:rd:t2:t5 0x1f0:rs:t2:t5 0x1f0:rt:t3:t6 0x1f4:rs:a1:t7 0x1f4:rt:a1:t7 0x1f8:rt:t3:t6 0x200:rs:t3:t6 0x214:rt:a1:t7 0x25c:rt:a1:t0 0x260:rd:t4:t8 0x264:rs:t4:t8 0x264:rt:t5:t9 0x268:rs:a1:t0 0x268:rt:a1:t0 0x26c:rt:t5:t9 0x274:rs:t5:t9 0x288:rt:a1:t0 0x2b8:rt:a1:t3 0x2bc:rd:t6:t1 0x2c0:rs:t6:t1 0x2c0:rt:t7:t2 0x2c4:rs:a1:t3 0x2c4:rt:a1:t3 0x2c8:rt:t7:t2 0x2d0:rs:t7:t2 0x2e4:rt:a1:t3 0x2ec:rt:v0:t4 0x2f0:rs:v0:t4 0x2f0:rt:t8:t6 0x2f4:rt:t0:t5 0x2f8:rs:t0:t5 0x2f8:rt:t0:t5 0x300:rd:t9:t7 0x300:rt:t8:t6 0x304:rt:t8:t6 0x308:rd:v1:v0 0x308:rs:t9:t5 0x308:rt:t0:t7 0x314:rs:v1:v0 0x318:rs:v1:v0 0x32c:rt:t1:t8 0x330:rs:t1:t8 0x330:rt:t1:t8 0x338:rs:t1:t8 0x338:rt:t2:t9 0x340:rt:t2:t9 0x34c:rt:t3:t0 0x350:rs:t3:t0 0x354:rt:t4:t1 0x358:rs:t4:t1 0x358:rt:t4:t1 0x360:rs:t4:t1 0x360:rt:t5:t2 0x368:rt:t5:t2 0x374:rt:t6:t3 0x378:rs:t6:t3 */
/* etc_sub_sub_step5 - ETC overlay, Address: 0x8013C8DC (head) / 0x8013C8E4, 904 bytes (226 insns). */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=0 IDO52_FORCE_WEB_COLOR=0:a0,10:v0 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);
extern s32 get_CD_State(void);
extern void set_XAsector(s32, s32, s32);
extern s32 rand(void);

extern volatile u8 now_girl;
extern volatile u8 step_mode;
extern volatile s32 etc_xa_last;
extern volatile s32 etc_xa_sel;
extern u8 etc_xa_cnt[];
extern s32 *etc_xa_tbl;

void etc_sub_sub_step5(void)
{
    s32 a0 = now_girl;
    s32 v1;
    s32 old;
    s32 idx;

    if (a0 == 12 || a0 == 11) {
        sub_sub_step_up();
        return;
    }

    switch (step_mode) {
    case 0:
        if (a0 == 0) {
            for (;;) {
                v1 = (u32)rand();
                idx = now_girl;
                old = etc_xa_last;
                v1 = (u32)v1 % etc_xa_cnt[idx];
                etc_xa_sel = v1;
                if (v1 == old) {
                    continue;
                }
                etc_xa_sel = v1;
                if (v1 == 20) {
                    continue;
                }
                etc_xa_sel = v1;
                if (v1 != 0) {
                    break;
                }
            }
        } else if (a0 == 2) {
            for (;;) {
                v1 = (u32)rand();
                idx = now_girl;
                old = etc_xa_last;
                v1 = (u32)v1 % etc_xa_cnt[idx];
                etc_xa_sel = v1;
                if (v1 == old) {
                    continue;
                }
                etc_xa_sel = v1;
                if (v1 != 8) {
                    break;
                }
            }
        } else if (a0 == 3) {
            for (;;) {
                v1 = (u32)rand();
                idx = now_girl;
                old = etc_xa_last;
                v1 = (u32)v1 % etc_xa_cnt[idx];
                etc_xa_sel = v1;
                if (v1 == old) {
                    continue;
                }
                etc_xa_sel = v1;
                if (v1 != 9) {
                    break;
                }
            }
        } else if (a0 == 4) {
            for (;;) {
                v1 = (u32)rand();
                idx = now_girl;
                old = etc_xa_last;
                v1 = (u32)v1 % etc_xa_cnt[idx];
                etc_xa_sel = v1;
                if (v1 == old) {
                    continue;
                }
                etc_xa_sel = v1;
                if (v1 != 13) {
                    break;
                }
            }
        } else if (a0 == 9) {
            for (;;) {
                v1 = (u32)rand();
                idx = now_girl;
                old = etc_xa_last;
                v1 = (u32)v1 % etc_xa_cnt[idx];
                etc_xa_sel = v1;
                if (v1 == old) {
                    continue;
                }
                etc_xa_sel = v1;
                if (v1 != 0) {
                    break;
                }
            }
        } else {
            do {
                v1 = (u32)rand();
                idx = now_girl;
                old = etc_xa_last;
                v1 = (u32)v1 % etc_xa_cnt[idx];
                etc_xa_sel = v1;
            } while (v1 == old);
        }

        {
            s32 sel = etc_xa_sel;
            s32 *entry = etc_xa_tbl + sel * 2;
            etc_xa_last = sel;
            set_XAsector(entry[0], entry[1], now_girl);
        }
        device_cmd_send(768);
        step_mode = step_mode + 1;
        return;

    case 1:
        v1 = get_CD_State();
        if (v1 & 4) {
            step_mode = step_mode + 1;
        }
        return;

    case 2:
        v1 = get_CD_State();
        if (v1 & 2) {
            sub_sub_step_up();
        }
        return;

    default:
        return;
    }
}
