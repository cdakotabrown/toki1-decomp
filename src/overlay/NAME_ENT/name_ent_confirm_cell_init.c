/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: patch_insn_reg_field 0x20:rt:t2:a1 0x24:rt:t3:a0 0x28:rt:t4:v1 0x34:rs:t2:v1 0x34:rt:t2:v1 0x38:rs:t3:a0 0x38:rt:t3:a0 0x3c:rs:t4:a1 0x3c:rt:t4:a1 0x48:rt:t1:t4 0x4c:rt:t0:t3 0x50:rt:a3:t2 0x54:rt:a2:t1 0x58:rt:a1:t0 0x5c:rt:a0:a3 0x60:rt:v1:a2 0x68:rt:v1:a2 0x6c:rt:a0:a3 0x74:rt:a1:t0 0x78:rt:t2:v1 0x7c:rt:t3:a0 0x80:rt:t4:a1 0x84:rt:a2:t1 0x88:rt:a3:t2 0x8c:rt:t0:t3 0x94:rt:t1:t4 */
/* name_ent_confirm_cell_init - NAME_ENT overlay, Address: 0x80137F94, Size: 224 bytes */
#include "types.h"

extern u32 img_set_adr;
extern u32 anim_set_adr;
extern u32 tpage_set_adr;
extern u8 class_w[];

extern s16 cal_cw_frame;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s16 cw97_frame;
extern s16 cw97_x;
extern s16 cw97_y;
extern u8 cw97_prio;

extern void class_clear_f();

void name_ent_confirm_cell_init(void)
{
    u8 *slot;
    u8 *endSlot;
    u32 va;
    u32 vb;
    u32 vc;

    class_clear_f();

    va = img_set_adr;
    vb = anim_set_adr;
    vc = tpage_set_adr;

    slot = class_w;
    endSlot = class_w + 136;

    do {
        slot += 68;
        slot[6461] = 2;
        slot[6462] = 64;
        *(u32 *)&slot[6516] = 0;
        slot[6463] = 132;
        *(u32 *)&slot[6472] = va;
        *(u32 *)&slot[6476] = vb;
        *(u32 *)&slot[6512] = vc;
        slot[6464] = 8;
        slot[6465] = 129;
        slot[6527] = 16;
        *(s16 *)&slot[6498] = 400;
    } while (slot != endSlot);

    cal_cw_frame = 2;
    cal_cw_x = 0;
    cal_cw_y = 0;
    cw97_frame = 6;
    cw97_x = -10;
    cw97_y = 88;
    cw97_prio = 3;
}
