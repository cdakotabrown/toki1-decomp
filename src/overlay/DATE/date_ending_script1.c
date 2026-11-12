/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x70 */
/* @PostProc: swap_instructions --offset1 0x2bc --offset2 0x2c0 */
/* @PostProc: swap_instructions --offset1 0x334 --offset2 0x33c */
/* @PostProc: swap_instructions --offset1 0x4dc --offset2 0x4e0 */
/* @PostProc: swap_instructions --offset1 0x4e0 --offset2 0x4e4 */
/* @PostProc: swap_instructions --offset1 0x4e4 --offset2 0x4e8 */
/* @PostProc: swap_instructions --offset1 0x4e8 --offset2 0x4ec */
/* @PostProc: swap_instructions --offset1 0x4ec --offset2 0x4f0 */
/* @PostProc: swap_instructions --offset1 0x4f0 --offset2 0x4f4 */
/* @PostProc: swap_instructions --offset1 0x4f4 --offset2 0x4f8 */
/* @PostProc: swap_instructions --offset1 0x514 --offset2 0x51c */
/* @PostProc: swap_instructions --offset1 0x518 --offset2 0x51c */
/* @PostProc: swap_instructions --offset1 0x71c --offset2 0x720 */
/* @PostProc: swap_instructions --offset1 0xb0 --offset2 0xb8 */
/* @PostProc: swap_instructions --offset1 0xb4 --offset2 0xb8 */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xbc */
/* @PostProc: swap_instructions --offset1 0xbc --offset2 0xc0 */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xd0 */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xd4 */
/* @PostProc: swap_instructions --offset1 0xcc --offset2 0xdc */
/* @PostProc: swap_instructions --offset1 0x7a8 --offset2 0x7b0 */
/* @PostProc: patch_insn_reg_field 0x68:rt:t8:v1 0x78:rt:t8:v1 0x98:rt:t9:t8 0x9c:rt:t0:t9 0xa0:rt:t9:t8 0xa8:rt:t0:t9 0xb0:rt:t1:t0 0xb4:rt:t2:s2 0xb8:rt:t1:t0 0xc0:rs:t2:s2 0xc0:rt:t2:s2 0xc8:rt:t4:t1 0xcc:rt:t4:t1 0xd4:rs:t2:s2 0xd4:rt:t3:s2 0xdc:rt:t3:s2 0x110:rd:t5:t2 0x114:rs:t5:t2 0x114:rt:t6:t3 0x11c:rt:t6:t3 0x14c:rd:t7:t4 0x150:rs:t7:t4 0x150:rt:t8:t5 0x158:rt:t8:t5 0x188:rd:t9:t6 0x18c:rs:t9:t6 0x18c:rt:t0:t7 0x194:rt:t0:t7 0x1c4:rd:t1:t8 0x1c8:rs:t1:t8 0x1c8:rt:t2:t9 0x1d0:rt:t2:t9 0x200:rd:t3:t0 0x204:rs:t3:t0 0x204:rt:t4:t1 0x20c:rt:t4:t1 0x23c:rd:t5:t2 0x240:rs:t5:t2 0x240:rt:t6:t3 0x248:rt:t6:t3 0x254:rt:t7:t4 0x258:rs:t7:t4 0x25c:rt:t8:s2 0x260:rs:t8:s2 0x260:rt:t8:s2 0x268:rs:t8:s2 0x268:rt:t9:s2 0x270:rt:t9:s2 0x274:rt:t0:t5 0x278:rs:t0:t5 0x278:rt:t0:t5 0x280:rs:t0:t5 0x280:rt:t1:t6 0x288:rd:t2:t7 0x288:rs:t1:t6 0x290:rs:t2:t7 0x298:rs:t0:t5 0x298:rt:t3:t8 0x2a0:rd:t4:t9 0x2a0:rs:t3:t8 0x2a8:rs:t4:t9 0x2bc:rt:t5:s2 0x2c4:rs:t5:s2 0x2c4:rt:t5:s2 0x2d4:rs:t5:s2 0x2d4:rt:t6:s2 0x2d8:rt:t6:s2 0x2fc:rt:t7:s2 0x300:rs:t7:s2 0x300:rt:t7:s2 0x308:rs:t7:s2 0x308:rt:t8:s2 0x310:rt:t8:s2 0x32c:rt:t9:t0 0x330:rs:t9:t0 0x330:rt:t9:t0 0x338:rs:t9:s0 0x338:rt:s0:t0 0x33c:rt:t1:s2 0x340:rt:t1:s2 0x360:rt:t2:t1 0x364:rs:t2:t1 0x364:rt:t2:t1 0x36c:rs:t2:t1 0x370:rd:t0:t2 0x384:rs:t0:t2 0x398:rd:t6:t7 0x3c4:rt:t6:t7 0x3cc:rt:t5:t6 0x3d0:rt:t5:t6 0x3d4:rt:t7:t5 0x3d8:rs:t7:t5 0x3d8:rt:t7:t5 0x3e4:rt:t7:t5 0x408:rd:t1:t0 0x40c:rt:t1:t0 0x44c:rd:t2:t1 0x450:rs:t2:t1 0x450:rt:t0:t2 0x454:rt:t0:t2 0x488:rt:t5:t6 0x48c:rs:t5:t6 0x494:rs:t5:t6 0x494:rt:t5:t6 0x498:rs:t5:t6 0x4ac:rt:t6:t7 0x4b0:rs:t6:t7 0x4b8:rs:t6:t7 0x4b8:rt:t6:t7 0x4bc:rd:t7:t5 0x4bc:rt:t6:t7 0x4c0:rs:t7:t5 0x4f0:rd:t1:t0 0x4f4:rs:t1:t0 0x4f4:rt:t2:t1 0x4fc:rt:t2:t1 0x500:rt:t0:t2 0x504:rs:t0:t2 0x504:rt:t0:t2 0x508:rt:t0:t2 0x510:rt:t5:t6 0x514:rs:t5:t6 0x514:rt:t5:t6 0x520:rt:t5:t6 0x524:rt:t6:t7 0x528:rt:t7:t5 0x530:rs:t6:t7 0x530:rt:t7:t5 0x554:rt:t1:t0 0x558:rt:t1:t0 0x570:rt:t2:t1 0x574:rt:t2:t1 0x580:rt:t0:t2 0x584:rt:t0:t2 0x59c:rt:t6:t7 0x5a0:rs:t6:t7 0x5a0:rt:t6:t7 0x5ac:rt:t5:t6 0x5b0:rt:t7:t5 0x5bc:rt:t7:t5 0x5c0:rt:t5:t6 0x5c4:rt:t6:t7 0x5dc:rt:t1:t0 0x5e0:rs:t1:t0 0x5e0:rt:t1:t0 0x5f0:rt:t2:t1 0x5f4:rt:t0:t2 0x5f8:rt:t0:t2 0x5fc:rt:t2:t1 0x604:rt:t1:t0 0x62c:rt:t5:t6 0x630:rt:t6:t7 0x634:rt:t6:t7 0x638:rt:t5:t6 0x650:rt:t7:t5 0x654:rs:t7:t5 0x654:rt:t7:t5 0x65c:rs:t7:t5 0x69c:rt:t1:t0 0x6a0:rs:t1:t0 0x6a8:rs:t1:t0 0x6a8:rt:t1:t0 0x6b0:rs:t1:t0 0x6d4:rt:t2:t1 0x6d8:rs:t2:t1 0x6e0:rs:t2:t1 0x6e0:rt:t2:t1 0x6e8:rs:t2:t1 0x710:rt:v0:v1 0x714:rs:v0:v1 0x714:rt:v0:v1 0x71c:rs:v0:v1 0x71c:rt:t3:v0 0x720:rs:v0:v1 0x720:rt:t0:t3 0x724:rs:t3:v0 0x728:rt:t0:t3 0x734:rt:v0:v1 0x738:rs:v0:v1 0x738:rt:v0:v1 0x740:rs:v0:v1 0x748:rt:t4:v1 0x750:rt:t4:v1 0x754:rt:a0:v1 0x758:rs:a0:v1 0x758:rt:a0:v1 0x760:rs:a0:v1 0x768:rs:a0:v1 0x768:rt:t5:t4 0x770:rt:t5:t4 0x7ac:rs:t6:s0 0x7ac:rt:s0:t6 0x7b0:rt:t7:s2 0x7b4:rt:t7:s2 */
/* @RodataBase: 0x8015B330 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:213 IDO52_COPYPROP_VAR=-8:2,-8:4 IDO52_FORCE_WEB_COLOR=0:s2 IDO52_AS1_XBB_SKIP_CAND=5,6 IDO52_AS1_XBB_VOLATILE_OK=8 */
/* Address: 0x80148940 */
/* Size: 2068 bytes (517 instructions) */
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
extern s32 date_match_points();

