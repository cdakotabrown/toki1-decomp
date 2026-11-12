/* Function: holiday_girl_exit */
/* Address: 0x8007798C */
/* Size: 736 bytes (184 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* 1. `@CompileEnv: IDO52_NO_CSE=1` had to GO.  It was fitted while Hook 1 */
/* @SymbolStart: 0x8007798C */
/* @SymbolSize: 736 */
/* @CompileEnv: IDO52_NO_CSE_OPCODES=mpy IDO52_AS1_XBB_SKIP_CAND=10 IDO52_CONST_DEF_BB=212:41:+ */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0x18:rt:v0:v1 0x1c:rs:v0:v1 0x1c:rt:v0:v1 0x24:rs:v0:v1 0x178:rd:t4:t5 0x17c:rs:t4:t5 0x180:rd:t5:t7 0x184:rs:t5:t7 0x188:rd:t6:t8 0x18c:rt:t6:t8 0x18c:rd:t7:t9 0x190:rs:t7:t9 0x190:rt:a0:t1 0x194:rd:t9:t5 0x198:rt:a0:t1 0x198:rd:t8:t3 0x19c:rs:t8:t3 0x1a4:rs:t9:t5 0x1b4:rd:t1:t6 0x1b8:rt:t1:t6 0x1bc:rt:t2:t8 0x1c4:rs:t2:t8 0x1c4:rt:t3:t9 0x1cc:rt:t3:t9 0x1e0:rd:t4:t1 0x1e4:rt:t4:t1 0x1e4:rd:t5:t2 0x1e8:rs:t5:t2 0x1f0:rd:t6:t4 0x1f4:rs:t6:t4 0x1f8:rd:t7:t6 0x1fc:rs:t7:t6 0x200:rd:t8:t7 0x204:rt:t8:t7 0x204:rd:t9:t8 0x208:rs:t9:t8 0x208:rt:a0:t9 0x20c:rd:t2:t4 0x210:rt:a0:t9 0x210:rd:t1:t2 0x214:rs:t1:t2 0x21c:rs:t2:t4 0x22c:rd:t3:t5 0x230:rt:t3:t5 0x234:rt:t4:t7 0x23c:rs:t4:t7 0x23c:rt:t5:t8 0x244:rt:t5:t8 0x258:rd:t6:t9 0x25c:rt:t6:t9 0x25c:rd:t7:t1 0x260:rs:t7:t1 0x268:rd:t8:t3 0x26c:rs:t8:t3 0x270:rd:t9:t5 0x274:rs:t9:t5 0x278:rd:t1:t6 0x27c:rt:t1:t6 0x27c:rd:t2:t7 0x280:rs:t2:t7 0x280:rt:a0:t8 0x284:rd:t4:t3 0x288:rt:a0:t8 0x288:rd:t3:t1 0x28c:rs:t3:t1 0x294:rs:t4:t3 0x2a4:rd:t5:t4 0x2a8:rt:t5:t4 */
#include "types.h"

extern s32 step_counter;
extern s16 cal_cw_anim;
extern u8  schedule_cursor;
extern u8  screen_w[];
extern u8  now_youbi;
extern u16 basyo_pack;
extern u8  girl_no;
extern u16 pass_days;

extern void device_cmd_send(s32 arg0);
extern void class_clear_n(s32 arg0);
extern void parameter_up_down(u8 arg0, s32 arg1, u8 arg2);
extern void parameter_show_init(void);
extern s32  basyo_girl_get(s32 arg0);
extern void goto_step(s32 arg0);
extern void return_schedule(s32 arg0);

void holiday_girl_exit(void)
{
    s32 input;
    s32 t6;
    u8  aa;
    u8  cc;
    u32 month;
    u8 *ptr;
    s32 result;
    s32 flags;
    s32 month_field;
    u8 *elem;

    input = step_counter;

    if (input == 32) {
        t6 = cal_cw_anim & 1;
        switch (t6) {
        case 0: device_cmd_send(1281); break;
        case 1: device_cmd_send(1282); break;
        }
        ptr = screen_w + (s32)(schedule_cursor * 4U);
        cc = ptr[1694];
        aa = ptr[1693];
        parameter_up_down(aa, 1, cc);
        parameter_show_init();
        return;
    }

    if ((u32)input < 65U) {
        return;
    }

    class_clear_n(96);
    class_clear_n(97);

    if (now_youbi != 6) {
        goto call_72c1c;
    }

    ptr = screen_w + (s32)(schedule_cursor * 4U);
    if (ptr[1693] != 4) {
        goto call_72c1c;
    }

    month = (u32)basyo_pack >> 12;
    if (month == 11) {
        goto call_72c1c;
    }

    if (*(u16 *)(screen_w + month * 4 + 1644) != 6) {
        goto call_72c1c;
    }

    result = basyo_girl_get((s32)((u32)basyo_pack >> 12));
    girl_no = (u8)result;
    if (result == -1) {
        goto call_72c1c;
    }

    if (pass_days >= 730U) {
        goto call_72c1c;
    }

    month = (u32)basyo_pack >> 12;

    if (month == 10) {
        if (result == 0) {
            flags = *(s32 *)(screen_w + (u32)(result * 56) + 456);
            if ((flags << 0x1d) >= 0) {
                goto second;
            }
            if ((flags << 0x1e) >= 0) {
                goto second;
            }
            month_field = *(s32 *)(screen_w + month * 4 + 1644);
            if ((month_field << 2) < 0) {
                goto second;
            }
            if ((flags << 0xd) < 0) {
                goto second;
            }
            elem = screen_w + (u32)(result * 56);
            elem[458] = elem[458] | 4;
            goto_step(96);
            return;
        }
second:
        flags = *(s32 *)(screen_w + (u32)(result * 56) + 456);
        if ((flags << 0x1d) >= 0) {
            goto call_72c1c;
        }
        if ((flags << 0x1e) >= 0) {
            goto call_72c1c;
        }
        month_field = *(s32 *)(screen_w + month * 4 + 1644);
        if ((month_field << 2) < 0) {
            goto call_72c1c;
        }
        if ((flags << 0xd) < 0) {
            goto call_72c1c;
        }
        elem = screen_w + (u32)(result * 56);
        elem[458] = elem[458] | 4;
        goto_step(96);
        return;
    } else {
        flags = *(s32 *)(screen_w + (u32)(result * 56) + 456);
        if ((flags << 0x1e) >= 0) {
            goto call_72c1c;
        }
        if ((flags << 0x1d) >= 0) {
            goto call_72c1c;
        }
        month_field = *(s32 *)(screen_w + month * 4 + 1644);
        if ((month_field << 2) < 0) {
            goto call_72c1c;
        }
        if ((flags << 0xd) < 0) {
            goto call_72c1c;
        }
        elem = screen_w + (u32)(result * 56);
        elem[458] = elem[458] | 4;
        goto_step(96);
        return;
    }

call_72c1c:
    return_schedule(0);
}
