/* @SymbolStart: 0x801399A0 */
/* @SymbolSize: 656 */
/* @RodataBase: 0x8013AEC8 */
/* @PostProc: patch_insn_reg_field 0x1B4:rt:t8:t9 0x1b8:rs:t8:t9 0x1c0:rt:t9:t0 0x1c4:rs:t9:t0 0x1c4:rt:t9:t0 0x1cc:rt:t0:t1 0x1d0:rs:t0:t1 0x1d0:rt:t0:t1 0x1d4:rt:t9:t0 0x1dc:rt:t1:t2 0x1e0:rs:t1:t2 0x1e0:rt:t1:t2 0x1e4:rt:t0:t1 0x1f0:rt:t1:t2 0x1f4:rt:t2:t3 0x1f8:rs:t2:t3 0x1f8:rt:t2:t3 0x200:rt:t3:t4 0x204:rs:t3:t4 0x204:rt:t3:t4 0x208:rt:t2:t3 0x210:rt:t4:t5 0x214:rs:t4:t5 0x214:rt:t4:t5 0x218:rt:t3:t4 0x224:rt:t4:t5 0x228:rt:t5:t6 0x22c:rs:t5:t6 0x22c:rt:t5:t6 0x234:rt:t6:t7 0x238:rs:t6:t7 0x238:rt:t6:t7 0x23c:rt:t5:t6 0x244:rt:t7:t8 0x248:rs:t7:t8 0x248:rt:t7:t8 0x24c:rt:t6:t7 0x258:rt:t7:t8 0x25c:rt:t8:t9 0x260:rs:t8:t9 0x260:rt:t8:t9 0x268:rt:t9:t0 0x26c:rs:t9:t0 0x26c:rt:t9:t0 0x270:rt:t8:t9 0x278:rt:t0:t1 0x27c:rs:t0:t1 0x27c:rt:t0:t1 0x280:rt:t9:t0 0x288:rt:t0:t1 */
#include "types.h"

extern s32  get_g_zyotai_s(s32 a0);

extern u8 now_girl;
extern s32 ss_serifu_shugaku;
extern s32 ss_serifu_shugaku_kind;
extern s32 sss_sound_shugaku;

extern s32 shugaku_serifu_tbl[];
extern s32 shugaku_serifu_kind_tbl[];
extern s32 shugaku_sound_tbl[];

void shugaku_serifu_set(void)
{
    switch (now_girl) {
    case 0:
        ss_serifu_shugaku = shugaku_serifu_tbl[1];
        ss_serifu_shugaku_kind = shugaku_serifu_kind_tbl[1];
        sss_sound_shugaku = shugaku_sound_tbl[1];
        break;
    case 1:
        ss_serifu_shugaku = shugaku_serifu_tbl[5];
        ss_serifu_shugaku_kind = shugaku_serifu_kind_tbl[5];
        sss_sound_shugaku = shugaku_sound_tbl[5];
        break;
    case 2:
        ss_serifu_shugaku = shugaku_serifu_tbl[2];
        ss_serifu_shugaku_kind = shugaku_serifu_kind_tbl[2];
        sss_sound_shugaku = shugaku_sound_tbl[2];
        break;
    case 3:
        ss_serifu_shugaku = shugaku_serifu_tbl[3];
        ss_serifu_shugaku_kind = shugaku_serifu_kind_tbl[3];
        sss_sound_shugaku = shugaku_sound_tbl[3];
        break;
    case 4:
        ss_serifu_shugaku = shugaku_serifu_tbl[10];
        ss_serifu_shugaku_kind = shugaku_serifu_kind_tbl[10];
        sss_sound_shugaku = shugaku_sound_tbl[10];
        break;
    case 5:
        ss_serifu_shugaku = shugaku_serifu_tbl[6];
        ss_serifu_shugaku_kind = shugaku_serifu_kind_tbl[6];
        sss_sound_shugaku = shugaku_sound_tbl[6];
        break;
    case 6:
        ss_serifu_shugaku = shugaku_serifu_tbl[8];
        ss_serifu_shugaku_kind = shugaku_serifu_kind_tbl[8];
        sss_sound_shugaku = shugaku_sound_tbl[8];
        if ((u32)(get_g_zyotai_s(6) & 0x7F) < 2) {
            ss_serifu_shugaku = shugaku_serifu_tbl[7];
            ss_serifu_shugaku_kind = shugaku_serifu_kind_tbl[7];
            sss_sound_shugaku = shugaku_sound_tbl[7];
        }
        break;
    case 7:
        ss_serifu_shugaku = shugaku_serifu_tbl[4];
        ss_serifu_shugaku_kind = shugaku_serifu_kind_tbl[4];
        sss_sound_shugaku = shugaku_sound_tbl[4];
        break;
    case 8:
        ss_serifu_shugaku = shugaku_serifu_tbl[0];
        ss_serifu_shugaku_kind = shugaku_serifu_kind_tbl[0];
        sss_sound_shugaku = shugaku_sound_tbl[0];
        break;
    case 9:
        ss_serifu_shugaku = shugaku_serifu_tbl[9];
        ss_serifu_shugaku_kind = shugaku_serifu_kind_tbl[9];
        sss_sound_shugaku = shugaku_sound_tbl[9];
        break;
    }
}
