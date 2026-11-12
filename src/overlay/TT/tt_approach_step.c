/* @SymbolStart: 0x80140910 */
/* @SymbolSize: 924 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=a0:152 */
/* @PostProc: patch_stack_complete --old 88 --new 104 */
/* @PostProc: remap_stack_slots --map '28:56,40:68,44:72,76:92,80:100,84:96' */
/* @PostProc: patch_insn_reg_field 0x12c:rt:t9:t1 0x134:rs:t9:t1 0x134:rt:t0:t2 0x138:rt:t0:t2 0x140:rt:t1:t9 0x148:rs:t1:t9 0x148:rt:t2:t0 0x150:rt:t2:t0 0x2b0:rd:t9:t0 0x2b4:rt:t0:t9 0x2b8:rd:t9:t0 0x2b8:rs:t9:t0 0x2bc:rd:t9:t0 0x2bc:rt:t9:t0 0x2dc:rd:t2:t3 0x2e0:rt:t3:t2 0x2e4:rd:t2:t3 0x2e4:rs:t2:t3 0x2ec:rd:t2:t3 0x2ec:rt:t2:t3 0x310:rt:t0:t4 0x330:rt:t9:t3 0x334:rt:t3:t9 0x338:rt:t4:t0 0x33c:rd:a0:a1 0x33c:rs:t9:t3 0x33c:rt:t0:t4 0x340:rd:t1:t5 0x340:rt:a0:a1 0x344:rd:t2:t6 0x344:rt:t1:t5 0x348:rd:a1:a0 0x348:rs:t3:t9 0x348:rt:t4:t0 0x34c:rd:t5:t1 0x34c:rt:a1:a0 0x350:rs:t2:t6 0x358:rd:t6:t2 0x358:rt:t5:t1 0x35c:rs:t2:t6 0x364:rs:t6:t2 0x36c:rs:t6:t2 */
/* tt_approach_step - TT overlay, Address: 0x80140918 */
#include "types.h"

extern u8 tt_anim_tbl[];
extern u8 *tt_game_wk;
extern u8 *tt_player_wk;
extern u8 *tt_boss_wk;
extern volatile s32 tt_approach_ang_wk;

extern void tt_cmd_once(s32 id);
extern s32 tt_angle_between(void *a, void *b);
extern s32 rcos(s32 angle);
extern s32 rsin(s32 angle);

void tt_approach_step(void)
{
    u8 *p;
    u8 *q;
    u8 *r;
    s32 v;
    s32 x;
    s32 y;
    s32 tmp[9];

    p = tt_boss_wk;
    q = tt_game_wk;
    r = tt_player_wk;
    switch ((u32)*(u16 *)(p + 8)) {
    case 0:
        tt_cmd_once(1296);
        *(u8 **)(p + 516) = tt_anim_tbl;
        *(u8 *)(p + 520) = 0;
        *(u8 *)(p + 521) = 4;
        *(u8 *)(p + 522) = 65;
        *(u8 *)(p + 524) = 96;
        *(u8 *)(p + 527) = 15;
        *(u8 *)(p + 532) = 1;
        *(u8 *)(p + 807) = 1;
        *(s16 *)(p + 726) = 10;
        *(s16 *)(p + 630) = 0;
        *(s16 *)(p + 678) = 8;
        *(u8 *)(p + 526) = 0;
        *(s16 *)(p + 528) = 0;
        *(s16 *)(p + 530) = 0;
        *(s32 *)(p + 1004) = 0x00080000;
        *(u8 *)(p + 836) = 0;
        *(u16 *)(p + 8) = 64;
        break;
    case 64:
        if ((*(u16 *)(tt_game_wk + 8) & 3) != 0) {
            *(u8 *)(p + 807) = 1;
        } else {
            *(u8 *)(p + 807) = 0;
        }
        *(s16 *)(p + 530) = (s16)(*(s16 *)(p + 530) + 32);
        *(u8 *)(p + 526) = 0;
        *(s16 *)(p + 528) = (s16)(*(s16 *)(p + 528) + 32);
        *(s32 *)(p + 1004) = *(s32 *)(p + 1004) + 0x8000;
        *(s16 *)(p + 678) = (s16)(*(s32 *)(p + 1004) / 65536);
        if (*(s16 *)(p + 530) >= 4096) {
            *(u16 *)(p + 8) = 80;
            *(u8 *)(p + 532) = 0;
            *(u16 *)(p + 872) = 0;
        }
        break;
    case 80:
        if ((*(u16 *)(tt_game_wk + 8) & 3) != 0) {
            *(u8 *)(p + 807) = 1;
        } else {
            *(u8 *)(p + 807) = 0;
        }
        v = *(u16 *)(p + 872);
        *(u8 *)(p + 526) = 0;
        if ((u32)v >= 60U) {
            *(u16 *)(p + 8) = 81;
        } else {
            *(u16 *)(p + 872) = (s16)(v + 1);
        }
        break;
    case 81:
        tt_cmd_once(1548);
        if ((*(u16 *)(q + 8) & 3) != 0) {
            *(u8 *)(p + 807) = 1;
        } else {
            *(u8 *)(p + 807) = 0;
        }
        *(u8 *)(p + 526) = 0;
        tmp[3] = *(s16 *)(p + 20) << 16;
        tmp[4] = (*(s16 *)(p + 22) + 72) << 16;
        tt_approach_ang_wk = tt_angle_between(tmp, r + 20);
        *(s32 *)(p + 1004) = *(s16 *)(p + 630) << 16;
        *(s32 *)(p + 1008) = *(s16 *)(p + 678) << 16;
        *(u16 *)(p + 8) = 82;
        break;
    case 82:
        if ((*(u16 *)(tt_game_wk + 8) & 3) != 0) {
            *(u8 *)(p + 807) = 1;
        } else {
            *(u8 *)(p + 807) = 0;
        }
        *(u8 *)(p + 526) = 0;
        *(s32 *)(p + 1004) =
            *(s32 *)(p + 1004) + rcos(tt_approach_ang_wk) * 28;
        *(s32 *)(p + 1008) =
            *(s32 *)(p + 1008) + rsin(tt_approach_ang_wk) * 28;
        *(s16 *)(p + 630) = (s16)(*(s32 *)(p + 1004) / 65536);
        *(s16 *)(p + 678) = (s16)(*(s32 *)(p + 1008) / 65536);
        x = (s16)(*(s16 *)(p + 22) + *(s16 *)(p + 678));
        y = (s16)(*(s16 *)(p + 20) + *(s16 *)(p + 630));
        if (x >= 313 || x < -72 || y < -200 || y >= 201) {
            *(u8 *)(p + 807) = 0;
            *(u16 *)(p + 8) = 160;
        }
        break;
    case 160:
        *(u8 *)(p + 836) = 1;
        *(s32 *)(p + 1000) = 0;
        *(s16 *)(p + 6) = 0;
        *(u16 *)(p + 8) = 0;
        *(s32 *)(p + 996) = *(s32 *)(p + 32);
        break;
    }
}
