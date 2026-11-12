/* Function: func_8005B6D4 */
/* Address: 0x8005B6D4 */
/* Size: 844 bytes (211 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_func_8005B6D4.py */
#include "common.h"

extern s32 func_80065214(s32 a0);
extern void cd_command_issue(s32 a0, s32 a1, s32 a2);
extern void device_cmd_send(s32 a0);
extern s32 device_init_flag_get(void);
extern void device_port_init(s32 a0, s32 a1);
extern s32 basyo_girl_get(s32 a0);
extern void goto_step(s32 a0);
extern void goto_sub_step(s32 a0);
extern void cursor_work_copy(s32 a0);
extern void sndisp(u8 *a0, s32 a1, s32 a2);

extern s32 pad_data;
extern s32 pad_trig;
extern s8 now_icon;
extern u8 sched_icon;
extern u16 basyo_pack;
extern u8 card_mode;
extern u8 now_day;
extern u8 cal_cell_tbl_1[];
extern u8 icon_select_check_str0;
extern u8 icon_select_check_str1;

typedef struct { u32 data[14]; } CharData56;
extern CharData56 D_800FE760[];

s32 func_8005B6D4(s32 statusVal) {
    s32 flag;
    s32 menuIdx;
    s32 result;
    u16 val16;
    s32 type;
    u8 oldVal;
    u8 subByte;
    u8 lookup;
    CharData56 *elem;
    s32 flags456;

    if (statusVal == 2) {
        if (pad_data & 1) {
            return 1;
        }
    }

    flag = 0;

    if (!(pad_trig & 0x20)) {
        goto done;
    }

    menuIdx = now_icon;

    if (menuIdx > 0) {
        flag = 1;
        cd_command_issue(9, 0, 0);
        device_cmd_send(3);
        result = device_init_flag_get();
        if (result == 0) {
            device_port_init(0, 0);
        }
    }

    menuIdx = now_icon;

    switch (menuIdx) {
    case 4:
        result = func_80065214(menuIdx);
        if (result == 0) {
            goto done;
        }
        device_cmd_send(1280);
        val16 = basyo_pack;
        type = (u32)val16 >> 12;
        if (type == 11) {
            oldVal = sched_icon;
            sched_icon = (now_icon & 0xF) | (oldVal & 0xFFF0);
            goto_sub_step(5);
            goto done;
        }
        result = basyo_girl_get(type);
        elem = &D_800FE760[result];
        flags456 = *(s32*)((u8*)elem + 456);
        if ((flags456 << 29) < 0) {
            (*(s16*)((u8*)elem + 450))++;
        }
        goto_step(48);
        oldVal = sched_icon;
        sched_icon = (now_icon & 0xF) | (oldVal & 0xFFF0);
        goto done;

    case 0: case 1: case 2: case 3:
    case 5: case 6: case 7: case 8: case 9: case 10:
        result = func_80065214(menuIdx);
        if (result == 0) {
            goto done;
        }
        menuIdx = now_icon;
        if (menuIdx != 8 && menuIdx != 10) {
            device_cmd_send(1280);
        } else {
            device_cmd_send(1293);
        }
        goto_step(48);
        oldVal = sched_icon;
        sched_icon = (now_icon & 0xF) | (oldVal & 0xFFF0);
        goto done;

    case 12:
        device_cmd_send(1293);
        goto_sub_step(2);
        goto done;

    case 13:
        result = func_80065214(menuIdx);
        if (result == 0) {
            goto done;
        }
        subByte = card_mode;
        if (subByte != 0) {
            goto done;
        }
        cursor_work_copy(0);
        device_cmd_send(1293);
        goto_sub_step(3);
        goto done;

    case 11:
        device_cmd_send(1293);
        goto_sub_step(4);
        goto done;

    case 14:
        result = func_80065214(menuIdx);
        if (result == 0) {
            goto done;
        }
        oldVal = sched_icon;
        sched_icon = (oldVal & 0xFFF0) | 0xE;
        subByte = now_day;
        lookup = cal_cell_tbl_1[subByte];
        if (lookup == 5) {
            device_cmd_send(1280);
            goto_step(48);
            sndisp(&icon_select_check_str0, 0, 31);
            goto done;
        }
        if (lookup == 11) {
            device_cmd_send(1280);
            goto_step(48);
            sndisp(&icon_select_check_str1, 0, 31);
            goto done;
        }
        goto_step(48);
        oldVal = sched_icon;
        sched_icon = (oldVal & 0xFFF0) | 0x7;
        goto done;
    }

done:
    return flag;
}
