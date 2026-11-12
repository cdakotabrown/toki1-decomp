/* geko_cell_event -- GEKO overlay, Address: 0x80132D04, Size: 800 bytes (200 insns) */
/* @PostProc: patch_insn_reg_field 0x2c:rt:v1:t7 0x30:rs:v1:t7 0x30:rt:v1:t7 0x3c:rs:v1:t7 0x48:rs:v1:t7 0x4c:rt:t1:t2 0x50:rt:t7:t8 0x54:rt:t9:t0 0x58:rd:t8:t9 0x58:rt:t7:t8 0x5c:rs:t1:t2 0x5c:rt:t1:t2 0x60:rd:t0:t1 0x60:rs:t8:t9 0x60:rt:t9:t0 0x64:rd:v1:a1 0x64:rs:t0:t1 0x64:rt:t1:t2 0x68:rs:v1:a1 0x68:rt:v0:v1 0x70:rs:v0:v1 0x74:rt:t2:t3 0x78:rs:t2:t3 0x78:rt:t2:t3 0x7c:rt:t3:t4 0x80:rs:t2:t3 0x88:rs:t3:t4 0x88:rt:t3:t4 0x8c:rt:t5:t6 0x90:rd:t4:t5 0x90:rt:t3:t4 0x94:rs:t4:t5 0xa0:rs:t5:t6 0xa0:rt:t5:t6 0xa8:rd:t6:t7 0xa8:rt:t5:t6 0xac:rs:t6:t7 0xbc:rt:v1:a1 0xc0:rt:v1:a1 0xc8:rs:v1:a1 0xc8:rt:v0:v1 0xd8:rt:v1:a1 0xdc:rt:v1:a1 0xe4:rs:v1:a1 0xe4:rt:v0:v1 0xf0:rs:v0:v1 0xfc:rt:v1:a1 0x100:rt:v1:a1 0x108:rs:v1:a1 0x108:rt:v0:v1 0x114:rs:v0:v1 0x11c:rt:t7:t8 0x120:rt:t8:t9 0x124:rs:t7:t8 0x130:rs:t8:t9 0x130:rt:t8:t9 0x138:rs:t8:t9 0x144:rt:v1:a1 0x148:rt:v1:a1 0x15c:rt:v1:a1 0x160:rt:v1:a1 0x168:rt:t9:t0 0x16c:rs:t9:t0 0x16c:rt:t9:t0 0x174:rs:t9:t0 0x178:rt:t0:t1 0x17c:rs:t0:t1 0x17c:rt:t0:t1 0x184:rs:t0:t1 0x188:rt:t1:t2 0x18c:rs:t1:t2 0x18c:rt:t1:t2 0x190:rt:t2:t3 0x194:rs:t1:t2 0x1a0:rs:t2:t3 0x1a0:rt:t2:t3 0x1a8:rs:t2:t3 0x1b4:rt:v1:a1 0x1b8:rt:v1:a1 0x1c0:rs:v1:a1 0x1c0:rt:v0:v1 0x1c8:rs:v0:v1 0x1d4:rt:v1:a1 0x1d8:rt:v1:a1 0x1e0:rs:v1:a1 0x1e0:rt:v0:v1 0x1ec:rs:v0:v1 0x1f8:rt:v1:a1 0x1fc:rt:v1:a1 0x204:rs:v1:a1 0x204:rt:v0:v1 0x210:rs:v0:v1 0x21c:rt:v1:a1 0x220:rt:v1:a1 0x228:rs:v1:a1 0x228:rt:v0:v1 0x230:rt:t3:t4 0x234:rs:t3:t4 0x234:rt:t3:t4 0x23c:rs:t3:t4 0x244:rs:v0:v1 0x250:rt:v1:a1 0x254:rt:v1:a1 0x25c:rs:v1:a1 0x25c:rt:v0:v1 0x264:rt:t4:t5 0x268:rs:t4:t5 0x268:rt:t4:t5 0x270:rs:t4:t5 0x278:rs:v0:v1 0x284:rt:v1:a1 0x288:rt:v1:a1 0x290:rs:v1:a1 0x290:rt:v0:v1 0x29c:rs:v0:v1 0x2a0:rt:t5:t6 0x2ac:rt:t5:t6 0x2b0:rt:t7:t8 0x2b4:rt:t9:t0 0x2b8:rd:t6:t7 0x2b8:rt:t5:t6 0x2bc:rs:t9:t0 0x2bc:rt:t9:t0 0x2c0:rd:t8:t9 0x2c0:rs:t6:t7 0x2c0:rt:t7:t8 0x2c4:rd:v1:a1 0x2c4:rs:t8:t9 0x2c4:rt:t9:t0 0x2c8:rs:v1:a1 0x2c8:rt:v0:v1 0x2d0:rs:v0:v1 0x2d4:rt:t0:t1 0x2d8:rs:t0:t1 0x2d8:rt:t0:t1 0x2e0:rs:t0:t1 0x2f4:rt:v1:a1 0x2f8:rt:v1:a1 0x300:rs:v1:a1 0x300:rt:v0:v1 0x30c:rs:v0:v1 */
#include "types.h"

extern s32 get_g_zyotai_s(u8);
extern void geko_state_set(s32);

extern u8 screen_w[];
extern volatile u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u16 basyo_pack;
extern u8 girl0_basyo;
extern u8 now_girl;
extern u8 geko_serifu_state;

void geko_cell_event(s16 row, s16 col)
{
    u8 *p;
    u8 sel;

    sel = (u8)(get_g_zyotai_s(now_girl) & 0x7F);

    if (geko_serifu_state != 1 && geko_serifu_state != 16) {
        p = &screen_w[(row << 6) + col];

        if (p[101] == 12) {
            if (now_girl == 0) {
                if (((u32)basyo_pack >> 12) < 6U ||
                    ((u32)girl0_basyo >> 4) < 6U) {
                    geko_state_set(2);
                }
            } else {
                geko_state_set(2);
            }
        }
        if (p[101] == 4) {
            geko_state_set(6);
        }
        if (p[101] == 11) {
            if ((u32)sel < 2U || now_girl == 10) {
                geko_state_set(7);
            } else {
                geko_state_set(8);
            }
        }
        if (now_year == 97 && now_month == 9 && (u32)now_day < 14U &&
            now_girl == 10) {
            geko_state_set(8);
        }
        if (p[101] == 6) {
            geko_state_set(11);
        }
        if (p[101] == 14) {
            geko_state_set(12);
        }
        if (p[101] == 20) {
            geko_state_set(13);
        }
        if (now_year == 96 && p[101] == 2) {
            geko_state_set(14);
        }
        if (now_year == 97 && p[101] == 2) {
            geko_state_set(15);
        }
        if (p[101] == 15) {
            geko_state_set(9);
        }
    }

    p = &screen_w[(row << 6) + col];
    if (p[101] == 5 && geko_serifu_state != 16) {
        geko_serifu_state = 0;
        geko_state_set(1);
    }
    if (p[101] == 21) {
        geko_serifu_state = 0;
        geko_state_set(16);
    }
}
