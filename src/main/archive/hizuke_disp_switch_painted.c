/* Function: hizuke_disp_switch (body, after thunk at holiday_icon_check) */
/* Address: 0x800760BC */
/* Size: 1144 bytes (286 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_func_800760BC.py */
/* @PostProc: scheduler_normalizer */
/* @PostProc: custom_script remap_func_800760BC.py */
#include "types.h"

extern u8 icon_mode;
extern s32 pad_data;
extern s32 pad_trig;
extern s8 now_icon;
extern u8 sched_icon;
extern u8 now_youbi;
extern u16 basyo_pack;
extern u8 schedule_cursor;
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;

typedef struct { u32 data[14]; } CharData56;
extern CharData56 screen_w[];

extern void cd_command_issue(s32, s32, s32);
extern s32 device_init_flag_get(void);
extern void device_port_init(s32, s32);
extern s32 func_80065214(s32);
extern void device_cmd_send(s32);
extern void goto_sub_step(s32);
extern s32 func_8006BE48(s32);
extern s32 basyo_girl_get(s32);
extern s32 reserved_date_check(u8, u8, u8);

s32 hizuke_disp_switch(void) {
    s32 sp44;
    s32 mode;
    s32 result;
    s32 type;
    CharData56 *elem;
    u8 oldVal;
    u8 idx;

    sp44 = 0;

    if (icon_mode == 2) {
        if (pad_data & 1) {
            return 1;
        }
    }

    if (!(pad_trig & 0x20)) {
        goto done;
    }

    mode = now_icon;
    if (mode > 0) {
        sp44 = 1;
        cd_command_issue(9, 0, 0);
        result = device_init_flag_get();
        if (result == 0) {
            device_port_init(0, 0);
        }
    }

    mode = now_icon;

    switch (mode) {
    case 4:
        result = func_80065214(mode);
        if (result == 0) {
            goto done;
        }
        device_cmd_send(1280);
        if (now_youbi == 6) {
            if (((u32)basyo_pack >> 12) == 11) {
                oldVal = sched_icon;
                sched_icon = (oldVal & 0xFF0F) | (now_icon << 4);
                idx = schedule_cursor;
                *(s8 *)((u8 *)screen_w + idx * 4 + 0x69D) = now_icon;
                result = func_8006BE48(1);
                idx = schedule_cursor;
                *(u8 *)((u8 *)screen_w + idx * 4 + 0x69E) = result;
                goto_sub_step(7);
            } else {
                idx = schedule_cursor;
                *(s8 *)((u8 *)screen_w + idx * 4 + 0x69D) = now_icon;
                result = func_8006BE48(1);
                idx = schedule_cursor;
                *(u8 *)((u8 *)screen_w + idx * 4 + 0x69E) = result;
                oldVal = sched_icon;
                sched_icon = (oldVal & 0xFF0F) | (now_icon << 4);
                type = basyo_girl_get((u32)basyo_pack >> 12);
                elem = &screen_w[type];
                if ((*(s32 *)((u8 *)elem + 456) << 29) < 0) {
                    (*(s16 *)((u8 *)elem + 450))++;
                }
                goto_sub_step(6);
            }
        } else {
            type = basyo_girl_get((u32)basyo_pack >> 12);
            elem = &screen_w[type];
            if ((*(s32 *)((u8 *)elem + 456) << 29) < 0) {
                (*(s16 *)((u8 *)elem + 450))++;
            }
            idx = schedule_cursor;
            *(s8 *)((u8 *)screen_w + idx * 4 + 0x69D) = now_icon;
            result = func_8006BE48(1);
            idx = schedule_cursor;
            *(u8 *)((u8 *)screen_w + idx * 4 + 0x69E) = result;
            oldVal = sched_icon;
            sched_icon = (oldVal & 0xFF0F) | (now_icon << 4);
            goto_sub_step(5);
        }
        goto done;

    case 0: case 1: case 2: case 3:
    case 5: case 6: case 7:
        result = func_80065214(mode);
        if (result == 0) {
            goto done;
        }
        oldVal = sched_icon;
        sched_icon = (oldVal & 0xFF0F) | (now_icon << 4);
        device_cmd_send(1280);
        idx = schedule_cursor;
        *(s8 *)((u8 *)screen_w + idx * 4 + 0x69D) = now_icon;
        result = func_8006BE48(1);
        idx = schedule_cursor;
        *(u8 *)((u8 *)screen_w + idx * 4 + 0x69E) = result;
        goto_sub_step(5);
        goto done;

    case 9:
        result = func_80065214(mode);
        if (result == 0) {
            goto done;
        }
        if (reserved_date_check(now_year, now_month, now_day) <= 0) {
            goto done;
        }
        oldVal = sched_icon;
        sched_icon = (oldVal & 0xFF0F) | (now_icon << 4);
        device_cmd_send(1280);
        goto_sub_step(254);
        goto done;

    case 8:
        result = func_80065214(mode);
        if (result == 0) {
            goto done;
        }
        oldVal = sched_icon;
        sched_icon = (oldVal & 0xFF0F) | (now_icon << 4);
        device_cmd_send(1293);
        goto_sub_step(16);
        goto done;

    case 10:
        result = func_80065214(mode);
        if (result == 0) {
            goto done;
        }
        device_cmd_send(1293);
        goto_sub_step(32);
        goto done;

    case 12:
        device_cmd_send(1293);
        goto_sub_step(48);
        goto done;

    case 11:
        device_cmd_send(1293);
        goto_sub_step(64);
        goto done;
    }

done:
    return sp44;
}
