/* @RodataBase: 0x8014D854 */
/* @PostProc: swap_branch_operands --offsets 0x170,0x188 */
/* tt_slot_step - TT overlay, Address: 0x8014C670, Size: 460 bytes (115 insns) */
#include "types.h"

extern u8 *tt_slot_buf;

extern void tt_script_run(void);
extern void tt_curve_move(u8 *a0);
extern void tt_obj_timer(u8 *a0);
extern void tt_sway_move(u8 *a0);
extern void tt_arc_move(u8 *a0);
extern void tt_spread_move(u8 *a0);
extern void tt_orbit_move(u8 *a0);
extern void tt_rise_move(u8 *a0);
extern void tt_cross_move(u8 *a0);
extern void tt_slot_hit_init(u8 *a0);
extern void tt_slot_move(u8 *a0);
extern void tt_anim(u8 *a0, u8 *a1, u8 *a2);
extern void anim_cel_step(u8 *a0);

void tt_slot_step(void)
{
    u8 *s0;
    s32 s1;
    s32 v0;

    tt_script_run();

    s0 = tt_slot_buf;
    s1 = 0;

    do {
        if (s0[0] == 0) {
            goto next;
        }

        v0 = *(u16 *)(s0 + 4);
        switch (v0) {
        case 64:
            goto sub;
        case 112:
            goto retire;
        case 113:
            goto common;
        case 128:
            goto kill;
        }
        goto common;

sub:
        switch (*(u16 *)(s0 + 2)) {
        case 64:
            tt_curve_move(s0);
            goto common;
        case 65:
        case 66:
            tt_obj_timer(s0);
            goto common;
        case 67:
            tt_sway_move(s0);
            goto common;
        case 68:
            tt_arc_move(s0);
            goto common;
        case 69:
            tt_spread_move(s0);
            goto common;
        case 70:
            tt_orbit_move(s0);
            goto common;
        case 71:
            tt_rise_move(s0);
            goto common;
        case 72:
            tt_cross_move(s0);
            goto common;
        }
        goto common;

retire:
        tt_slot_hit_init(s0);
        *(s16 *)(s0 + 4) = (s16)113;
        *(s16 *)(s0 + 10) = 0;
        *(s16 *)(s0 + 8) = 0;
        *(s16 *)(s0 + 6) = 0;
        s0[15] = 0;
        s0[14] = 0;
        s0[13] = 0;
        goto common;

kill:
        s0[0] = 0;
        s0[81] = 0;

common:
        tt_slot_move(s0);
        v0 = *(u16 *)(s0 + 4);
        if (64 == v0) {
            tt_anim(s0 + 56, s0 + 92, s0 + 93);
            goto next;
        }
        if (113 == v0) {
            anim_cel_step(s0 + 56);
            if ((s0[64] & 1) != 0) {
                s0[0] = 0;
            }
        }

next:
        s1++;
        s0 += 120;
    } while (s1 != 64U);
}
