/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0x7C --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x90 --offset2 0x94 */
/* @PostProc: patch_insn_reg_field 0x74:rt:s5:s3 0x78:rs:s5:s3 0x78:rt:s5:s3 0x90:rt:s3:s5 0xac:rd:t7:t8 0xb0:rs:t7:t8 0xb8:rd:t8:t9 0xbc:rd:t9:t7 0xc0:rs:t9:s3 0xc0:rt:s5:t7 0xc4:rt:a0:t0 0xc8:rt:a1:t2 0xcc:rd:t0:t1 0xcc:rs:a0:t0 0xcc:rt:t8:t9 0xd0:rt:t0:t1 0xd4:rt:a2:t4 0xd8:rd:t1:t3 0xd8:rs:a1:t2 0xd8:rt:t8:t9 0xdc:rs:a2:t4 0xe4:rt:t1:t3 0xf4:rt:t2:t5 0xf8:rd:t3:t6 0x100:rt:t2:t5 0x104:rt:t3:t6 0x130:rt:s3:s5 0x140:rt:s3:s5 0x14c:rt:t4:t7 0x150:rt:t4:t7 0x154:rt:s3:s5 0x158:rt:s3:s5 0x168:rt:t5:t8 0x16c:rs:t5:t8 0x16c:rt:t5:t8 0x170:rd:v1:v0 0x170:rt:t5:t8 0x174:rs:v1:v0 0x174:rt:v0:t9 0x17c:rs:v0:t9 0x17c:rt:v0:t0 0x180:rs:v0:t0 0x180:rt:t6:t1 0x184:rs:t6:t1 0x18c:rs:v1:v0 0x18c:rt:v0:t0 0x194:rd:t7:t2 0x198:rd:t8:t3 0x198:rs:t7:s3 0x198:rt:s5:t2 0x19c:rs:t8:t3 */
/* en_nichi_shot_main - EN_NICHI overlay, Address: 0x80137FC8, Size: 468 bytes (117 insns) */
#include "types.h"

extern void en_nichi_bar_disp(void);
extern s32 en_nichi_shot_hit_check(s32);
extern void device_cmd_send(s32);

extern u16 en_nichi_bar_len;
extern u8 en_nichi_shot_state[];
extern u8 en_nichi_shot_spd[];
extern u8 en_nichi_shot_wait[];
extern s16 en_nichi_shot_hit_no[];
extern u8 class_w[][68];

void en_nichi_shot_main(void)
{
    u8 *pSt;
    u8 *base;
    u8 *pc;
    u8 *pIdx;
    u8 *spr;
    s32 i;
    s32 k1;
    s32 k2;
    s32 k4096;
    s32 k68;
    u8 st;
    u32 cw;
    s32 d;
    s16 x0;
    s16 y0;
    s16 x1;
    s32 v;
    s32 c;

    cw = (u16)(en_nichi_bar_len + 4);
    en_nichi_bar_len = cw;
    if (!(cw < 113U)) {
        en_nichi_bar_len = 0;
    }
    en_nichi_bar_disp();

    pSt = en_nichi_shot_state;
    pIdx = en_nichi_shot_spd;
    base = class_w[0];
    k1 = 1;
    k2 = 2;
    k4096 = 4096;
    k68 = 68;
    i = 0;
    do {
        st = pSt[0];
        if (st == k1) {
            d = pIdx[i] << 3;
            spr = base + i * k68;
            x0 = *(s16 *)(spr + 6690);
            y0 = *(s16 *)(spr + 6692);
            *(s16 *)(spr + 6690) = x0 - d;
            x1 = *(s16 *)(spr + 6690);
            *(s16 *)(spr + 6692) = y0 - d;
            if (x1 < 1025) {
                if (en_nichi_shot_hit_check(i) != 0) {
                    *(s16 *)(spr + 6688) = 39;
                    v = en_nichi_shot_hit_no[i];
                    if (v < 16) {
                        goto snd1288;
                    }
                    if (v < 20) {
                        goto snd1289;
                    }
snd1288:
                    device_cmd_send(1288);
                    *(s16 *)(spr + 6690) = k4096;
                    goto parked;
snd1289:
                    device_cmd_send(1289);
                    *(s16 *)(spr + 6690) = k4096;
                    goto parked;
                }
                device_cmd_send(1287);
                *(s16 *)(spr + 6688) = 38;
                *(s16 *)(spr + 6690) = k4096;
parked:
                *(s16 *)(spr + 6692) = k4096;
                pSt[0] = k2;
                st = pSt[0];
            }
        }
        if (st == k2) {
            c = en_nichi_shot_wait[i] + 1;
            en_nichi_shot_wait[i] = c;
            if (!((u32)(u8)c < 61U)) {
                spr = base + i * k68;
                spr[6667] = 0;
                pSt[0] = 0;
            }
        }
        i += 1;
        pSt += 1;
    } while (i != 5);
}
