/* @PostProc: patch_insn_reg_field 0x204:rs:a2:a0 0xf8:rt:a2:a1 0x100:rs:a2:a1 0x100:rt:a2:a1 0x108:rs:a2:a1 0x110:rt:v0:v1 0x118:rs:v0:v1 0x118:rt:v0:v1 0x124:rs:v0:v1 0x12c:rt:v1:v0 0x130:rd:v0:v1 0x138:rs:v1:v0 0x138:rt:v1:v0 0x140:rs:v0:v1 0x140:rt:v1:v0 0x148:rd:a1:s0 0x14c:rd:v0:v1 0x14c:rs:v1:v0 0x158:rd:v1:a2 0x15c:rs:v0:v1 0x164:rd:s0:v1 0x174:rs:a2:a1 0x1a8:rt:a2:a0 0x1ac:rs:a2:a0 0x1ac:rt:a2:a0 0x1b4:rt:a2:a0 0x1bc:rt:a2:a0 0x1c0:rd:a0:a1 0x1c8:rs:a0:a1 0x1e8:rt:a1:a2 0x200:rs:a0:a1 0x21c:rs:a1:a2 0x224:rs:a1:a2 0x230:rt:a1:a2 0x238:rs:a1:a2 0x250:rd:a1:a2 0x250:rt:a1:a2 0x288:rs:a1:a2 0x2a0:rs:a1:a2 0x2b4:rs:a1:a2 0x2c8:rs:a1:a2 */
/* ending_kuchi_level_set - ENDING overlay, Address: 0x80137758, */
/* Size: 888 bytes (222 instructions) */
#include "types.h"
#include "tokimeki/class_w.h"

typedef struct {
    s16 f0;
    s16 f2;
    u8  f4;
    u8  f5;
    u8  f6;
    s8  f7;
} Actor;

extern s16 *get_ksys(void);
extern Actor *get_k_work();
extern s32 get_CD_State(void);
extern s32 SD_CDLevelGet(void);

extern s16  ending_now_serifu_in;
extern s16  ending_now_serifu;
extern u8 **ending_ss_serifu_kind;
extern s16  bustup2_cw_anim;
extern s32  frame_counter;
extern s16  kuchi_level[];
extern u8   now_girl;
extern volatile u8 bustup_cd_read_tbl0;
extern u8   bustup_kuchi_ani_tbl0[];
extern s32  pad_data;
extern CLASS_W class_w[];

s32 ending_kuchi_level_set(void)
{
    s16 *cnt;
    Actor *ac;
    s16 i;
    s32 cue;
    s32 vote;
    s32 mask;
    s32 best;
    s32 bestIdx;
    s32 second;
    s32 sel;
    s32 thr;
    s32 hit;
    s32 step;

    cnt = get_ksys();
    if (ending_now_serifu_in <= 0) {
        return 0;
    }

    cue = ending_ss_serifu_kind[ending_now_serifu][ending_now_serifu_in - 1];
    if (cue == 0 || cue == 6) {
        for (i = 1; i <= *cnt; i = (s16)(i + 1)) {
            ac = get_k_work(i);
            ac->f7 = 1;
        }
        class_w[31].frame = 0;
        return 0;
    }

    if ((get_CD_State() & 4) != 0) {
        vote = SD_CDLevelGet();
    } else {
        vote = 0;
    }

    mask = frame_counter;
    kuchi_level[mask & 3] = (s16)vote;

    best = kuchi_level[0];
    bestIdx = 0;
    if (best < kuchi_level[1]) {
        best = kuchi_level[1];
        bestIdx = 1;
    }
    if (best < kuchi_level[2]) {
        best = kuchi_level[2];
        bestIdx = 2;
    }
    if (best < kuchi_level[3]) {
        best = kuchi_level[3];
        bestIdx = 3;
    }

    second = 0;
    sel = 0;
    for (i = 0; i < 4; i = (s16)(i + 1)) {
        if (i != bestIdx) {
            if (second < kuchi_level[i]) {
                second = kuchi_level[i];
            }
        }
    }

    step = now_girl;
    for (i = 0; i < 3; i = (s16)(i + 1)) {
        thr = bustup_kuchi_ani_tbl0[step * 3 + i];
        if (i == 0) {
            thr = 8;
        }
        if ((u32)thr < (u32)second) {
            sel = i + 1;
        }
    }

    hit = 0;
    if (step == 0) {
        if (bustup2_cw_anim == 8) {
            hit = 1;
        }
    }
    if (sel >= 4) {
        sel = 3;
    }
    bustup_cd_read_tbl0 = (u8)hit;
    if (sel != 0) {
        bustup_cd_read_tbl0 = (u8)hit;
        if (hit != 0) {
            sel = 3 - sel;
        }
    }

    if (class_w[31].frame != 0 && (mask & 1) != 0) {
    } else {
        if (second == 0) {
            class_w[31].frame = 0;
        } else if (sel == 0) {
            class_w[31].frame = 0;
        } else if (sel == 1) {
            class_w[31].frame = 1;
        } else if (sel == 2) {
            class_w[31].frame = 2;
        } else if (sel >= 3) {
            class_w[31].frame = 3;
        }
    }

    if (pad_data != 0) {
        for (i = 1; i <= *cnt; i = (s16)(i + 1)) {
            ac = get_k_work(i);
            if (ac->f2 >= 72) {
                ac->f7 = 1;
            }
        }
        return 0;
    }

    for (i = 1; i <= *cnt; i = (s16)(i + 1)) {
        ac = get_k_work(i);
        ac->f7 = 0;
    }
}
