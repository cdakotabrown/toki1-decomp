/* Function: pre_syogatu1 */
/* Address: 0x800616C8 */
/* Size: 412 bytes (103 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0xcc:rd:t0:t1 0xd0:rs:t0:t1 0x13c:rt:t1:v1 0x140:rs:t1:v1 0x140:rt:t1:v1 0x148:rs:t1:v1 */
#include "types.h"

extern u8 step_mode;
extern u8 screen_w;
extern s32 pre_syogatu1_str0;
extern s32 pre_syogatu1_str1;
extern s32 pre_syogatu1_str2;
extern u8 p_sei;

extern void get_h_yuukou_table(u8 *);
extern void k_sub_reset(void);
extern void strcpy(void *, void *);
extern void strcat(void *, void *);
extern void get_g_name(void *, s32);
extern void gnsx(void *);
extern s32  sndisp(void *, s32, s32);
extern s32  xa_wait(void);
extern s32  goto_step(s32);

s32 pre_syogatu1(void)
{
    s32 r;
    s32 i;
    u8 spBuf[20];
    s8 sp6C[256];
    s8 sp5C[16];
    s32 count;

    r = step_mode;
    switch (r) {
    case 0:
        get_h_yuukou_table(spBuf);
        k_sub_reset();
        strcpy(sp6C, &pre_syogatu1_str0);
        count = 0;
        for (i = 0; i != 11; i++) {
            if (spBuf[i] >= (u32)77) {
                if ((*(s32 *)(&screen_w + i * 56 + 456) << 30) < 0) {
                    if (count != 0) {
                        strcat(sp6C, &pre_syogatu1_str1);
                    }
                    get_g_name(sp5C, i);
                    strcat(sp6C, sp5C);
                    count++;
                }
            }
        }
        strcat(sp6C, &pre_syogatu1_str2);
        gnsx(&p_sei);
        r = sndisp(sp6C, 0, 31);
        step_mode++;
        break;
    case 1:
        r = xa_wait();
        break;
    case 2:
        r = goto_step(83);
        break;
    }
    return r;
}
