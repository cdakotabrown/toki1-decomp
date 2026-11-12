/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:47 IDO52_AS1_XBB_VOLATILE_OK=6 IDO52_UGEN_DEST_TEMP=27 */
/* @PostProc: swap_instructions --offset1 0x98 --offset2 0xc0 */
/* @PostProc: swap_instructions --offset1 0xa8 --offset2 0xc0 */
/* @PostProc: swap_instructions --offset1 0xec --offset2 0xf0 */
/* @PostProc: patch_insn_reg_field 0x78:rt:v1:t4 0x7c:rs:v1:t4 0x7c:rt:v1:t4 0x84:rs:v1:t4 0x90:rt:t7:t6 0x98:rt:v1:t0 0x9c:rd:t4:t5 0xa0:rd:v0:v1 0xa0:rt:t4:t5 0xa4:rs:v0:v1 0xa4:rt:t5:t7 0xa8:rt:t1:t5 0xac:rs:t5:t7 0xac:rt:t8:t9 0xb0:rs:v0:v1 0xb0:rt:t8:t9 0xb4:rs:t7:t6 0xb4:rt:t7:t6 0xbc:rs:t7:t6 0xc4:rs:v1:t0 0xc4:rt:v1:t0 0xcc:rs:v1:t0 0xd0:rd:t9:t1 0xd4:rd:v0:v1 0xd4:rt:t9:t1 0xd8:rs:v0:v1 0xd8:rt:t6:t3 0xe0:rs:t6:t3 0xe0:rt:t0:t4 0xe4:rs:v0:v1 0xe4:rt:t0:t4 0xe8:rt:a0:v1 0xec:rs:a0:v1 0xf8:rt:t1:t5 0xfc:rt:t2:t8 0x100:rs:t2:t8 0x100:rt:t2:t8 0x110:rs:a0:t8 0x110:rt:t2:a0 0x118:rt:t3:t7 0x11c:rs:t3:t7 0x11c:rt:t3:t7 0x128:rs:t3:t7 0x12c:rs:t3:t7 0x130:rs:t3:t7 0x140:rd:t7:t9 0x144:rs:t7:t9 0x148:rs:t7:t9 0x14c:rd:t4:t6 0x150:rd:t5:t0 0x150:rt:t4:t6 0x154:rs:t5:t0 0x15c:rd:t8:t1 0x160:rs:t8:t9 0x160:rt:t7:t1 0x164:rs:t8:t1 0x1e4:rt:t9:t3 0x1e8:rs:v1:a0 0x1e8:rt:a0:v1 0x1f8:rs:t9:t3 0x1f8:rt:t9:t3 0x200:rs:t9:t3 0x214:rd:t6:t4 0x218:rd:t0:t5 0x218:rt:t6:t4 0x21c:rs:t0:t5 0x224:rd:t1:t8 0x228:rs:t1:t2 0x228:rt:t2:t8 0x22c:rs:t1:t8 0x278:rt:t3:t9 0x290:rs:t3:t9 0x290:rt:t3:t9 0x294:rt:t5:t0 0x298:rd:t4:t7 0x298:rt:t3:t9 0x29c:rs:t4:t7 0x2a4:rs:t5:t0 0x2b8:rs:t5:t0 0x2ec:rt:t8:t1 0x30c:rs:t8:t1 0x30c:rt:t8:t1 0x314:rd:t7:t2 0x314:rt:t8:t1 0x318:rs:t7:t2 */
/* Address: 0x80133C50 */
/* Size: 836 bytes (209 instructions) */
#include "types.h"

typedef struct {
    u8 b[20];
} E20;

typedef struct {
    E20 e[12];
} Tbl240;

extern void strcpy(u8 *src, E20 *dst);
extern s32 get_g_zyotai_s(s32 girl);
extern void geko_state_set(s32 code);
extern void sub_sub_step_up(void);

extern u8 mname[];
extern u8 screen_w[];
extern u16 basyo_pack;
extern volatile u8 now_girl;
extern s16 geko_now_serifu;
extern u8 geko_serifu_state;
extern Tbl240 geko_reaction_tbl;

void geko_reaction(void)
{
    u32 u;
    u8 sel;
    Tbl240 tbl;
    s32 pad;
    u32 girl;
    s32 stride;

    tbl = geko_reaction_tbl;
    stride = 56;
    strcpy(mname, &tbl.e[(u32)basyo_pack >> 12]);
    screen_w[now_girl * stride + 456] |= 1;
    if (geko_serifu_state == 10) {
        screen_w[now_girl * stride + 456] |= 0x40;
    }
    geko_now_serifu = 22;
    u = get_g_zyotai_s(now_girl);
    if (geko_serifu_state == 2U) {
        geko_serifu_state = 0;
        girl = now_girl;
        if (girl == 0) {
            u = (u32)basyo_pack >> 12;
            girl = (u32)screen_w[girl * stride + 458] >> 4;
            if (u == girl && u < 6) {
                geko_state_set(2);
            } else if (u != girl && u < 6 && girl < 6) {
                geko_state_set(3);
            } else if (u < 6 && girl >= 6) {
                geko_state_set(4);
            } else {
                geko_state_set(5);
            }
        } else if (girl == 1 || girl == 2 || girl == 3) {
            u = (u32)basyo_pack >> 12;
            girl = (u32)screen_w[now_girl * stride + 458] >> 4;
            if (u == girl) {
                geko_state_set(2);
            } else if (u != girl && u < 6) {
                geko_state_set(3);
            } else {
                geko_state_set(5);
            }
        } else if (girl == 4 || girl == 5 || girl == 10) {
            if (((u32)basyo_pack >> 12) < 6) {
                geko_state_set(3);
            } else if ((u & 0x7F) < 2) {
                geko_state_set(4);
            } else {
                geko_state_set(5);
            }
        } else if (girl == 6 || girl == 7 || girl == 8 || girl == 9) {
            if (((u32)basyo_pack >> 12) < 6) {
                geko_state_set(2);
            } else {
                geko_state_set(3);
            }
        }
    }
    sub_sub_step_up();
}
