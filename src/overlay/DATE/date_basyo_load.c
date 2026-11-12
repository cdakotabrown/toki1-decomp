/* @PostProc: patch_insn_reg_field 0x280:rt:t6:v1 0x284:rs:t6:v1 0x284:rt:t6:v1 0x28c:rd:t8:t6 0x28c:rt:t6:v1 0x290:rt:t8:t6 */
/* Function: date_basyo_load */
/* Overlay: DATE */
/* Address: 0x8013EB18 */
/* Size: 688 bytes (172 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);

typedef struct {
    s32 e[21];
} TableW;

typedef struct {
    u16 e[21];
} TableH;

extern TableW date_basyo_load_tbl0;
extern TableW date_basyo_load_tbl1;
extern TableW date_basyo_load_tbl2;
extern TableH date_basyo_load_tbl3;
extern volatile u8 now_basyo;
extern u8 cdq_footer_chk;

void date_basyo_load(void) {
    s32 idx;
    TableW tblC;
    TableW tblB;
    TableW tblA;
    s32 arg1;
    s32 arg2;
    TableH tblH;

    tblC = date_basyo_load_tbl0;
    tblB = date_basyo_load_tbl1;
    tblA = date_basyo_load_tbl2;
    arg1 = 0xBFA6;
    arg2 = 0xBF87;
    tblH = date_basyo_load_tbl3;

    device_cmd_send(207);
    idx = now_basyo;
    cdq_footer_chk = 0;

    if (idx == 2) {
        arg1 = 0xC525;
        arg2 = 0xC522;
    } else if (idx == 17) {
        arg1 = 0xC58C;
        arg2 = 0xC57D;
    } else if (idx == 1 || idx == 3) {
        arg1 = 0xC1D1;
        arg2 = 0xC1BB;
    } else if (idx == 10 || idx == 9) {
        arg1 = 0xC1E0;
        arg2 = 0xC1D3;
    } else if (idx == 12) {
        arg1 = 0xC665;
        arg2 = 0xC631;
    } else if (idx == 17 || idx == 16) {
        arg1 = 0xC58C;
        arg2 = 0xC57D;
    } else if (idx == 20 || idx == 19) {
        arg1 = 0xC61F;
        arg2 = 0xC612;
    } else if (idx == 8) {
        arg1 = 0xC0CE;
        arg2 = 0xC0C4;
    } else if (idx == 7) {
        arg1 = 0xC4A1;
        arg2 = 0xC488;
    } else if (idx == 18) {
        arg1 = 0xC4D3;
        arg2 = 0xC4A3;
    } else if (idx == 14) {
        arg1 = 0xC5D7;
        arg2 = 0xC5C7;
    }

    device_load_start(1, arg1, arg2, tblB.e[idx], tblA.e[idx], tblC.e[idx]);

    if (device_load_poll() == 1) {
        device_cmd_send(tblH.e[now_basyo]);
        sub_sub_step_up();
    }
    sub_sub_step_up();
}
