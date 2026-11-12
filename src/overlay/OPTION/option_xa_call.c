/* @PostProc: patch_insn_reg_field 0x24:rt:v0:a3 0x28:rs:v0:a3 0x28:rt:v0:a3 0x30:rt:v1:v0 0x34:rs:v0:a3 0x34:rt:v0:a3 0x38:rs:v0:a3 0x3c:rd:v0:a3 0x40:rs:v1:v0 0x40:rt:v1:v0 0x48:rs:v1:v0 0x50:rs:v1:v0 0x54:rs:v0:a3 0x54:rt:v0:a3 0x58:rs:v0:a3 0x5c:rd:v0:a3 0x64:rt:t3:t1 0x68:rs:v0:a3 0x70:rs:v0:a3 0x70:rt:v0:a3 0x74:rs:v0:a3 0x74:rt:t1:t0 0x78:rd:v0:a3 0x78:rs:t1:t0 0x7c:rt:t0:t2 0x80:rs:t3:t1 0x80:rt:t3:t1 0x84:rs:t0:t2 0x84:rt:t4:t3 0x88:rd:t5:t4 0x88:rt:t4:t3 0x8c:rd:t6:t5 0x8c:rs:t3:t1 0x8c:rt:t5:t4 0x90:rs:t6:t5 0x90:rt:a3:v1 0x98:rs:a3:v1 0x9c:rt:v0:a3 0xa0:rt:v0:a3 0xa4:rd:v1:v0 0xa4:rs:a3:v1 0xa8:rs:v1:v0 0xac:rs:v1:v0 0xb4:rt:v0:a3 0xbc:rs:t0:t2 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x64 */
/* @SymbolSize: 200 */
/* Overlay: OPTION */
/* Address: 0x80139E5C, Size: 200 bytes (50 instructions) */
#include "types.h"

extern volatile u16 option_xa_no;
extern u32 pad_data;
extern u8 now_girl;
extern s32 **option_girl_xa_ptr;
extern void set_XAsector();
extern void device_cmd_send();

void option_xa_call(s32 arg0)
{
    s32 cursor;
    u32 pad;
    u8 sel;
    s32 *table;
    s32 *entry;
    s32 e0, e1;

    if (arg0 > 0) {
        option_xa_no = (u16)arg0;
    }
    cursor = (s32)option_xa_no;

    if (arg0 < 0) {
        cursor = (cursor + 1) & 0xFFFF;
    }

    pad = pad_data;
    if ((pad & 4) != 0) {
        cursor = (cursor + 1) & 0xFFFF;
    }

    if ((pad & 1) != 0) {
        if (cursor != 0) {
            cursor = (cursor - 1) & 0xFFFF;
        }
    }

    sel = now_girl;
    table = option_girl_xa_ptr[sel & 0xF];

    option_xa_no = (u16)cursor;
    if (table != 0) {
        entry = (s32 *)((s32)table + (cursor << 3));
        e0 = entry[0];
        e1 = entry[1];
        option_xa_no = (u16)cursor;
        set_XAsector(e0, e1, (s32)sel);
    }
    device_cmd_send(768);
}
