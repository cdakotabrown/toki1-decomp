/* @SymbolSize: 500 */
/* @PostProc: patch_stack_complete --old 48 --new 64 */
/* @PostProc: remap_stack_slots --map '36:52,40:60' */
/* tt_mode1_main - TT overlay, Address: 0x801365C4, Size: 500 bytes (125 insns) */
#include "types.h"

extern u8 *tt_boss_wk;
extern u8 *tt_step_wk;
extern u8 *tt_player_wk;
extern u8 *tt_stage_wk;

extern void device_cmd_send(s32 id);
extern void change_trans_pointer(u32 addr);
extern void tt_hit_scan(void);
extern void tt_match_init(void);
extern void tt_slide_in(void);
extern void tt_seq_wait_step(void);
extern void tt_orbit_step(void);
extern void tt_serve_intro(void);
extern void tt_camera_step(void);
extern void tt_pulse_disp(void);
extern void tt_sprite_pool_clear(void);

void tt_mode1_main(void)
{
    u8 *s0;
    u8 *hdr;
    u8 *blk;
    s32 state;
    s32 v0;

    s0 = tt_boss_wk;
    state = *(u16 *)(s0 + 4);
    hdr = tt_step_wk;
    blk = tt_player_wk;

    switch (state) {
    case 0:
        tt_sprite_pool_clear();
        if (*(u16 *)(hdr + 4) == 0) {
            device_cmd_send(177);
            change_trans_pointer(0x8019A000);
            device_cmd_send(18);
            device_cmd_send(35);
            device_cmd_send(46);
            device_cmd_send(33);
            device_cmd_send(4);
            device_cmd_send(512);
        }
        tt_match_init();
        *(s16 *)(s0 + 4) = 32;
        break;

    case 32:
        tt_slide_in();
        tt_camera_step();
        break;

    case 48:
        tt_seq_wait_step();
        tt_camera_step();
        break;

    case 64:
        v0 = *(s16 *)(tt_boss_wk + 780);
        if (v0 < 32) {
            *(s16 *)(s0 + 6) = 2;
        } else if (v0 < 64) {
            *(s16 *)(s0 + 6) = 1;
        } else {
            *(s16 *)(s0 + 6) = 0;
        }
        tt_orbit_step();
        tt_camera_step();
        if (*(u16 *)(s0 + 8) == 0) {
            tt_pulse_disp();
        }
        if (*(s16 *)(s0 + 780) < 0) {
            *(s16 *)(s0 + 4) = 128;
            *(s16 *)(s0 + 10) = 0;
            *(s16 *)(s0 + 8) = 0;
            *(s16 *)(s0 + 6) = 0;
            s0[15] = 0;
            s0[14] = 0;
            s0[13] = 0;
        }
        break;

    case 128:
        device_cmd_send(193);
        blk[82] = 1;
        tt_serve_intro();
        break;

    case 160:
        s0[0] = 0;
        hdr[18] = 0;
        *(s16 *)(tt_stage_wk + 30) = 1;
        *(s16 *)(hdr + 2) = 160;
        break;
    }

    tt_hit_scan();
}
