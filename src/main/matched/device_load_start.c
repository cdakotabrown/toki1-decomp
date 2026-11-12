/* Function: device_load_start */
/* Address: 0x80044ADC */
/* Size: 1004 bytes */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* `@CompileFlags: -Wc,-pic1` reserves the PIC linkage area natively, so */
/* @CompileFlags: -Wc,-pic1 */
/* @SymbolStart: 0x80044AC0 */
/* @SymbolSize: 1032 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 device_load_poll_tbl1;
extern u8 device_load_poll_tbl0;
extern u8 device_load_start_tbl0;
extern u8 device_load_start_tbl6;
extern u8 device_load_start_tbl1;
extern u8 device_load_poll_tbl2;
extern u8 device_load_start_tbl7;
extern s32 device_load_start_tbl2;
extern s32 device_load_start_tbl3;
extern s32 device_load_start_tbl4;
extern s32 device_load_start_tbl5;

extern u8 get_reads(void);
extern void change_trans_pointer(s32);
extern void cd_read_start(s32, s32, s32);

void device_load_start(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    device_load_start_tbl6 = 255;
    device_load_start_tbl0 = 0;
    device_load_start_tbl1 = 0;
    device_load_poll_tbl1 = (u8)arg0;
    device_load_poll_tbl2 = 0;
    device_load_start_tbl7 = 1;
    device_load_poll_tbl0 = get_reads();

    if (arg0 == 0) {
        change_trans_pointer(0x801B8000);
        if (device_load_start_tbl2 == arg1) {
            device_load_poll_tbl0 = device_load_poll_tbl0 - 2;
            device_load_start_tbl0 = 1;
        } else {
            if (arg1 != 0) {
                cd_read_start(3, 0x80010000, arg1);
                device_load_start_tbl2 = arg1;
            } else {
                device_load_poll_tbl0 = device_load_poll_tbl0 - 1;
                device_load_start_tbl0 = 1;
            }
            if (arg2 != 0) {
                cd_read_start(32, 0x801B8000, arg2);
            } else {
                device_load_poll_tbl0 = device_load_poll_tbl0 - 1;
                device_load_start_tbl0 = 1;
            }
        }
        if (device_load_start_tbl3 == arg3) {
            device_load_poll_tbl0 = device_load_poll_tbl0 - 3;
            device_load_start_tbl1 = 1;
            return;
        }
        if (arg5 != 0) {
            cd_read_start(37, 0x8001C000, arg5);
            device_load_start_tbl3 = arg3;
        } else {
            device_load_poll_tbl0 = device_load_poll_tbl0 - 1;
            device_load_start_tbl1 = 1;
        }
        if (arg4 != 0) {
            cd_read_start(80, 0x801C8000, arg4);
        } else {
            device_load_poll_tbl0 = device_load_poll_tbl0 - 1;
            device_load_start_tbl1 = 1;
        }
        if (arg3 != 0) {
            cd_read_start(6, 0x80012000, arg3);
            device_load_start_tbl3 = arg3;
            return;
        }
        device_load_poll_tbl0 = device_load_poll_tbl0 - 1;
        device_load_start_tbl1 = 1;
        return;
    }

    change_trans_pointer(0x801B8000);
    if (device_load_start_tbl4 == arg1) {
        device_load_poll_tbl0 = device_load_poll_tbl0 - 2;
        device_load_start_tbl0 = 1;
    } else {
        if (arg1 != 0) {
            cd_read_start(3, 0x8001A000, arg1);
            device_load_start_tbl4 = arg1;
        } else {
            device_load_poll_tbl0 = device_load_poll_tbl0 - 1;
            device_load_start_tbl0 = 1;
        }
        if (arg2 != 0) {
            cd_read_start(53, 0x801E0000, arg2);
        } else {
            device_load_poll_tbl0 = device_load_poll_tbl0 - 1;
            device_load_start_tbl0 = 1;
        }
    }
    if (device_load_start_tbl5 == arg3) {
        device_load_poll_tbl0 = device_load_poll_tbl0 - 3;
        device_load_start_tbl1 = 1;
        return;
    }
    if (arg5 != 0) {
        cd_read_start(37, 0x8002E800, arg5);
        device_load_start_tbl5 = arg3;
    } else {
        device_load_poll_tbl0 = device_load_poll_tbl0 - 1;
        device_load_start_tbl1 = 1;
    }
    if (arg4 != 0) {
        cd_read_start(80, 0x801B8000, arg4);
    } else {
        device_load_poll_tbl0 = device_load_poll_tbl0 - 1;
        device_load_start_tbl1 = 1;
    }
    if (arg3 != 0) {
        cd_read_start(6, 0x80016000, arg3);
        device_load_start_tbl5 = arg3;
        return;
    }
    device_load_poll_tbl0 = device_load_poll_tbl0 - 1;
    device_load_start_tbl1 = 1;
}
