/* @PostProc: patch_stack_complete --old 120 --new 128 */
/* @PostProc: remap_stack_slots --from 76 --to 84 --from 32 --to 40 */
/* @PostProc: patch_insn_reg_field 0x14:rt:v0:v1 0x24:rs:v0:v1 0x24:rt:v0:v1 0xa4:rd:v1:v0 0xa4:rt:v0:v1 0xb0:rt:v1:v0 0xb8:rt:v1:v0 0xc8:rs:v0:v1 */
/* Address: 0x80138A3C */
/* Size: 212 bytes (53 insns) */
#include "types.h"

extern s32 device_cmd_send(s32 a0);
extern s32 set_XAsector(s32 a0, s32 a1, s32 a2);

extern volatile u8 taiiku_kari_girl;

typedef struct { s32 w[11]; } Buf44;
extern Buf44 taiiku_kari_xa_beg;
extern Buf44 taiiku_kari_xa_end;

void taiiku_xa_start(void) {
    Buf44 buf_a;
    Buf44 buf_b;
    u8 idx;

    idx = taiiku_kari_girl;
    buf_a = taiiku_kari_xa_beg;
    buf_b = taiiku_kari_xa_end;

    set_XAsector(buf_a.w[idx], buf_b.w[idx], idx);
    device_cmd_send(768);
}
