/* @SymbolSize: 296 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: patch_stack_complete --old 48 --new 64 */
/* @PostProc: remap_stack_slots --from 36 --to 56 --from 32 --to 52 */
/* @PostProc: patch_insn_reg_field 0xbc:rd:v1:t5 0xc0:rd:a0:t6 0xc4:rt:a1:t4 0xc8:rs:v1:t5 0xc8:rt:v0:at 0xcc:rs:v1:t5 0xcc:rt:v1:t5 0xd0:rs:a0:t6 0xd0:rt:v0:at 0xd4:rs:v1:t5 0xd4:rt:v0:at 0xd8:rs:a0:t6 0xd8:rt:a0:t6 0xdc:rs:a0:t6 0xdc:rt:v0:at 0xe0:rs:v1:t5 0xe0:rt:v0:at 0xe4:rs:v1:t5 0xe4:rt:a1:t4 0xe8:rs:a0:t6 0xe8:rt:v0:at 0xec:rt:t3:t7 0xf0:rs:t3:t7 0xf0:rt:t3:t7 0xf4:rt:t4:t8 0xf8:rt:t5:t9 0x104:rt:t3:t7 0x10c:rt:t4:t8 0x114:rt:t5:t9 */
/* tt_widget_pair_init - TT overlay, Address: 0x80144084, Size: 296 bytes (74 insns) */
#include "types.h"

extern s32 tt_cmd_once();
extern s32 anim_cel_step();

extern s32 tt_scroll_wk;
extern s32 tt_stage_wk;
extern s32 tt_player_wk;
extern s32 tt_cel_buf;
extern u8 tt_anim_tbl[];
extern u8 tt_replay0_dat[];

void tt_widget_pair_init(void)
{
    u8 *rA;
    u8 *rB;
    u8 *pa;
    u8 *pb;
    u8 *ra_ptr;
    u8 *rb_ptr;
    u8 *end;
    s32 at;

    pa = (u8 *)tt_scroll_wk;
    pb = (u8 *)tt_stage_wk;
    rA = (u8 *)tt_player_wk;
    rB = (u8 *)tt_cel_buf;

    tt_cmd_once(1552);
    tt_cmd_once(1543);

    if ((u32)*(u16 *)(pb + 2) < 1025U) {
        *(s32 *)(pa + 4) = 0x8000;
        *(s32 *)(pa + 0) = 0x8000;
    }
    *(u8 *)(rA + 82) = 1;
    *(u8 *)(rA + 86) = 1;
    *(u8 *)(rA + 83) = 0;
    *(s32 *)(rA + 40) = 0;
    *(s32 *)(rA + 44) = 0x00010000;
    *(s32 *)(rA + 56) = (s32)tt_anim_tbl;
    *(u8 *)(rA + 60) = 0;
    *(u8 *)(rA + 61) = 1;
    *(u8 *)(rA + 62) = 0;
    *(u8 *)(rA + 64) = 128;
    anim_cel_step(rA + 56);

    ra_ptr = rA;
    rb_ptr = rB;
    end = rA + 36;
    do {
        at = *(s32 *)(ra_ptr + 20);
        ra_ptr += 12;
        *(s32 *)(rb_ptr + 20) = at;
        at = *(s32 *)(ra_ptr + 12);
        rb_ptr += 12;
        *(s32 *)(rb_ptr + 12) = at;
        at = *(s32 *)(ra_ptr + 16);
        *(s32 *)(rb_ptr + 16) = at;
    } while (ra_ptr != end);

    *(s32 *)(rB + 40) = 0;
    *(s32 *)(rB + 44) = 0;
    *(s32 *)(rB + 56) = (s32)((&tt_replay0_dat[4092]) + 4);
    *(u8 *)(rB + 60) = 0;
    *(u8 *)(rB + 61) = 2;
    *(u8 *)(rB + 62) = 0;
    *(u8 *)(rB + 64) = 128;
    anim_cel_step(rB + 56);
    *(s32 *)(rB + 152) = 0;
}
