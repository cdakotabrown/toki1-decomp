/* Function: holiday_girl_exit */
/* Address: 0x80077994 */
/* Size: 728 bytes (182 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 24 --new 40 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: custom_script custom_80077994.py */
#include "common.h"

extern s16 cal_cw_anim;
extern u8  schedule_cursor;
extern u8  screen_w[];
extern u8  now_youbi;
extern u16 basyo_pack;
extern u16 D_800EE854b;
extern u8  girl_no;
extern u16 pass_days;

extern void device_cmd_send(s32 arg0);
extern void class_clear_n(s32 arg0);
extern void func_80074890(u8 arg0, s32 arg1, u8 arg2);
extern void parameter_show_init(void);
extern s32  basyo_girl_get(s32 arg0);
extern void goto_step(s32 arg0);
extern void return_schedule(s32 arg0);

void func_80077994(s32 input)
{
    s32 t6;
    u32 month;
    u8 *ptr;
    s32 result;
    s32 flags;
    s32 month_field;
    u8 *elem;

    if (input == 32) {
        t6 = cal_cw_anim & 1;
        switch (t6) {
        case 0: device_cmd_send(1281); break;
        case 1: device_cmd_send(1282); break;
        }
        ptr = screen_w + (s32)(schedule_cursor * 4U);
        func_80074890(ptr[1693], 1, ptr[1694]);
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

    result = basyo_girl_get((s32)month);
    girl_no = (u8)result;
    if (result == -1) {
        goto call_72c1c;
    }

    if (pass_days >= 730U) {
        goto call_72c1c;
    }

    month = (u32)D_800EE854b >> 12;

    if (month == 10) {
        if (result == 0) {
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
        }
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
    return;
}
