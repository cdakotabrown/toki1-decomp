/* Address: 0x80132830 */
/* Size: 800 bytes (200 insns) */
/* @RodataBase: 0x8013ACB0 */
/* @SymbolStart: 0x8013282C */
/* @SymbolSize: 804 */
/* @PostProc: patch_insn_reg_field 0x1D4:rt:t8:t9 0x1d8:rs:t8:t9 0x1e0:rt:t9:t0 0x1e4:rs:t9:t0 0x1e4:rt:t9:t0 0x1ec:rt:t0:t1 0x1f0:rs:t0:t1 0x1f0:rt:t0:t1 0x1f4:rt:t9:t0 0x1fc:rt:t1:t2 0x200:rs:t1:t2 0x200:rt:t1:t2 0x204:rt:t0:t1 0x210:rt:t1:t2 0x214:rt:t2:t3 0x218:rs:t2:t3 0x218:rt:t2:t3 0x220:rt:t3:t4 0x224:rs:t3:t4 0x224:rt:t3:t4 0x228:rt:t2:t3 0x230:rt:t4:t5 0x234:rs:t4:t5 0x234:rt:t4:t5 0x238:rt:t3:t4 0x244:rt:t4:t5 0x248:rt:t5:t6 0x24c:rs:t5:t6 0x24c:rt:t5:t6 0x254:rt:t6:t7 0x258:rs:t6:t7 0x258:rt:t6:t7 0x25c:rt:t5:t6 0x264:rt:t7:t8 0x268:rs:t7:t8 0x268:rt:t7:t8 0x26c:rt:t6:t7 0x278:rt:t7:t8 0x27c:rt:t8:t9 0x280:rs:t8:t9 0x280:rt:t8:t9 0x288:rt:t9:t0 0x28c:rs:t9:t0 0x28c:rt:t9:t0 0x290:rt:t8:t9 0x298:rt:t0:t1 0x29c:rs:t0:t1 0x29c:rt:t0:t1 0x2a0:rt:t9:t0 0x2ac:rt:t0:t1 0x2b0:rt:t1:t2 0x2b4:rs:t1:t2 0x2b4:rt:t1:t2 0x2bc:rt:t2:t3 0x2c0:rs:t2:t3 0x2c0:rt:t2:t3 0x2c4:rt:t1:t2 0x2cc:rt:t3:t4 0x2d0:rs:t3:t4 0x2d0:rt:t3:t4 0x2d4:rt:t2:t3 0x2dc:rt:t3:t4 0x2e0:rt:t4:t5 0x2e4:rs:t4:t5 0x2e4:rt:t4:t5 0x2e8:rt:t5:t6 0x2ec:rs:t4:t5 0x2f4:rt:t7:t8 0x2f8:rs:t5:t6 0x2f8:rt:t5:t6 0x300:rs:t5:t6 0x300:rt:t6:t7 0x308:rt:t6:t7 0x30c:rs:t7:t8 0x30c:rt:t7:t8 0x314:rs:t7:t8 0x314:rt:t8:t9 0x318:rt:t8:t9 */
#include "types.h"

extern s32 get_g_zyotai_s(s32);
extern void date_name_set(s32);
extern void sub_sub_step_up(void);

extern u8  now_girl;
extern u8  now_year;
extern u8  now_month;
extern u8  shugaku_gakunen;
extern s32 shugaku_birth_serifu_tbl[];
extern s32 shugaku_birth_serifu_kind_tbl[];
extern s32 shugaku_birth_sound_tbl[];
extern s32 shugaku_birth_ss_serifu;
extern s32 shugaku_birth_ss_serifu_kind;
extern s32 shugaku_birth_sss_sound;
extern s16 shugaku_birth_now_serifu;
extern s16 shugaku_birth_now_serifu_in;

