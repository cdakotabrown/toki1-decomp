/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: remap_stack_slots --from 44 --to 40 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x6C */
/* @PostProc: swap_instructions --offset1 0xB8 --offset2 0xC0 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0xA0 */
/* @PostProc: patch_insn_reg_field 0x54:rt:v1:v0 0x6c:rt:t3:t4 0x74:rs:v1:v0 0x74:rt:v1:t1 0x7c:rs:v1:t1 0x7c:rt:t1:t2 0x80:rs:t1:t2 0x84:rs:t1:t2 0x88:rs:t1:t2 0x8c:rt:t2:t3 0x90:rs:t2:t3 0x94:rs:t3:t4 0x94:rt:t3:t4 0x9c:rt:t3:t4 0xa8:rd:t4:t5 0xb0:rd:t4:t5 0xb4:rd:t4:t5 0xb4:rs:t4:t5 0xb8:rt:t4:t5 0xc8:rt:v1:v0 0xcc:rt:t5:t8 0xd0:rs:v1:v0 0xd4:rt:t7:t9 0xd8:rs:t5:t8 0xd8:rt:t5:t8 0xe0:rs:t5:t8 0xe0:rt:t8:t7 0xe4:rt:t8:t7 0xec:rt:v1:v0 0xf4:rs:v1:v0 0xfc:rs:t7:t9 0xfc:rt:t7:t9 0x104:rs:t7:t9 0x104:rt:t9:t6 0x108:rt:t9:t6 */
/* Address: 0x801359E8 */
/* Size: 276 bytes (69 instructions) */
#include "types.h"

struct Elem { u8 b[5]; };
struct Buf { struct Elem e[7]; };

extern struct Buf shougatu_omikuji_set_tbl;
extern u8 oname[];
extern s32 strcpy();
extern void param_clamp();

extern s32 frame_counter;
extern u8 now_girl;
extern u8 shougatu_omikuji_no;
extern s16 para_str;

void shougatu_omikuji_set(void)
{
    struct Buf local;
    struct Elem *ep;
    s32 idx;
    s32 st;

    local = shougatu_omikuji_set_tbl;

    idx = frame_counter & 7;
    if (idx != 0) {
        idx = (idx - 1) & 0xFF;
    }
    if (now_girl == 3 && idx == 3) {
        idx = 4;
    }

    ep = &local.e[idx];
    shougatu_omikuji_no = idx;
    strcpy(oname, ep, &local);
    st = shougatu_omikuji_no;
    if (st == 0) {
        para_str -= 5;
    }
    shougatu_omikuji_no = st;
    if (st == 4) {
        para_str += 5;
    }
    param_clamp();
}