extern volatile s32 step_counter;
extern volatile u8 now_sub_sub_step;
extern u32 frame_counter;
extern s32 script_on;
extern s16 date2_intro_script_tbl1[];
extern u32 *mdec_bs;
extern char date_ending_script1_str0[];
extern u16 date_script_cnt[];
s16 date_ending_spot_x[4] = {0};
s16 date_ending_spot_y[4] = {0};
extern s16 date_ending_spot_dx[];
extern s16 date_ending_spot_dy[];
extern s16 date_ending_spot_cnt[];

#define STATE  step_counter
#define VAL58  frame_counter
#define BYTE6A now_sub_sub_step

#define arrA(i) (date_ending_spot_dx[i])
#define arrB(i) (date_ending_spot_dy[i])
#define arrC(i) (date_ending_spot_x[i])
#define arrD(i) (date_ending_spot_y[i])
#define CTR     (date_ending_spot_cnt[0])
#define CTR2    (date_script_cnt[0])

s32 date_ending_script1(void)
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
    s16 cn;
    u16 m;
    u32 *p;
    u8 old;

    state = STATE;
    switch (state) {
    case 0:
        script_on = 1;
        CTR = 100;
        srand(VAL58);
        CTR2 = 0;
        hizuke_disp_switch(0);
        date_match_points(17338);
        arrC(0) = -80;
        arrC(1) = 0;
        arrC(2) = 100;
        arrD(0) = -40;
        arrD(1) = 0;
        STATE = STATE + 1;
        arrD(2) = -60;
        three = 3;
        v = rand();
        arrA(0) = (v % three) - 1;
        v = rand();
        arrA(1) = (v % three) - 1;
        v = rand();
        arrA(2) = (v % three) - 1;
        v = rand();
        arrB(0) = (v % three) - 1;
        v = rand();
        arrB(1) = (v % three) - 1;
        v = rand();
        arrB(2) = (v % three) - 1;
        return v;
    case 1:
        if (get_CD_State2() & 1) {
            STATE = STATE + 1;
        }
        return;
    case 2:
        p = mdec_bs;
        if (((p[0] & 0xFFFF0000) == 0x38000000) &&
            ((p[1] & 0xFF010000) == 0x00010000)) {
            goto ok2;
        }
        printf(date_ending_script1_str0);
        date2_intro_script_tbl1[0] = 0;
        date2_intro_script_tbl1[1] = 0;
        STATE = STATE - 2;
        return 0;
ok2:
        dec_bg_init(1);
        indexed_byte_toggle(1);
        v = device_cmd_send(515);
        STATE = STATE + 1;
        return v;
    case 3:
        old = BYTE6A;
        n2 = (s32)state;
        fade_in_step();
        STATE = n2;
        if (old != BYTE6A) {
            state = n2 + 1;
            STATE = state;
        }
        BYTE6A = old;
        return;
    case 4:
        three = 3;
        for (i = 0; i < 3; i++) {
            v = rand();
            if ((v % 30) == (VAL58 % 30)) {
                v = rand();
                arrA(i) = (v % three) - 1;
            }
            pA = &date_ending_spot_dx[i];
            v = rand();
            if ((v % 30) == (VAL58 % 30)) {
                v = rand();
                arrB(i) = (v % three) - 1;
            }
            pB = &date_ending_spot_dy[i];
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
            pC = &date_ending_spot_x[i];
            pD = &date_ending_spot_y[i];
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
        spot_light(arrC(0), arrD(0), 10, 2, 2, 0x00808080, 3, 1);
        spot_light(arrC(1), arrD(1), 10, 2, 2, 0x00804040, 3, 1);
        spot_light(arrC(2), arrD(2), 10, 2, 2, 0x00800000, 3, 1);
        if (VAL58 % 50 == 0) {
            if ((u32)rand() % 4 == 0) {
                device_cmd_send(1284);
            } else if (rand() % 4 == 1) {
                device_cmd_send(1285);
            } else if (rand() % 4 == 2) {
                device_cmd_send(1286);
            } else {
                device_cmd_send(1287);
                CTR = 0;
            }
        }
        cn = CTR;
        CTR = cn + 1;
        if ((cn ^ 20) == 0) {
            device_cmd_send(1287);
        }
        cn = CTR;
        if (cn >= 101) {
            CTR = 100;
        }
        m = CTR2;
        CTR2 = m + 1;
        v = (m == 300);
        if (v) {
            state = STATE + 1;
            STATE = state;
        }
        return v;
    case 5:

        old = BYTE6A;
        n2 = (s32)state;
        fade_out_step();
        STATE = n2;
        if (old != BYTE6A) {
            state = n2 + 1;
            STATE = state;
        }
        BYTE6A = old;
        return;
    case 6:
        v = indexed_byte_toggle(1);
        state = STATE + 1;
        STATE = state;

        return v;
    }
    device_cmd_send(512);
    script_on = 0;
    hizuke_disp_switch(1);
    return sub_sub_step_up();
}
