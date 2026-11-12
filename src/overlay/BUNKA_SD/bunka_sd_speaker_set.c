/* @SymbolStart: 0x80132148 */
/* @RodataBase: 0x80139F48 */
/* Address: 0x80132150 */
/* Size: 412 bytes (103 insns) */
/* @SymbolSize: 420 */
#include "types.h"

extern void set_XAsector(s32, s32, s32);
extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);

extern u8  now_girl;
extern u8  bunka_sd_kind2_line_tbl[];
extern u8  bunka_sd_kind2_xa_tbl[];
extern u8  bunka_sd_kind3_line_tbl[];
extern u8  bunka_sd_kind3_xa_tbl[];
extern u8  bunka_sd_kind0_line_tbl[];
extern u8  bunka_sd_kind1_line_tbl[];
extern u8  bunka_sd_kind1_xa_tbl[];
extern u8  bunka_sd_kind4_line_tbl[];
extern u8  bunka_sd_kind4_xa_tbl[];
extern s32 bunka_sd_line_ptr;
extern s32 bunka_sd_xa_ptr;

extern s32 bunka_sd_kind;

void bunka_sd_speaker_set(void) {
    switch (bunka_sd_kind) {
    case 0:
        bunka_sd_line_ptr = (s32)bunka_sd_kind0_line_tbl;
        break;
    case 1:
        bunka_sd_line_ptr = (s32)bunka_sd_kind1_line_tbl;
        bunka_sd_xa_ptr = (s32)bunka_sd_kind1_xa_tbl;
        now_girl = 14;
        set_XAsector(0x1E00004C, 0x10000051, 14);
        device_cmd_send(768);
        break;
    case 2:
        bunka_sd_line_ptr = (s32)bunka_sd_kind2_line_tbl;
        bunka_sd_xa_ptr = (s32)bunka_sd_kind2_xa_tbl;
        now_girl = 0;
        set_XAsector(0x1C0002E9, 0x410002F3, 0);
        device_cmd_send(768);
        break;
    case 3:
        bunka_sd_line_ptr = (s32)bunka_sd_kind3_line_tbl;
        bunka_sd_xa_ptr = (s32)bunka_sd_kind3_xa_tbl;
        now_girl = 1;
        set_XAsector(0x1A0001B5, 0x150001CA, 1);
        device_cmd_send(768);
        break;
    case 4:
    default:
        bunka_sd_line_ptr = (s32)bunka_sd_kind4_line_tbl;
        bunka_sd_xa_ptr = (s32)bunka_sd_kind4_xa_tbl;
        now_girl = 11;
        set_XAsector(0x1F0000AD, 0x100000B3, 11);
        device_cmd_send(768);
        break;
    }
    sub_sub_step_up();
}
