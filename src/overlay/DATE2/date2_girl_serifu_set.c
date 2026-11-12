/* @SymbolStart: 0x80136A90 */
/* @SymbolSize: 236 */
/* @PostProc: patch_insn_reg_field 0x3c:rt:t8:t9 0x44:rs:t8:t9 0x58:rt:v1:t0 0x5c:rs:v1:t0 0x64:rs:v1:t0 0x64:rt:v1:t0 0x68:rs:v1:t0 0x6c:rt:t9:t1 0x70:rs:t9:t1 0x70:rt:t9:t1 0x78:rs:t9:t1 0x78:rt:t0:t2 0x7c:rt:t0:t2 0x90:rt:t1:t4 0x98:rs:t1:t4 0xac:rt:v1:t5 0xb0:rs:v1:t5 0xb8:rs:v1:t5 0xb8:rt:v1:t5 0xbc:rs:v1:t5 0xc0:rt:t2:t6 0xc4:rs:t2:t6 0xc4:rt:t2:t6 0xcc:rs:t2:t6 0xd0:rt:t3:t7 0xd4:rs:t3:t7 0xd4:rt:t3:t7 0xdc:rs:t3:t7 0xdc:rt:t4:t8 0xe0:rt:t4:t8 */
/* date2_girl_serifu_set -- DATE2 overlay, Address: 0x80136A98 (true start 0x80136A90: */
#include "types.h"

extern s16 date2_g0_now_s;
extern s16 date2_girl_now_s;
extern u8 girl_no;
extern s32 date2_scroll_add(void);
extern s32 get_g_zyotai_s(s32);
extern s32 rand(void);
extern void sub_sub_step_up(void);

void date2_girl_serifu_set(void)
{
    s32 v0;
    s32 t;
    u8 a;

    date2_g0_now_s = 9;
    date2_girl_now_s = 9;
    date2_scroll_add();

    v0 = get_g_zyotai_s(0);
    if ((v0 & 0x7F) == 4) {
        v0 = rand();
        t = v0 % 2;
        if (t < 0) {
        }
        if (t != 0) {
            date2_g0_now_s = (s16)(date2_g0_now_s + 1);
        }
    }

    a = girl_no;
    v0 = get_g_zyotai_s((s32)a);
    if ((v0 & 0x7F) == 4) {
        v0 = rand();
        t = v0 % 2;
        if (t < 0) {
        }
        if (t != 0) {
            if (girl_no != 6) {
                date2_girl_now_s = (s16)(date2_girl_now_s + 1);
            }
        }
    }
    sub_sub_step_up();
}
