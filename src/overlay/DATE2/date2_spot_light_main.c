/* date2_spot_light_main -- DATE2 overlay, Address: 0x80135E44, Size: 1932 bytes */
/* (forces materialisation + `beq`) plus `@CompileEnv: */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:189 IDO52_COPYPROP_VAR=-8:2,-8:4 IDO52_FORCE_WEB_COLOR=0:s2 */
/* @RodataBase: 0x8013A290 */
/* @SymbolSize: 1932 */
/* @PostProc: swap_instructions --offset1 0x550 --offset2 0x554 */
/* @PostProc: swap_instructions --offset1 0x554 --offset2 0x558 */
/* @PostProc: swap_instructions --offset1 0x558 --offset2 0x55c */
/* @PostProc: swap_instructions --offset1 0x55c --offset2 0x560 */
/* @PostProc: swap_instructions --offset1 0x560 --offset2 0x564 */
/* @PostProc: swap_instructions --offset1 0x564 --offset2 0x568 */
/* @PostProc: swap_instructions --offset1 0x568 --offset2 0x56c */
/* @PostProc: swap_instructions --offset1 0x2c0 --offset2 0x2c4 */
/* @PostProc: swap_instructions --offset1 0x338 --offset2 0x340 */
/* @PostProc: swap_instructions --offset1 0x588 --offset2 0x590 */
/* @PostProc: swap_instructions --offset1 0x58c --offset2 0x590 */
/* @PostProc: swap_instructions --offset1 0x710 --offset2 0x718 */
/* @PostProc: swap_instructions --offset1 0xb4 --offset2 0xbc */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xbc */
/* @PostProc: swap_instructions --offset1 0xbc --offset2 0xc0 */
/* @PostProc: swap_instructions --offset1 0xc0 --offset2 0xc4 */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xd4 */
/* @PostProc: swap_instructions --offset1 0xcc --offset2 0xd8 */
/* @PostProc: swap_instructions --offset1 0xd0 --offset2 0xe0 */
/* @PostProc: patch_insn_reg_field 0xb8:rt:t1:s2 0xc4:rs:t1:s2 0xc4:rt:t1:s2 0xcc:rt:t3:t1 0xd0:rt:t3:t1 0xd8:rs:t1:s2 0xd8:rt:t2:s2 0xe0:rt:t2:s2 0x114:rd:t4:t2 0x118:rs:t4:t2 0x118:rt:t5:t3 0x120:rt:t5:t3 0x150:rd:t6:t4 0x154:rs:t6:t4 0x154:rt:t7:t5 0x15c:rt:t7:t5 0x18c:rd:t8:t6 0x190:rs:t8:t6 0x190:rt:t9:t7 0x198:rt:t9:t7 0x1c8:rd:t0:t8 0x1cc:rs:t0:t8 0x1cc:rt:t1:t9 0x1d4:rt:t1:t9 0x204:rd:t2:t0 0x208:rs:t2:t0 0x208:rt:t3:t1 0x210:rt:t3:t1 0x240:rd:t4:t2 0x244:rs:t4:t2 0x244:rt:t5:t3 0x24c:rt:t5:t3 0x258:rt:t6:t4 0x25c:rs:t6:t4 0x260:rt:t7:s2 0x264:rs:t7:s2 0x264:rt:t7:s2 0x26c:rs:t7:s2 0x26c:rt:t8:s2 0x274:rt:t8:s2 0x278:rt:t9:t5 0x27c:rs:t9:t5 0x27c:rt:t9:t5 0x284:rs:t9:t5 0x284:rt:t0:t6 0x28c:rd:t1:t7 0x28c:rs:t0:t6 0x294:rs:t1:t7 0x29c:rs:t9:t5 0x29c:rt:t2:t8 0x2a4:rd:t3:t9 0x2a4:rs:t2:t8 0x2ac:rs:t3:t9 0x2c0:rt:t4:s2 0x2c8:rs:t4:s2 0x2c8:rt:t4:s2 0x2d8:rs:t4:s2 0x2d8:rt:t5:s2 0x2dc:rt:t5:s2 0x300:rt:t6:s2 0x304:rs:t6:s2 0x304:rt:t6:s2 0x30c:rs:t6:s2 0x30c:rt:t7:s2 0x314:rt:t7:s2 0x330:rt:t8:t0 0x334:rs:t8:t0 0x334:rt:t8:t0 0x33c:rs:t8:s0 0x33c:rt:s0:t0 0x340:rt:t0:s2 0x344:rt:t0:s2 0x390:rd:t9:t2 0x3c4:rd:t2:t3 0x3c8:rs:t9:t2 0x3c8:rt:t2:t3 0x3dc:rd:t5:t7 0x408:rt:t5:t7 0x40c:rd:t3:t4 0x410:rs:t3:t4 0x410:rt:t4:t6 0x414:rt:t4:t6 0x418:rt:t6:t5 0x41c:rs:t6:t5 0x41c:rt:t6:t5 0x428:rt:t6:t5 0x42c:rt:t7:t8 0x430:rs:t7:t8 0x430:rt:t7:t8 0x438:rs:t7:t8 0x448:rd:t8:t9 0x480:rs:t8:t9 0x4c4:rt:t9:t2 0x4c8:rt:t9:t2 0x4d0:rt:t2:t3 0x4d4:rs:t2:t3 0x4d4:rt:t2:t3 0x4dc:rt:t2:t3 0x4e0:rt:t3:t4 0x4e8:rs:t3:t4 0x4fc:rt:t4:t6 0x500:rs:t4:t6 0x508:rs:t4:t6 0x508:rt:t4:t6 0x50c:rs:t4:t6 0x520:rt:t5:t7 0x524:rs:t5:t7 0x52c:rs:t5:t7 0x52c:rt:t5:t7 0x530:rd:t6:t5 0x530:rt:t5:t7 0x534:rs:t6:t5 0x534:rt:t7:t8 0x538:rt:t7:t8 0x554:rt:t8:t9 0x558:rs:t8:t9 0x560:rs:t8:t9 0x560:rt:t8:t9 0x564:rt:t8:t9 0x574:rt:t9:t2 0x578:rs:t9:t2 0x578:rt:t9:t2 0x57c:rt:t9:t2 0x580:rt:t2:t3 0x584:rt:t4:t6 0x588:rs:t4:t6 0x588:rt:t4:t6 0x58c:rd:t3:t4 0x58c:rs:t2:t3 0x590:rt:t3:t4 0x594:rt:t4:t6 0x598:rt:t5:t7 0x59c:rt:t6:t5 0x5a4:rd:t7:t8 0x5a4:rs:t5:t7 0x5a4:rt:t6:t5 0x5b0:rt:t7:t8 0x5cc:rd:t8:t9 0x5d8:rt:t8:t9 0x604:rt:t9:t2 0x608:rs:t9:t2 0x608:rt:t9:t2 0x618:rt:t2:t3 0x61c:rt:t3:t4 0x620:rt:t3:t4 0x624:rt:t2:t3 0x62c:rt:t9:t2 0x644:rt:t5:t7 0x648:rs:t5:t7 0x648:rt:t5:t7 0x654:rt:t4:t6 0x658:rt:t6:t5 0x65c:rt:t7:t8 0x660:rt:t7:t8 0x664:rt:t6:t5 0x668:rt:t4:t6 0x66c:rt:t5:t7 0x68c:rt:t8:t9 0x698:rt:t9:t2 0x69c:rt:t9:t2 0x6a8:rt:t8:t9 0x6b8:rt:a1:v1 0x6bc:rs:a1:v1 0x6cc:rt:t2:t3 0x6d4:rt:t2:t3 0x6d8:rt:t3:s2 0x6dc:rs:t3:s2 0x6dc:rt:t3:s2 0x6e4:rs:t3:s2 0x6e4:rt:t4:s2 0x6ec:rt:t4:s2 0x708:rt:t5:t4 0x70c:rs:t5:t4 0x70c:rt:t5:t4 0x714:rs:t5:s0 0x714:rt:s0:t4 0x718:rt:t6:s2 0x71c:rt:t6:s2 0x734:rt:t7:s2 0x738:rs:t7:s2 0x738:rt:t7:s2 0x740:rs:t7:s2 0x740:rt:t8:s2 0x748:rt:t8:s2 */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 device_cmd_send();
extern s32 get_CD_State2();
extern s32 spot_light();
extern s32 indexed_byte_toggle();
extern s32 dec_bg_init();
extern s32 hizuke_disp_switch();
extern s32 fade_in_step();
extern s32 fade_out_step();
extern s32 printf();
extern s32 rand();
extern s32 srand();
extern s32 date2_bg_cache_step2();

