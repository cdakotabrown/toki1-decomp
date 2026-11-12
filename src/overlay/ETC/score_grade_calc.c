/* @SymbolStart: 0x80146D38 */
/* @SymbolSize: 1056 */
/* @PostProc: patch_stack_complete --old 64 --new 88 */
/* @RodataBase: 0x8014D7EC */
/* @PostProc: swap_instructions --offset1 0x380 --offset2 0x384 */
/* @PostProc: swap_instructions --offset1 0x41c --offset2 0x420 */
/* @PostProc: swap_instructions --offset1 0x420 --offset2 0x424 */
/* @PostProc: swap_instructions --offset1 0x378 --offset2 0x37c */
/* @PostProc: remap_stack_slots --from 60 --to 68 --from 52 --to 60 --from 44 --to 72 */
/* @PostProc: patch_insn_reg_field 0xb8:rt:v0:s0 0xbc:rs:v0:s0 0xbc:rt:v0:s0 0xc4:rt:v0:s0 0xc8:rt:v0:s0 0x234:rt:t6:t7 0x244:rs:t6:t7 0x244:rt:t7:t8 0x248:rs:t7:t8 0x2c0:rt:t0:a3 0x2d0:rs:t8:t9 0x2e0:rs:t9:t0 0x2f8:rs:t0:a3 0x2f8:rt:t0:a3 0x304:rt:t0:a3 0x32c:rt:t4:a3 0x330:rs:t4:a3 0x330:rt:t4:a3 0x33c:rt:t6:t5 0x340:rt:t7:t6 0x344:rd:t5:t4 0x344:rt:t4:a3 0x348:rt:t5:t4 0x34c:rd:t8:t7 0x34c:rs:t7:t6 0x34c:rt:t6:t5 0x350:rt:t8:t7 0x364:rt:t9:a3 0x368:rs:t9:a3 0x368:rt:t9:a3 0x36c:rt:a1:t8 0x370:rd:t0:t9 0x370:rt:t9:a3 0x374:rd:t1:t0 0x374:rt:t0:t9 0x378:rs:t1:t0 0x378:rt:t2:t1 0x37c:rs:a1:t8 0x37c:rt:a1:t8 0x388:rs:t2:t8 0x388:rt:a1:t1 0x398:rt:t3:a3 0x39c:rs:t3:a3 0x39c:rt:t3:a3 0x3a0:rt:t5:t4 0x3a4:rd:t4:t3 0x3a4:rt:t3:a3 0x3a8:rt:t4:t3 0x40c:rs:t5:t4 0x40c:rt:t5:t4 0x414:rs:t5:t4 0x414:rt:t7:t6 0x418:rt:t7:t6 0x2c4:rd:t8:t9 0x2dc:rd:t9:t0 0x10:rt:t6:a3 0x14:rs:t6:a3 0x14:rt:t6:a3 0x20:rs:t6:a3 0x34:rt:t6:a3 */
/* Address: 0x80146D40 */
/* Size: 1048 bytes (262 instructions) */
#include "types.h"

extern void game_reset(void);
extern s32 biorhythm_sf2(s32, s32, s32, s32);
extern void printf();
extern s32 rand(void);
extern s32 get_etc_hosei(void);
extern s32 etc_month_record_check(s32);

extern u8  now_youbi;
extern u8  byouki_kind;
extern s16 para_bun;
extern s16 para_ri;
extern s16 para_gei;
extern s16 para_zat;
extern s32 work_fuku[];
extern s32 siken_total;
extern u8  etc_score_parts_fmt[];
extern u8  etc_score_today_fmt[];
extern u8  etc_score_total_fmt[];

s32 score_grade_calc(void)
{
    s32 result;
    s32 s0;
    s32 a3;
    s32 v0;
    s32 v1;
    s32 *p;

    result = 0;
    s0 = 0;

    switch ((u32)now_youbi) {
    case 0:
        result = para_bun;
        a3 = biorhythm_sf2(0, 0, 1, 16);
        break;
    case 1:
        result = para_ri;
        a3 = biorhythm_sf2(0, 0, 1, 16);
        break;
    case 2:
        result = (para_ri * 7) / 10;
        s0 = (para_zat * 3) / 10;
        if (s0 >= 39) {
            s0 = 38;
        }
        a3 = biorhythm_sf2(0, 1, 1, 16);
        break;
    case 3:
        s0 = (para_bun * 3) / 10;
        result = (para_zat * 7) / 10;
        a3 = biorhythm_sf2(0, 0, 1, 16);
        if (s0 >= 39) {
            s0 = 38;
        }
        break;
    case 4:
        {
            int d = 10;
            result = (para_gei * 8) / d;
            s0 = (para_zat * 2) / d;
        }
        a3 = biorhythm_sf2(0, 1, 0, 16);
        if (s0 >= 26) {
            s0 = 25;
        }
        break;
    default:
        game_reset();
        break;
    }

    v0 = result + s0 + a3 - 8;
    v1 = v0;
    if (byouki_kind & 2) {
        v1 = v0 / 2;
    }
    printf(etc_score_parts_fmt, result, s0, a3, v1);
    if (v1 < 0) {
        v1 = 1;
    }
    v1 = get_etc_hosei() + v1;
    s0 = etc_month_record_check(15);
    v0 = rand();
    v1 = (s0 * (v0 % 10)) + v1;

    if (v1 < 128) {
        p = &work_fuku[now_youbi];
        if (1) {
            p[10] = (v1 * 100) / 128;
        }
    } else {
        v0 = rand();
        p = &work_fuku[now_youbi];
        p[10] = 100 - (v0 & 3);
    }

    printf(etc_score_today_fmt, p[10]);

    p = &work_fuku[now_youbi];
    siken_total = siken_total + p[10];
    printf(etc_score_total_fmt, siken_total);

    p = &work_fuku[now_youbi];
    v1 = p[10];
    if (v1 < 40) {
        v1 = 0;
    } else if (v1 < 60) {
        v1 = 1;
    } else if (v1 < 80) {
        v1 = 2;
    } else if (v1 < 95) {
        v1 = 3;
    } else {
        v1 = 4;
    }

    if (v1 == 0) {
        work_fuku[9] = work_fuku[9] + 1;
    }
    p[4] = v1;
    return v1;
}
