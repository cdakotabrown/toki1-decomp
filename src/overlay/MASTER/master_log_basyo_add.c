/* @SymbolStart: 0x8013348C */
/* @SymbolSize: 500 */
/* @RodataBase: 0x8013BAF0 */
/* @PostProc: patch_insn_reg_field 0x80:rt:t1:t0 0x84:rs:t1:t0 0x84:rt:t1:t0 0x90:rd:t2:t1 0x90:rt:t1:t0 0x94:rt:t2:t1 0xe4:rt:t3:t2 0xe8:rs:t3:t2 0xe8:rt:t3:t2 0xfc:rd:t4:t3 0xfc:rt:t3:t2 0x100:rd:t5:t4 0x100:rt:t4:t3 0x104:rt:t7:t6 0x108:rs:t5:t4 0x10c:rs:t7:t6 0x10c:rt:t7:t6 0x110:rt:t6:t5 0x114:rd:t8:t7 0x114:rt:t7:t6 0x118:rd:t9:t8 0x118:rt:t8:t7 0x11c:rs:t9:t8 0x11c:rt:t6:t5 0x120:rt:t1:t9 0x124:rs:t1:t9 0x124:rt:t1:t9 0x12c:rd:t2:t0 0x12c:rt:t1:t9 0x130:rd:t3:t1 0x130:rt:t2:t0 0x138:rs:t3:t1 0x13c:rt:t4:t2 0x140:rs:t4:t2 0x140:rt:t4:t2 0x14c:rd:t5:t3 0x14c:rt:t4:t2 0x150:rd:t7:t4 0x150:rt:t5:t3 0x154:rs:t7:t4 0x158:rt:a1:t7 0x15c:rs:a1:t7 0x15c:rt:a1:t7 0x160:rt:t8:t6 0x164:rs:t8:t6 0x164:rt:t8:t6 0x168:rd:t6:t5 0x168:rt:a1:t7 0x16c:rd:t9:t8 0x16c:rt:t6:t5 0x170:rt:a2:t0 0x174:rs:t9:t8 0x174:rt:t8:t6 0x178:rs:a2:t0 0x178:rt:a2:t0 0x17c:rt:t1:t9 0x180:rs:t1:t9 0x180:rt:t1:t9 0x184:rd:t2:t1 0x184:rt:a2:t0 0x188:rd:t3:t2 0x188:rt:t2:t1 0x18c:rt:a3:t4 0x190:rs:t3:t2 0x190:rt:t1:t9 0x194:rs:a3:t4 0x194:rt:a3:t4 0x198:rt:t4:t3 0x19c:rs:t4:t3 0x19c:rt:t4:t3 0x1a0:rd:t5:t7 0x1a0:rt:a3:t4 0x1a4:rd:t7:t5 0x1a4:rt:t5:t7 0x1a8:rt:t0:t8 0x1ac:rs:t7:t5 0x1ac:rt:t4:t3 0x1b0:rs:t0:t8 0x1b0:rt:t0:t8 0x1bc:rd:t8:t0 0x1bc:rt:t0:t8 0x1c0:rd:t9:t1 0x1c0:rt:t8:t0 0x1c4:rt:a0:t9 0x1c8:rs:t9:t1 0x1cc:rs:a0:t9 0x1cc:rt:a0:t9 0x1d4:rs:a0:t9 0x1d4:rt:a0:t2 0x1d8:rt:a0:t2 0x1dc:rs:a0:t2 0x1dc:rt:t2:t4 0x1e0:rs:t2:t4 0x1e8:rt:t1:t7 0x1f0:rt:t1:t7 */
/* master_log_basyo_add - MASTER overlay, Address: 0x8013348C, Size: 500 bytes */
#include "types.h"

extern s32 get_g_zyotai_h(s32);

extern u16 basyo_pack;
extern u8 girl0_basyo;
extern u8 girl1_basyo;
extern s32 master_basyo_girl_no;
extern u8 screen_w[];
extern volatile u8 rec_idx;
extern volatile u8 now_year;
extern volatile u8 now_month;
extern volatile u8 now_day;
extern volatile u8 now_youbi;

void master_log_basyo_add(void)
{
    u8 *base;
    u8 kind;
    s32 mode;
    s32 ret;
    s32 next;
    u32 masked;
    u32 c0;
    u32 c1;
    u32 c2;
    u32 c3;

    switch ((u32)basyo_pack >> 12) {
    case 0:
        kind = 0;
        break;
    case 1:
        if (((u32)girl0_basyo >> 4) == 1U && master_basyo_girl_no == 0) {
            kind = 11;
        } else if (((u32)girl1_basyo >> 4) == 1U && master_basyo_girl_no == 1) {
            kind = 12;
        } else {
            kind = 1;
        }
        break;
    case 2:
        kind = 2;
        break;
    case 3:
        kind = 3;
        break;
    case 4:
        kind = 4;
        break;
    case 5:
        kind = 5;
        break;
    }

    base = screen_w;
    mode = master_basyo_girl_no;
    base[(rec_idx << 3) + 1886] = (u8)mode;
    base[(rec_idx << 3) + 1888] = 21;
    base[(rec_idx << 3) + 1887] = kind;
    ret = get_g_zyotai_h(mode);
    base[(rec_idx << 3) + 1889] = (u8)ret;
    c0 = rec_idx;
    base[(c0 << 3) + 1890] = now_year;
    c1 = rec_idx;
    base[(c1 << 3) + 1891] = now_month;
    c2 = rec_idx;
    base[(c2 << 3) + 1892] = now_day;
    c3 = rec_idx;
    base[(c3 << 3) + 1893] = now_youbi;

    next = rec_idx + 1;
    rec_idx = (u8)next;
    masked = next & 0xFF;
    if (masked >= 255) {
        rec_idx = 254;
    }
}
