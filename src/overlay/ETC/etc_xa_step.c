/* @SymbolStart: 0x8013F2F0 */
/* @SymbolSize: 216 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x20:rs:v0:v1 0x2c:rs:v0:v1 0x34:rs:v0:v1 0x44:rt:t8:t6 0x48:rs:t8:t6 0x48:rt:t8:t6 0x4c:rd:t7:t8 0x50:rs:t7:t6 0x74:rt:v0:v1 0x78:rs:v0:v1 0x78:rt:v0:v1 0x80:rs:v0:v1 0x80:rt:v0:v1 0x88:rt:v0:v1 0x9c:rt:v0:v1 0xa0:rs:v0:v1 0xa0:rt:v0:v1 0xa8:rs:v0:v1 0xa8:rt:v0:v1 0xb0:rt:v0:v1 0xc4:rt:v0:v1 0xc8:rs:v0:v1 0xc8:rt:v0:v1 0xd0:rs:v0:v1 0xd0:rt:v0:v1 0xd4:rt:v0:v1 */
/* Address: 0x8013F2F8 */
/* Size: 208 bytes (52 instructions) */
#include "types.h"

extern void set_XAsector(s32, s32, s32);
extern void device_cmd_send(s32);
extern s32 get_CD_State(void);
extern u8 now_girl;
extern s32 etc_xa_phase;
extern s32 *etc_xa_tbl;

void etc_xa_step(s32 a0)
{
    s32 v1 = etc_xa_phase;

    s32 *p;

    switch (v1) {
    case 0:
        p = etc_xa_tbl + a0 * 2;
        set_XAsector(p[0], p[1], now_girl);
        device_cmd_send(768);
        etc_xa_phase = etc_xa_phase + 1;
        return;
    case 1:
        if ((get_CD_State() & 4) != 0) {
            etc_xa_phase = etc_xa_phase + 1;
        }
        return;
    case 2:
        if ((get_CD_State() & 2) != 0) {
            etc_xa_phase = etc_xa_phase + 1;
        }
        return;
    }
}
