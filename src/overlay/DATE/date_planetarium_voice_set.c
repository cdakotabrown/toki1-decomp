/* @RodataBase: 0x8015AEA0 */
/* @SymbolSize: 708 */
/* @PostProc: swap_instructions --offset1 0x100 --offset2 0x108 */
/* @PostProc: swap_instructions --offset1 0x104 --offset2 0x10c */
/* @PostProc: swap_instructions --offset1 0x110 --offset2 0x114 */
/* @PostProc: swap_instructions --offset1 0x114 --offset2 0x118 */
/* @PostProc: swap_instructions --offset1 0x160 --offset2 0x164 */
/* @PostProc: patch_insn_reg_field 0xf0:rt:a2:t9 0xf4:rs:a2:t9 0xf4:rt:a2:t9 0xf8:rt:a0:v1 0xfc:rs:a2:t9 0xfc:rt:a0:v1 0x108:rt:t0:t1 0x10c:rs:t0:t1 0x10c:rt:t0:t1 0x128:rd:t9:t0 0x12c:rd:t1:t2 0x12c:rs:t9:t0 0x12c:rt:t0:t1 0x130:rd:t2:t3 0x130:rs:t1:t2 0x134:rs:a2:t9 0x134:rt:a0:v1 0x138:rs:t2:t3 0x138:rt:t3:t4 0x140:rt:t3:t4 0x148:rd:t4:t5 0x14c:rd:t5:t6 0x14c:rs:t4:t5 0x14c:rt:t0:t1 0x150:rd:t6:t7 0x150:rs:t5:t6 0x154:rs:a2:t9 0x154:rt:a0:v1 0x158:rs:t6:t7 0x158:rt:t7:t0 0x15c:rt:t5:t7 0x160:rt:t7:t0 0x164:rs:t5:t7 0x164:rt:t5:t7 0x16c:rd:t9:t2 0x170:rd:t1:t3 0x170:rs:t9:t2 0x170:rt:t0:t1 0x174:rd:t2:t4 0x174:rs:t1:t3 0x178:rs:a2:t9 0x178:rt:a0:v1 0x17c:rs:t2:t4 0x17c:rt:t3:t5 0x184:rt:t3:t5 0x18c:rd:t4:t6 0x190:rd:t6:t0 0x190:rs:t4:t6 0x190:rt:t5:t7 0x194:rd:t7:t2 0x194:rs:t6:t0 0x198:rs:a2:t9 0x198:rt:a0:v1 0x19c:rs:t7:t2 0x19c:rt:t9:t1 0x1a4:rt:t9:t1 0x1ac:rd:t0:t3 0x1b0:rd:t1:t4 0x1b0:rs:t0:t3 0x1b0:rt:t5:t7 0x1b4:rd:t2:t5 0x1b4:rs:t1:t4 0x1b8:rs:a2:t9 0x1b8:rt:a0:v1 0x1bc:rs:t2:t5 0x1bc:rt:t3:t6 0x1c0:rt:t0:t4 0x1c4:rt:t3:t6 0x1c8:rs:t0:t4 0x1c8:rt:t0:t4 0x1d0:rd:t4:t0 0x1d4:rd:t6:t2 0x1d4:rs:t4:t0 0x1d4:rt:t5:t7 0x1d8:rd:t7:t1 0x1d8:rs:t6:t2 0x1dc:rs:t7:t1 0x1dc:rt:t9:t3 0x1e4:rt:t9:t3 0x1e8:rs:t0:t4 0x1f8:rs:a2:t9 0x200:rd:t1:t9 0x200:rt:a2:t9 0x208:rt:t1:t9 0x20c:rt:t1:t9 0x214:rs:t1:t9 0x21c:rt:t2:t5 0x224:rt:t2:t5 0x22c:rt:t3:t6 0x234:rt:t3:t6 0x238:rt:t4:t0 0x240:rt:t4:t0 0x248:rt:t5:t7 0x250:rt:t5:t7 0x254:rt:t6:t2 0x25c:rt:t6:t2 0x264:rt:t7:t1 0x26c:rt:t7:t1 0x270:rt:t9:t3 0x278:rt:t9:t3 0x280:rt:t0:t4 0x288:rt:t0:t4 0x29c:rt:t1:t9 0x2a0:rt:t1:t9 */
/* Address: 0x80134A30 */
/* Size: 708 bytes (177 insns) -- MERGED with former func_80134A54. */
#include "types.h"

extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern u8  ba_string[];
extern u8  ba_string2[];
extern u8  date_voice_set3_str0[];
extern u8  date_voice_set3_str1[];
extern u8  date_voice_set3_str2[];
extern volatile u8  now_girl;
extern u8  screen_w[];
extern u8  date_serifu_stride;
extern u8  date_pattern;
extern s16 now_serifu;
extern s16 para_ri;
extern s32 now_date_spot;
extern u8 *date_impkao_tbl_9_imp;
extern u8 *date_impkao_tbl_9_kao;
extern s16 imp[];
extern s16 kao[];
extern s32 date_serifu_tbl[];
extern s32 date_serifu_kind_tbl[];
extern s32 date_sound_tbl[];

extern s32 sub_sub_step_up();
extern s32 dec_bg_cache_step(s32);
extern s32 event_log_add(s32, s32);
extern s32 strcpy(u8 *, u8 *);

void date_planetarium_voice_set(void)
{
    u8 *p1;
    u8 *p2;
    s32 c3;
    u8 gv;
    u32 mode;

    ss_serifu = date_serifu_tbl[11];
    ss_serifu_kind = date_serifu_kind_tbl[11];
    sss_sound = date_sound_tbl[11];
    strcpy(ba_string, date_voice_set3_str0);
    date_serifu_stride = 7;
    if (!(screen_w[now_girl * 56 + 474] & 1)) {
        date_pattern = 0;
        strcpy(ba_string2, date_voice_set3_str1);
    } else {
        date_pattern = 1;
        strcpy(ba_string2, date_voice_set3_str2);
        if (now_girl == 10) {
            now_serifu = 1;
        }
    }
    gv = now_girl;
    mode = date_pattern;
    c3 = mode * 3;
    p1 = date_impkao_tbl_9_imp;
    now_date_spot = 10;
    imp[0] = (p1 + gv * 6U)[c3 + 0];
    imp[1] = (p1 + (u32)gv * 6U)[c3 + 1];
    imp[2] = (p1 + (gv | 0U) * 6U)[c3 + 2];
    p2 = date_impkao_tbl_9_kao;
    kao[0] = (p2 + (gv ^ 0U) * 6U)[c3 + 0];
    kao[1] = (p2 + (gv * 1U) * 6U)[c3 + 1];
    kao[2] = (p2 + (gv >> 0) * 6U)[c3 + 2];
    if (para_ri < 120 && mode != 0) {
        switch ((u32)gv) {
        case 0:
        case 3:
            imp[0] = 4;
            kao[0] = 4;
            break;
        case 2:
        case 6:
        case 8:
            imp[0] = 4;
            kao[0] = 3;
            break;
        case 1:
        case 4:
        case 5:
        case 9:
            kao[0] = 3;
            imp[0] = 3;
            break;
        case 7:
            kao[0] = 3;
            imp[0] = 3;
            break;
        case 10:
            kao[2] = 1;
            imp[2] = 1;
            break;
        }
    }
    dec_bg_cache_step(17894);
    event_log_add(1, (u8)now_date_spot);
    sub_sub_step_up();
}