extern volatile s32 step_counter;
extern volatile u8 now_sub_sub_step;
extern u32 frame_counter;
extern s32 script_on;
extern s16 date2_intro_script_tbl1[];
extern u32 *mdec_bs;
extern char date2_spot_light_main_str0[];
extern u16 date2_wait_cnt;
s16 date2_spot_x[4] = {0};
s16 date2_spot_y[4] = {0};
extern s16 date2_spot_dx[];
extern s16 date2_spot_dy[];

s32 date2_spot_light_main(void)
{
    u32 state;
    s32 n2;
    s32 v;
    s32 three;
    short i;
    s16 *pA;
    s16 *pB;
    s16 *pC;
    s16 *pD;
    s32 a1;
    s32 vd;
    s32 n;
    s32 r;
    u16 m;
    u32 *p;
    u8 old;

    state = step_counter;
    switch (state) {
    case 0:
        script_on = 1;
        srand(frame_counter);
        date2_wait_cnt = 0;
        hizuke_disp_switch(0);
        date2_bg_cache_step2(17338);
        date2_spot_x[0] = -80;
        date2_spot_x[1] = 0;
        date2_spot_x[2] = 100;
        date2_spot_y[0] = -40;
        date2_spot_y[1] = 0;
        step_counter = step_counter + 1;
        date2_spot_y[2] = -60;
        three = 3;
        v = rand();
        date2_spot_dx[0] = (v % three) - 1;
        v = rand();
        date2_spot_dx[1] = (v % three) - 1;
        v = rand();
        date2_spot_dx[2] = (v % three) - 1;
        v = rand();
        date2_spot_dy[0] = (v % three) - 1;
        v = rand();
        date2_spot_dy[1] = (v % three) - 1;
        v = rand();
        date2_spot_dy[2] = (v % three) - 1;
        return v;
    case 1:
        if (get_CD_State2() & 1) {
            step_counter = step_counter + 1;
        }
        return;
    case 2:
        p = mdec_bs;
        if (((p[0] & 0xFFFF0000) == 0x38000000) &&
            ((p[1] & 0xFF010000) == 0x00010000)) {
            goto ok2;
        }
        printf(date2_spot_light_main_str0);
        date2_intro_script_tbl1[0] = 0;
        date2_intro_script_tbl1[1] = 0;
        step_counter = step_counter - 2;
        return 0;
ok2:
        dec_bg_init(1);
        indexed_byte_toggle(1);
        v = device_cmd_send(515);
        step_counter = step_counter + 1;
        return v;
    case 3:
        old = now_sub_sub_step;
        n2 = (s32)state;
        fade_in_step();
        step_counter = n2;
        if (old != now_sub_sub_step) {
            state = n2 + 1;
            step_counter = state;
        }
        now_sub_sub_step = old;
        return;
    case 4:
        three = 3;
        for (i = 0; i < 3; i++) {
            v = rand();
            if ((v % 30) == (frame_counter % 30)) {
                v = rand();
                date2_spot_dx[i] = (v % three) - 1;
            }
            pA = &date2_spot_dx[i];
            v = rand();
            if ((v % 30) == (frame_counter % 30)) {
                v = rand();
                date2_spot_dy[i] = (v % three) - 1;
            }
            pB = &date2_spot_dy[i];
            n = *pA;
            if (n == 0 && *pB == 0) {
                r = rand() % 2;
                if (r) {
                    *pA = (rand() % 2) * 2 - 1;
                    n = *pA;
                } else {
                    r = (rand() % 2) * 2 - 1;
                    n = *pA;
                    *pB = r;
                }
            }
            pC = &date2_spot_x[i];
            pD = &date2_spot_y[i];
            *pC = n + *pC;
            *pD = *pB + *pD;
            a1 = *pC;
            if (a1 < -149) {
                *pA = 1;
            }
            if (a1 >= 150) {
                *pA = -1;
            }
            vd = *pD;
            if (vd < -109) {
                *pB = 1;
            }
            if (vd >= 30) {
                *pB = -1;
            }
        }
        spot_light(date2_spot_x[0], date2_spot_y[0], 10, 2, 2, 0x00808080, 3, 1);
        spot_light(date2_spot_x[1], date2_spot_y[1], 10, 2, 2, 0x00804040, 3, 1);
        spot_light(date2_spot_x[2], date2_spot_y[2], 10, 2, 2, 0x00800000, 3, 1);
        m = date2_wait_cnt;
        date2_wait_cnt = m + 1;
        v = (m == 150);
        if (v) {
            step_counter = step_counter + 1;
        }
        return v;
    case 5:
        old = now_sub_sub_step;
        n2 = (s32)state;
        fade_out_step();
        step_counter = n2;
        if (old != now_sub_sub_step) {
            state = n2 + 1;
            step_counter = state;
        }
        now_sub_sub_step = old;
        return;
    case 6:
        v = indexed_byte_toggle(1);
        step_counter = step_counter + 1;
        return v;
    }
    device_cmd_send(512);
    script_on = 0;
    hizuke_disp_switch(1);
    return sub_sub_step_up();
}
