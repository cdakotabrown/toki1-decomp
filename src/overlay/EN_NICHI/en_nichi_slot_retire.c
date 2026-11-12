/* @SymbolStart: 0x80133064 */
/* Address: 0x80133080, Size: 460 bytes (115 insns) */
/* @SymbolSize: 488 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x50 */
/* @PostProc: patch_insn_reg_field 0x4c:rt:a2:a0 0x54:rs:a2:a0 0x54:rt:a2:a0 0x44:rt:t0:a3 0x48:rt:a3:a1 0x50:rt:v1:v0 0x58:rs:a3:a1 0x58:rt:a3:a1 0x5c:rs:v1:v0 0x5c:rt:v1:v0 0x60:rs:t0:a3 0x60:rt:t0:a3 0x64:rt:a1:a2 0x68:rs:v1:v0 0x68:rt:a0:v1 0x70:rs:a0:v1 0x78:rs:a0:v1 0x78:rt:v0:t0 0x7c:rs:a1:a2 0x7c:rt:v0:t0 0x80:rs:v1:v0 0x80:rt:v0:t0 0x88:rs:a2:a0 0x88:rt:a2:a0 0x8c:rs:a3:a1 0x8c:rt:a3:a1 0x90:rs:v1:v0 0x90:rt:v1:v0 0x94:rs:v1:v0 0x94:rt:t0:a3 0xa0:rt:a3:a1 0xa8:rt:t1:t2 0xac:rs:t1:t2 0xac:rt:t1:t2 0xb0:rt:a2:a0 0xb8:rt:t2:t3 0xbc:rs:t2:t3 0xbc:rt:t2:t3 0xc8:rt:t1:t2 0xdc:rt:t2:t3 0xe8:rt:t3:t4 0xf0:rt:t3:t4 0xf4:rt:t4:t5 0xf8:rs:t4:t5 0xf8:rt:t4:t5 0xfc:rt:t5:t6 0x100:rs:t4:t5 0x108:rt:t6:t8 0x10c:rs:t5:t6 0x10c:rt:t5:t6 0x114:rs:t6:t8 0x114:rt:t6:t8 0x118:rt:t5:t6 0x120:rt:t6:t8 0x13c:rt:t8:t7 0x140:rs:t8:t7 0x140:rt:t8:t7 0x144:rt:t9:t0 0x148:rs:t8:t7 0x148:rt:t7:t9 0x14c:rs:t7:t9 0x15c:rs:t9:t0 0x15c:rt:t9:t0 0x164:rs:t9:t0 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void en_nichi_marker_add(s16, s16);
extern void en_nichi_cmd_send_once(s32);

extern s32 en_nichi_obj_wk[];
extern s32 en_nichi_kind_attr_tbl[];
extern s32 en_nichi_rare_cnt;
extern s32 en_nichi_get_cnt;
extern s32 en_nichi_action_step;
extern s32 en_nichi_action_timer;
extern s32 en_nichi_poi_wear;
extern s16 mouse_x;
extern s16 mouse_y;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern CLASS_W class_w[];

void en_nichi_slot_retire(void)
{
    s32 *p;
    s32 cnt4;
    s32 cntOther;
    s32 v1;
    s32 low;
    s32 i;

    en_nichi_action_timer++;
    if (en_nichi_poi_wear >= 80 && en_nichi_action_timer >= 30) {
        cnt4 = en_nichi_rare_cnt;
        cntOther = en_nichi_get_cnt;
        p = en_nichi_obj_wk;
        do {
            v1 = *p;
            if (v1 & 0x80) {
                low = v1 & 0x7F;
                *p = low;
                if (low == 4) {
                    cnt4 -= 1;
                } else {
                    cntOther -= 1;
                }
            }
            p += 12;
        } while (p != en_nichi_kind_attr_tbl);
        en_nichi_get_cnt = cntOther;
        en_nichi_rare_cnt = cnt4;

        mouse_x = class_w[152].x.half[1];
        mouse_y = class_w[152].y.half[1];
        en_nichi_marker_add(cursor_cw_x, cursor_cw_y);
        en_nichi_cmd_send_once(1282);
        en_nichi_action_step = 3;
    }

    if (en_nichi_action_timer >= 60) {
        mouse_x = class_w[152].x.half[1];
        mouse_y = class_w[152].y.half[1];
        en_nichi_action_timer = 0;
        en_nichi_action_step = 0;
        en_nichi_cmd_send_once(1293);

        for (i = 0; i < 10; i++) {
            if (en_nichi_obj_wk[i * 12] & 0x80) {
                en_nichi_obj_wk[i * 12] = 0;
            }
        }
    }
}
