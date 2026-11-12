/* @SymbolStart: 0x801387D8 */
/* @SymbolSize: 748 */
/* @PostProc: patch_insn_reg_field 0xa4:rs:t0:v1 0xa4:rt:v1:t0 0xc4:rs:t2:v1 0xc4:rt:v1:t2 0xfc:rt:t6:t5 0x104:rt:t7:t6 0x108:rs:t7:t6 0x108:rt:t7:t6 0x10c:rt:t8:t7 0x110:rs:t7:t6 0x114:rt:t9:t8 0x118:rs:t8:t7 0x118:rt:t8:t7 0x120:rs:t8:t7 0x134:rs:t9:t8 0x134:rt:t9:t8 0x13c:rs:t9:t8 0x140:rt:t0:t9 0x144:rs:t0:t9 0x144:rt:t0:t9 0x14c:rs:t0:t9 0x16c:rt:t2:t1 0x178:rt:t2:t1 0x188:rt:t4:t3 0x190:rt:t6:t5 0x194:rs:t6:t5 0x194:rt:t6:t5 0x19c:rs:t6:t5 0x1c0:rt:t9:t8 0x1c8:rt:t0:t9 0x1cc:rs:t0:t9 0x1cc:rt:t0:t9 0x1d0:rt:t1:t0 0x1d4:rs:t0:t9 0x1e0:rt:t1:t0 0x1f0:rt:t3:t2 0x1f8:rt:t4:t3 0x1fc:rs:t4:t3 0x1fc:rt:t4:t3 0x200:rt:t6:t5 0x204:rs:t4:t3 0x210:rt:t6:t5 0x234:rt:t7:t6 0x240:rt:t7:t6 0x24c:rt:t9:t8 0x250:rs:t9:t8 0x250:rt:t9:t8 0x254:rt:t0:t9 0x258:rs:t9:t8 0x264:rt:t0:t9 0x270:rt:t1:t0 0x274:rs:t1:t0 0x274:rt:t1:t0 0x27c:rs:t1:t0 0x290:rt:t2:t1 0x2a4:rs:t2:t1 0x2a4:rt:t2:t1 0x2ac:rs:t2:t1 0x2b0:rt:t3:t2 0x2b4:rs:t3:t2 0x2b4:rt:t3:t2 0x2bc:rs:t3:t2 0x2c0:rt:t4:t3 0x2c4:rs:t4:t3 0x2c4:rt:t4:t3 0x2cc:rs:t4:t3 0x2dc:rt:t6:t5 0x2e0:rs:t6:t5 0x2e0:rt:t6:t5 0x2ec:rs:t6:t5 0x2ec:rt:t8:t7 0x2f0:rt:t8:t7 0xe4:rt:v1:a1 0xe8:rs:t3:a0 0xe8:rt:a0:v1 0xec:rd:t4:t3 0xec:rs:v1:a1 0xec:rt:a0:v1 0xf0:rs:t4:t3 0xf0:rt:t4:t3 0xf4:rt:t5:t4 0xf8:rd:t6:t5 0xf8:rs:t5:t4 0xf8:rt:t4:t3 0x100:rd:t3:t2 0x100:rs:a0:a1 0x128:rt:v1:a1 0x150:rt:t1:t0 0x154:rs:t1:t0 0x154:rt:t1:t0 0x15c:rs:v1:t0 0x15c:rt:t1:a1 0x17c:rs:t3:t2 0x17c:rt:t3:t2 0x180:rt:t5:t4 0x184:rd:t4:t3 0x184:rs:t5:t4 0x184:rt:t3:t2 0x18c:rd:t7:t6 0x18c:rs:a0:a1 0x1b4:rs:t7:t6 0x1b4:rt:t7:t6 0x1b8:rt:t8:t7 0x1bc:rd:t9:t8 0x1bc:rs:t8:t7 0x1bc:rt:t7:t6 0x1c4:rd:t2:t1 0x1c4:rs:a0:a1 0x1e4:rs:t2:t1 0x1e4:rt:t2:t1 0x1e8:rt:t5:t4 0x1ec:rd:t3:t2 0x1ec:rs:t5:t4 0x1ec:rt:t2:t1 0x218:rt:t8:t7 0x21c:rs:t8:t7 0x21c:rt:t8:t7 0x224:rs:t8:t7 0x224:rt:v1:a1 0x278:rt:t5:t4 0x284:rs:t5:t4 0x284:rt:t5:t4 0x28c:rs:t5:t4 0x28c:rt:v1:a1 0x68:rt:t3:a0 0xe0:rs:t3:a0 0xe0:rt:t3:a0 */
/* @CompileEnv: IDO52_NO_CSE=op IDO52_EXCLUDE_REGS_WEB=42:v1 IDO52_FORM_SPLIT=const:1:70 IDO52_UGEN_DEST_TEMP=15 IDO52_AS1_XBB_VOLATILE_OK=all */
/* ending_girl6_cue_step - ENDING overlay, Address: 0x801387E0, Size: 740 bytes (185 insns) */
#include "types.h"

