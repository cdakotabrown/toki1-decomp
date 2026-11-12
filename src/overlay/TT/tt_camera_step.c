/* @SymbolStart: 0x80136244 */
/* @SymbolSize: 680 */
/* Address: 0x80136244 (cross-prologue head absorbed), Size: 680 bytes */
/* @PostProc: remap_stack_slots --from 44 --to 40 */
/* @PostProc: patch_insn_reg_field 0x238:rd:v0:t1 0x23c:rs:v0:t1 0x244:rd:t1:t2 0x248:rt:v0:t1 0x24c:rs:t1:t2 0x250:rs:t1:t2 0x258:rt:t1:t2 0x27c:rt:t3:t4 0x280:rt:t2:t3 0x288:rt:t2:t3 0x28c:rt:t3:t4 0x290:rt:v0:t5 0x298:rs:v0:t5 0x298:rt:t4:t6 0x29c:rt:t4:t6 0x2a4:rs:t4:t6 */
#include "types.h"
#include "tokimeki/tt_box.h"

extern u8 tt_camera_seq_tbl[];
extern u8 tt_camera2_seq_tbl[];
extern s16 tt_court_ofs_a;
extern s16 tt_court_ofs_b;

extern u8 *tt_boss_wk;
extern TT_BOX tt_hit_box_tbl[];

extern void tt_seq_step(u8 *p, s32 n);
extern void tt_court_layout(s32 a, s32 b, s32 c);
extern void tt_court_id_set(s32 a);

void tt_camera_step(void)
{
    u8 *s;
    s32 v;
    s32 p;
    s32 r;
    s32 t;
    s32 m;
    s32 d;

    s = tt_boss_wk;
    switch (s[62]) {
    case 0:
        *(s32 *)(s + 560) = 0;
        break;
    case 1:
        *(s32 *)(s + 560) = (s32)((u8 *)&tt_hit_box_tbl[11].x);
        break;
    case 2:
        *(s32 *)(s + 560) = (s32)((u8 *)&tt_hit_box_tbl[12].x);
        break;
    case 3:
        *(s32 *)(s + 560) = (s32)((u8 *)&tt_hit_box_tbl[13].x);
        break;
    case 31:
        *(s32 *)(s + 560) = (s32)((u8 *)&tt_hit_box_tbl[14].x);
        break;
    case 30:
        *(s32 *)(s + 560) = (s32)((u8 *)&tt_hit_box_tbl[15].x);
        break;
    case 29:
        *(s32 *)(s + 560) = (s32)((u8 *)&tt_hit_box_tbl[16].x);
        break;
    default:
        *(s32 *)(s + 560) = (s32)((u8 *)&tt_hit_box_tbl[10].x);
        break;
    }

    tt_seq_step(tt_camera_seq_tbl, 4);
    tt_seq_step(tt_camera2_seq_tbl, 5);

    switch (*(u16 *)(s + 6)) {
    case 0:
        tt_court_ofs_a = 15376;
        tt_court_ofs_b = 15376;
        break;
    case 1:
        tt_court_ofs_a = 15440;
        tt_court_ofs_b = 15440;
        break;
    case 2:
        tt_court_ofs_a = 15504;
        tt_court_ofs_b = 15504;
        break;
    }

    tt_court_layout(*(s16 *)(s + 948), *(s16 *)(s + 950), *(u16 *)(s + 936));

    v = *(u16 *)(s + 938);
    p = (s16)v;
    *(s16 *)(s + 936) = (s16)(*(u16 *)(s + 936) + p / 256);
    if (p < 0) {
        if (p < -2048) {
            *(s16 *)(s + 938) = (s16)(v + 128);
        } else {
            *(u16 *)(s + 938) = 0xF800;
        }
    } else {
        if (p >= 2049) {
            *(s16 *)(s + 938) = (s16)(v - 128);
        } else {
            *(s16 *)(s + 938) = 2048;
        }
    }

    p = *(s16 *)(s + 938);
    if (p < 0) {
        v = -p;
    } else {
        v = p;
    }
    if (v < 2049) {
        return;
    }

    d = *(u16 *)(s + 940) - (p / 256) / 4;
    r = d & 0xFFFF;
    t = (s16)r;
    *(s16 *)(s + 940) = (s16)d;
    m = t;
    if (t < 0) {
        v = -t;
    } else {
        v = m;
    }
    if (v < 2) {
        *(s16 *)(s + 940) = 0;
    } else if (m < 2048) {
        *(s16 *)(s + 940) = (s16)(r - 2);
    } else {
        *(s16 *)(s + 940) = (s16)(r + 2);
    }

    m = *(u16 *)(s + 940) & 0xFFF;
    *(s16 *)(s + 940) = (s16)m;
    tt_court_id_set(m & 0xFFFF);
}