void shugaku_birth_serifu_set(void) {
    shugaku_birth_now_serifu = 0;
    shugaku_birth_now_serifu_in = 0;
    date_name_set(now_girl);
    switch (now_girl) {
    case 0:
        shugaku_birth_ss_serifu = shugaku_birth_serifu_tbl[1];
        shugaku_birth_ss_serifu_kind = shugaku_birth_serifu_kind_tbl[1];
        shugaku_birth_sss_sound = shugaku_birth_sound_tbl[1];
        break;
    case 1:
        shugaku_birth_ss_serifu = shugaku_birth_serifu_tbl[5];
        shugaku_birth_ss_serifu_kind = shugaku_birth_serifu_kind_tbl[5];
        shugaku_birth_sss_sound = shugaku_birth_sound_tbl[5];
        break;
    case 2:
        shugaku_birth_ss_serifu = shugaku_birth_serifu_tbl[2];
        shugaku_birth_ss_serifu_kind = shugaku_birth_serifu_kind_tbl[2];
        shugaku_birth_sss_sound = shugaku_birth_sound_tbl[2];
        break;
    case 3:
        shugaku_birth_ss_serifu = shugaku_birth_serifu_tbl[3];
        shugaku_birth_ss_serifu_kind = shugaku_birth_serifu_kind_tbl[3];
        shugaku_birth_sss_sound = shugaku_birth_sound_tbl[3];
        break;
    case 4:
        shugaku_birth_ss_serifu = shugaku_birth_serifu_tbl[10];
        shugaku_birth_ss_serifu_kind = shugaku_birth_serifu_kind_tbl[10];
        shugaku_birth_sss_sound = shugaku_birth_sound_tbl[10];
        break;
    case 5:
        shugaku_birth_ss_serifu = shugaku_birth_serifu_tbl[6];
        shugaku_birth_ss_serifu_kind = shugaku_birth_serifu_kind_tbl[6];
        shugaku_birth_sss_sound = shugaku_birth_sound_tbl[6];
        break;
    case 6:
        shugaku_birth_ss_serifu = shugaku_birth_serifu_tbl[8];
        shugaku_birth_ss_serifu_kind = shugaku_birth_serifu_kind_tbl[8];
        shugaku_birth_sss_sound = shugaku_birth_sound_tbl[8];
        if ((u32)(get_g_zyotai_s(6) & 0x7F) < 2) {
            shugaku_birth_ss_serifu = shugaku_birth_serifu_tbl[7];
            shugaku_birth_ss_serifu_kind = shugaku_birth_serifu_kind_tbl[7];
            shugaku_birth_sss_sound = shugaku_birth_sound_tbl[7];
        }
        break;
    case 7:
        shugaku_birth_ss_serifu = shugaku_birth_serifu_tbl[4];
        shugaku_birth_ss_serifu_kind = shugaku_birth_serifu_kind_tbl[4];
        shugaku_birth_sss_sound = shugaku_birth_sound_tbl[4];
        break;
    case 8:
        shugaku_birth_ss_serifu = shugaku_birth_serifu_tbl[0];
        shugaku_birth_ss_serifu_kind = shugaku_birth_serifu_kind_tbl[0];
        shugaku_birth_sss_sound = shugaku_birth_sound_tbl[0];
        break;
    case 9:
        shugaku_birth_ss_serifu = shugaku_birth_serifu_tbl[9];
        shugaku_birth_ss_serifu_kind = shugaku_birth_serifu_kind_tbl[9];
        shugaku_birth_sss_sound = shugaku_birth_sound_tbl[9];
        break;
    case 10:
        shugaku_birth_ss_serifu = shugaku_birth_serifu_tbl[12];
        shugaku_birth_ss_serifu_kind = shugaku_birth_serifu_kind_tbl[12];
        shugaku_birth_sss_sound = shugaku_birth_sound_tbl[12];
        break;
    }
    if ((u32)now_month >= 4) {
        shugaku_gakunen = now_year - 95;
    } else {
        shugaku_gakunen = now_year - 96;
    }
    sub_sub_step_up();
}