extern void ending_bustup_anim_set(s32, s32);

extern s16 ending_now_serifu_in;
extern volatile s32 ending_serifu_frame;
extern s16 bustup2_cw_anim;
extern s32 wink2_stop;
extern u8  ending_wait_done;
extern u8  ending_wait_cnt;
extern s16 bustup_cw_anim;
extern s16 bustup_cw_frame;
extern s16 cw30_tick;
extern u8  mascot_flag2_6;

void ending_girl6_cue_step(void)
{
    s32 sel;
    s32 b;

    sel = ending_now_serifu_in;

    if (sel == 1) {
        if (ending_serifu_frame == 0) {
            bustup2_cw_anim = 7;
            ending_bustup_anim_set(0, 255);
            wink2_stop = 0;
            ending_wait_done = 0;
        }
        goto increment;
    }
    if (sel == 8) {
        if (ending_serifu_frame == 200) {
            ending_bustup_anim_set(1, 5);
            wink2_stop = 1;
        } else if (bustup_cw_anim == 1) {
            if (bustup_cw_frame == 2) {
                if (cw30_tick == 1) {
                    ending_bustup_anim_set(2, 0);
                    wink2_stop = 0;
                }
            }
        }
        goto increment;
    }

    b = mascot_flag2_6 & 1;

    if (sel == 12 - (b == 1)) {
        if (ending_wait_done == 0) {
            if (ending_wait_cnt == 10) {
                *(s8 *)&ending_wait_done = 1;
                ending_wait_cnt = 0;
            }
        }
        if (ending_wait_cnt == 30) {
            if (bustup_cw_anim == 2) {
                if (ending_wait_done == 1) {
                    ending_bustup_anim_set(3, 0);
                    bustup2_cw_anim = 8;
                }
            }
        }
        goto increment;
    }
    if (sel == 14 - (b == 1)) {
        if (ending_serifu_frame == 0) {
            ending_bustup_anim_set(0, 0);
        }
        goto increment;
    }
    if (sel == 19 - (b == 1)) {
        if (ending_serifu_frame == 0) {
            bustup2_cw_anim = 7;
        }
        goto increment;
    }
    if (sel == 22 - (b == 1)) {
        if (ending_serifu_frame == 0) {
            bustup2_cw_anim = 8;
        }
        goto increment;
    }
    if (sel == 24) {
        if (ending_serifu_frame == 1) {
            ending_bustup_anim_set(4, 5);
            wink2_stop = 1;
        }
        goto increment;
    }
    if (sel == 26) {
        if (ending_serifu_frame == 0) {
            bustup2_cw_anim = 7;
        }
        goto increment;
    }
    if (sel == 28) {
        if (ending_serifu_frame != 0) {
            if (ending_serifu_frame == 1) {
                ending_bustup_anim_set(5, 5);
            } else if (bustup_cw_anim == 5) {
                if (bustup_cw_frame == 4) {
                    if (cw30_tick == 13) {
                        ending_bustup_anim_set(6, 1);
                    }
                }
            }
        }
    }

increment:
    ending_serifu_frame = ending_serifu_frame + 1;
}
