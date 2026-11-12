/* @SymbolSize: 604 */
/* @CompileEnv: IDO52_AS1_FORCE_NARROW_FOLD=3 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x34 */
/* @PostProc: patch_insn_reg_field 0xf8:rt:t1:t2 0xfc:rs:t1:t2 0x104:rt:t2:t3 0x108:rs:t2:t3 0x108:rt:t2:t3 0x110:rs:t2:t3 0x114:rt:t3:t4 0x118:rs:t3:t4 0x118:rt:t3:t4 0x120:rs:t3:t4 0x120:rt:t4:t5 0x124:rt:t4:t5 0x130:rt:t5:t7 0x134:rs:t5:t7 0x13c:rt:t6:t8 0x140:rs:t6:t8 0x140:rt:t6:t8 0x148:rs:t6:t8 0x148:rt:t7:t9 0x14c:rs:t7:t9 0x164:rt:t8:t0 0x168:rs:t8:t0 0x168:rt:t8:t0 0x170:rs:t8:t0 0x1dc:rt:t9:t1 0x1e0:rs:t9:t1 0x1e0:rt:t9:t1 0x1e8:rt:t0:t2 0x1ec:rs:t0:t2 0x1ec:rt:t0:t2 0x1f0:rt:t9:t1 0x1f8:rt:t1:t3 0x1fc:rs:t1:t3 0x1fc:rt:t1:t3 0x200:rt:t0:t2 0x208:rt:t1:t3 0x214:rt:t2:t4 0x218:rs:t2:t4 0x218:rt:t2:t4 0x228:rs:t2:t4 0x228:rt:t3:t5 0x22c:rt:t3:t5 0x40:rd:s1:s0 0x48:rt:s1:s0 0x60:rs:s1:s0 0x84:rd:s0:s1 0x8c:rs:s1:s0 0x98:rs:s1:s0 0xa4:rs:s0:s1 0xb0:rs:s0:s1 0xc8:rs:s1:s0 0xd0:rs:s1:s0 0xd0:rt:s1:s0 0xd4:rt:s1:s0 0xd8:rd:s1:s0 0xdc:rs:s1:s0 */
#include "types.h"

extern u8  girl_appear[10];
extern u8  shugaku_place;
extern u8  day_flag;
extern u8  ret_step;
extern u8  ret_sub_step;
extern u8  now_girl;
extern u8  now_sub_sub_step;
extern s32 shugaku_pbirth_ss_serifu;
extern s32 shugaku_pbirth_ss_serifu_kind;
extern s32 shugaku_pbirth_sss_sound;
extern s16 shugaku_pbirth_now_serifu;
extern s16 shugaku_pbirth_now_serifu_in;
extern s32 shugaku_pbirth_serifu_tbl[];
extern s32 shugaku_pbirth_serifu_kind_tbl[];
extern s32 shugaku_pbirth_sound_tbl[];

extern s32  get_g_zyotai_s(s32 a0);
extern s32  get_h_yuukou(s32 a0);
extern s32  get_h_tokimeki(s32 a0);
extern void date_name_set(s32 a0);
extern void shugaku_girl_ptr_set(void);
extern void event_log_add(s32 a0, s32 a1);
extern s32 sub_sub_step_up(void);

s32 shugaku_sel_pick(void)
{
    s32 best_i;
    s32 best_a0;
    s16 i;
    s32 v0;
    s32 v1;
    s32 a0v;
    u8  lvu;
    s32 sc1, sc2, sc3, sc4;
    s32 picks;
    u8  bi;
    u8  t4;

    best_i = 255;
    best_a0 = 4;

    for (i = 0; i < 10; i++) {
        if (girl_appear[i] == 0) {
            continue;
        }
        v0 = get_g_zyotai_s(i);
        v1 = v0 & 0x7F;
        a0v = v1 & 0xFF;
        lvu = v1;
        if ((u32)a0v < (u32)best_a0) {
            goto update;
        }
        if (a0v != best_a0) {
            continue;
        }
        bi = best_i;
        if (a0v != best_a0) {
            continue;
        }
        sc1 = get_h_yuukou(i) & 0xFF;
        sc2 = get_h_tokimeki(i) & 0xFF;
        sc3 = get_h_tokimeki(bi) & 0xFF;
        sc4 = get_h_yuukou(bi);
        if ((u32)(sc4 + sc3) >= (u32)(sc2 + sc1)) {
            continue;
        }
    update:
        best_i = i & 0xFF;
        best_a0 = lvu & 0xFF;
    }

    v0 = get_g_zyotai_s(best_i);
    if (best_i == 4 && (u32)(v0 & 0x7F) < 2U && ret_sub_step == 5) {
        t4 = day_flag;
        day_flag = (u8)(t4 | 8);
    }

    if (best_i != 255 && (u32)(v0 & 0x7F) < 2U && (day_flag & 8) == 0) {
        picks = best_i & 0xFF;
        now_girl = picks;
        date_name_set(picks);
        shugaku_girl_ptr_set();
        if (ret_step != 6) {
            event_log_add(13, 38);
        } else {
            switch ((u32)shugaku_place) {
            case 0:
                event_log_add(13, 39);
                break;
            case 1:
                event_log_add(13, 40);
                break;
            default:
                event_log_add(13, 41);
                break;
            }
        }
    } else {
        shugaku_pbirth_ss_serifu = shugaku_pbirth_serifu_tbl[11];
        shugaku_pbirth_ss_serifu_kind = shugaku_pbirth_serifu_kind_tbl[11];
        shugaku_pbirth_sss_sound = shugaku_pbirth_sound_tbl[11];
        shugaku_pbirth_now_serifu = 0;
        shugaku_pbirth_now_serifu_in = 0;
        t4 = now_sub_sub_step;
        now_sub_sub_step = (u8)(t4 + 32);
        return 0;
    }
    return sub_sub_step_up();
}
