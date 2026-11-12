/* Function: serifu_main */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=49:t5,50:t4,51:t7,52:t8,53:t1,54:t2 */
/* Address: 0x8008296C */
/* Size: 1524 bytes (381 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x3b8 --offset2 0x3bc */
/* @PostProc: swap_instructions --offset1 0x3bc --offset2 0x3c0 */
/* @PostProc: swap_instructions --offset1 0x444 --offset2 0x448 */
/* @PostProc: swap_instructions --offset1 0x440 --offset2 0x444 */
/* @PostProc: swap_instructions --offset1 0x43c --offset2 0x440 */
/* @PostProc: swap_instructions --offset1 0x438 --offset2 0x43c */
/* @PostProc: swap_instructions --offset1 0x434 --offset2 0x438 */
/* @PostProc: swap_instructions --offset1 0x4e0 --offset2 0x4f0 */
/* @PostProc: swap_instructions --offset1 0x4f0 --offset2 0x4f8 */
/* @PostProc: swap_instructions --offset1 0x4e8 --offset2 0x4f8 */
/* @PostProc: patch_insn_reg_field 0x3e4:rs:t8:v0 0x3e4:rt:v0:t8 0x434:rt:t6:t5 0x438:rt:t5:t6 0x43c:rs:t5:t6 0x43c:rt:a1:t6 0x444:rs:a1:t6 0x44c:rs:t6:t5 0x44c:rt:t6:t5 0x454:rd:t5:t9 0x458:rs:t5:t9 0x458:rt:v0:t1 0x45c:rs:t6:t5 0x45c:rt:a0:v0 0x464:rs:a0:v0 0x464:rt:v0:t1 0x464:rd:t4:t2 0x468:rs:t4:t2 0x468:rt:t7:t3 0x46c:rt:t9:t8 0x470:rs:t7:t3 0x474:rt:t8:t4 0x478:rs:a1:t6 0x478:rt:t8:t4 0x47c:rt:t1:t5 0x480:rs:t1:t5 0x480:rt:t1:t5 0x488:rs:t1:t5 0x48c:rs:t9:t8 0x48c:rt:t9:t8 0x490:rs:a0:v0 0x490:rt:t2:t7 0x494:rs:t9:t8 0x494:rt:t2:t7 0x498:rt:t3:t9 0x49c:rs:t3:t9 0x49c:rt:t3:t9 0x4a0:rt:t6:t1 0x4a4:rs:t3:t9 0x4ac:rs:t6:t1 0x4ac:rt:t6:t1 0x4b0:rt:t5:t2 0x4b4:rs:t6:t1 0x4bc:rs:t5:t2 0x4bc:rt:t5:t2 0x4c4:rs:t5:t2 0x4d8:rt:t4:t3 0x4dc:rs:t4:t3 0x4dc:rt:t4:t3 0x4e0:rt:t1:t4 0x4e4:rs:t4:t3 0x4e8:rt:t7:t6 0x4ec:rs:t7:t6 0x4ec:rt:t7:t6 0x4f0:rs:t1:t4 0x4f0:rt:t1:t4 0x4f4:rs:t7:t6 0x4f4:rt:v1:t5 0x4f8:rt:t8:t1 0x4fc:rs:t8:t1 0x4fc:rt:t8:t1 0x500:rt:v1:t5 0x500:rd:t2:t7 0x504:rs:t1:t4 0x504:rt:t2:t7 0x504:rd:t9:t8 0x508:rs:t9:t8 0x508:rt:a1:t9 0x50c:rs:t8:t1 0x50c:rt:a0:t2 0x514:rs:a0:t2 0x514:rt:a1:t9 0x518:rt:a2:v0 0x520:rs:a2:v0 0x528:rs:a2:v0 0x530:rs:a2:v0 0x538:rs:a2:v0 0x540:rs:a2:v0 0x548:rs:a2:v0 0x550:rs:a2:v0 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 check_end_k(void);
extern s32 get_CD_State(void);
extern void device_cmd_send(s32 arg);
extern void k_reset(s32 arg);
extern void mes_arrow_anim(void);
extern s32 serifu_cmd_exec(s32, s32, s32, s32, s32, s32, s32);
extern void bustup_speech(void);
extern void bustup_flag_on(void);

extern s32 serifu_wait;
extern s32 serifu_voice;
extern s32 msg_wait;
extern s32 serifu_state;
extern u8 voice_level;
extern s32 pad_trig;
extern s16 sound_seek_time;
extern u8 xcut;
extern u8 yazirushi_cw_disp;
extern s32 yajirusi_timer;
extern s32 step_counter;

extern s16 *now_s;
extern s16 *now_s_in;
extern s32 *ss_s;
extern s32 *ss_s_kind;
extern s32 sss_so;
extern u8 sound_seek_wait;

extern s32 k_wait_cnt;
extern s32 bustup_on;
extern s32 mugon_flag;
extern s32 kuchi_stop;
extern s32 date_sprite_on;
extern s32 bustup_mode;

s32 serifu_main(u8 arg0, u8 arg1, u8 arg2)
{
    s32 state;
    s32 result;

    if (serifu_wait != 0) {
        if (check_end_k() != 0 &&
            (serifu_voice == 0 ||
             (serifu_voice == 1 && get_CD_State() != 4) ||
             voice_level == 0)) {
            serifu_wait = 0;
            sound_seek_time = 0;
        } else if ((pad_trig & 0x400040) != 0 && voice_level == 2) {
            device_cmd_send(180);
            serifu_wait = 2;
            sound_seek_time = 0;
            xcut = 1;
        }

        if (check_end_k() != 0 && serifu_wait == 2 && get_CD_State() != 4) {
            serifu_wait = 0;
            sound_seek_time = 0;
        }
        goto state_machine;
    }

    if (msg_wait != 0) {
        if (check_end_k() != 0) {
            state = serifu_voice;
            if (state == 0 || (state == 1 && get_CD_State() != 4) || voice_level == 0) {
                k_reset(1);
                msg_wait = 0;
                sound_seek_time = 0;
            }
        }
        goto state_machine;
    }

    if (serifu_state != 0) {
        if ((pad_trig & 0x400040) != 0 && voice_level == 2) {
            device_cmd_send(180);
            xcut = 1;
            serifu_state = 2;
            sound_seek_time = 0;
        }

        if (check_end_k() != 0 && serifu_state == 2 && get_CD_State() != 4) {
            k_reset(1);
            serifu_state = 0;
            yazirushi_cw_disp = yazirushi_cw_disp & 0xff7f;
            sound_seek_time = 0;
            serifu_voice = 0;
            return 1;
        }

        if ((pad_trig & 0x200020) != 0 && check_end_k() != 0) {
            state = serifu_voice;
            if (state == 0 || state == 10 || (state == 1 && get_CD_State() != 4) || voice_level == 0) {
                k_reset(1);
                serifu_state = 0;
                yazirushi_cw_disp = yazirushi_cw_disp & 0xff7f;
                sound_seek_time = 0;
                serifu_voice = 0;
                return 1;
            }
        }

        if (check_end_k() != 0) {
            state = serifu_voice;
            if (state == 0 || state == 10 ||
                (state == 1 && get_CD_State() != 4) || voice_level == 0) {
                yazirushi_cw_disp = yazirushi_cw_disp | 0x80;
                mes_arrow_anim();
                goto state_machine;
            }
        }
        yazirushi_cw_disp = yazirushi_cw_disp & 0x7F;
        goto state_machine;
    }

    {
        s16 *ptr1;
        s16 *ptr2;
        s32 *ptr3;
        s32 *ptr4;
        s16 idx1;
        s16 idx2;

        yajirusi_timer = 0;
        ptr1 = now_s;
        step_counter = 0;
        idx1 = ptr1[0];
        ptr2 = now_s_in;
        ptr3 = ss_s;
        ptr4 = ss_s_kind;
        idx2 = ptr2[0];

        result = serifu_cmd_exec(
            ((s32 *)(ptr3[idx1]))[idx2],
            ((u8 *)(ptr4[idx1]))[idx2],
            arg0,
            sss_so,
            idx1,
            idx2,
            arg2
        );

        if (arg1 != 0 && sound_seek_wait == 0 && (result & 0xff) != 100) {
            s16 *p;
            s32 *q;
            s16 v1_val;
            s16 v0_val;
            s32 t1_val;

            p = now_s;
            v1_val = p[0];
            q = ss_s_kind;
            v0_val = ((s16 *)now_s_in)[0];
            t1_val = q[v1_val];

            if (*((u8 *)(v0_val + t1_val)) == 255) {
                p[0] = v1_val + 1;
                ((s16 *)now_s_in)[0] = 0;
            } else {
                ((s16 *)now_s_in)[0] = (s16)(v0_val + 1);
            }
        }
    }

state_machine:
    if (serifu_wait != 0) goto do_func;
    if (serifu_state != 0) goto do_func;
    if (msg_wait == 0) goto done;
do_func:
    if (check_end_k() != 0) goto done;
    if (voice_level != 0) goto done;

    {
        s16 *p;
        s32 *q;
        s16 *r;
        s16 v1_val;
        s16 v0_val;
        s32 t1_val;
        u8 v0_byte;

        p = now_s;
        q = ss_s_kind;
        r = now_s_in;
        v1_val = p[0];
        v0_val = r[0];
        t1_val = q[v1_val];
        v0_byte = *(((u8 *)(v0_val + t1_val)) - 1);

        if (v0_byte != 1 && v0_byte != 2 && v0_byte != 5 &&
            v0_byte != 10 && v0_byte != 11 && v0_byte != 12 && v0_byte != 13) {
            goto done;
        }
    }

check_state:
    if (k_wait_cnt != 0) goto done;
    if (bustup_on == 0) goto call_67f2c;
    if (mugon_flag != 0) goto call_67f2c;
    if (kuchi_stop != 0) goto call_67f2c;
    bustup_speech();
    goto done;

call_67f2c:
    if (date_sprite_on == 0) goto done;
    if (bustup_mode != 0) goto done;
    if (mugon_flag != 0) goto done;
    bustup_flag_on();

done:
    return 0;
}
