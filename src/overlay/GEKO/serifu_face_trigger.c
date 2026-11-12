/* @CompileEnv: IDO52_CONST_INREG_VALUES=1:-1:2:2,1:-1:5:5,1:-1:9:9,1:-1:12:12,1:-1:15:15,1:-1:18:18,1:-1:22:22,1:-1:25:25 */
/* @PostProc: patch_insn_reg_field 0xcc:rt:t3:t2 0x5c:rs:v1:t7 0x5c:rt:t7:v1 0x70:rt:t9:v0 0x74:rt:t0:t9 0x78:rs:t9:v0 0x7c:rt:t0:t9 0xa0:rt:t1:t0 0xa4:rs:t1:t0 0xa4:rt:t1:t0 0xac:rs:t1:t0 0xb0:rt:t2:t1 0xb4:rs:t2:t1 0xb4:rt:t2:t1 0xb8:rt:t3:t2 0xbc:rs:v1:t1 0xbc:rt:t2:v1 0xc4:rs:t3:t2 0xc4:rt:t3:t2 0xd0:rt:t4:v0 0xd4:rs:t3:t2 0xd4:rt:t5:t3 0xd8:rs:t4:v0 0xdc:rt:t5:t3 0x100:rt:t6:t4 0x110:rs:t6:t4 0x110:rt:t6:t4 0x118:rs:t6:t4 0x11c:rt:t7:t5 0x120:rs:t7:t5 0x120:rt:t7:t5 0x124:rt:t9:t6 0x128:rs:v1:t5 0x128:rt:t7:v1 0x130:rs:t9:t6 0x130:rt:t9:t6 0x138:rt:t9:t6 0x13c:rt:t8:v0 0x140:rs:t9:t6 0x140:rt:t0:t7 0x144:rs:t8:v0 0x148:rt:t0:t7 0x168:rt:t1:t8 0x16c:rs:t1:t8 0x16c:rt:t1:t8 0x174:rs:t1:t8 0x178:rt:t2:t9 0x17c:rs:t2:t9 0x17c:rt:t2:t9 0x180:rt:t4:t0 0x184:rs:v1:t9 0x184:rt:t2:v1 0x18c:rs:t4:t0 0x18c:rt:t4:t0 0x194:rt:t4:t0 0x198:rt:t3:v0 0x19c:rs:t4:t0 0x19c:rt:t5:t1 0x1a0:rs:t3:v0 0x1a4:rt:t5:t1 */
/* serifu_face_trigger -- GEKO overlay, Address: 0x80133AA0, Size: 432 bytes (108 insns) */
#include "types.h"

extern s32 get_g_zyotai_h(u8 a0);
extern s32 geko_serifu_run(void);
extern s32 bustup_face_change(s32 a0);

extern u8  now_girl;
extern s32 step_counter;
extern s16 geko_now_serifu_in;
extern u8  geko_serifu_state;

void serifu_face_trigger(void)
{
    u8  chr;
    s16 phase;
    u8  savedArg;
    u8  flag;
    u8  one;
    s32 counter;
    s32 hit;

    savedArg = (u8)get_g_zyotai_h(now_girl);
    geko_serifu_run();
    one = 1;

    phase = geko_now_serifu_in;
    if (phase == 2) {
        chr = now_girl;
        if (chr == 8) {
            flag = geko_serifu_state;
            if (flag == one) {
                counter = step_counter;
                hit = !(one ^ counter);
                step_counter = counter + 1;
                if (hit) {
                    bustup_face_change(3);
                    phase = geko_now_serifu_in;
                }
            }
        }
    }

    if (phase == 4) {
        chr = now_girl;
        if (chr == 8) {
            flag = geko_serifu_state;
            if (flag == one) {
                counter = step_counter;
                hit = !(one ^ counter);
                step_counter = counter + 1;
                if (hit) {
                    bustup_face_change(savedArg);
                }
            }
        }
    }

    phase = geko_now_serifu_in;
    if (phase == 1U || phase == 5) {
        chr = now_girl;
        if (chr == 10) {
            flag = geko_serifu_state;
            if (flag == one) {
                counter = step_counter;
                hit = !(one ^ counter);
                step_counter = counter + 1;
                if (hit) {
                    bustup_face_change(2);
                }
            }
        }
    }

    phase = geko_now_serifu_in;
    if (phase == 3) {
        chr = now_girl;
        if (chr == 10) {
            flag = geko_serifu_state;
            if (flag == one) {
                counter = step_counter;
                hit = !(one ^ counter);
                step_counter = counter + 1;
                if (hit) {
                    bustup_face_change(3);
                }
            }
        }
    }
}
