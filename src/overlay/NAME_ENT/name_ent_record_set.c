/* @CompileFlags: -Wo,-loopunroll,0 */
/* name_ent_record_set - NAME_ENT overlay, Address: 0x80138D08. */
/* @PostProc: remap_stack_slots --from 80 --to 52 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x6C */
/* @PostProc: swap_instructions --offset1 0x7C --offset2 0x88 */
/* @PostProc: patch_insn_reg_field 0x60:rt:t1:t0 0x64:rt:t0:a2 0x68:rt:a0:v1 0x6c:rt:a3:t1 0x70:rt:t2:a3 0x74:rs:t0:a2 0x74:rt:t0:a2 0x78:rs:t1:t0 0x78:rt:t1:t0 0x7c:rs:t2:a3 0x7c:rt:t2:a3 0x80:rs:a3:t1 0x80:rt:a3:t1 0x84:rs:a0:v1 0x84:rt:a0:v1 0x88:rd:a2:a1 0x8c:rs:a0:v1 0x8c:rt:a1:a0 0x94:rs:a1:a0 0x94:rt:t0:a2 0x9c:rs:a0:v1 0xa4:rs:t1:t6 0xa4:rt:t6:t0 0xac:rs:a1:a0 0xb0:rt:a2:a1 0xb4:rs:a0:v1 0xc4:rs:a0:v1 0xc4:rt:v1:t9 0xc8:rs:t2:a3 0xcc:rt:a1:a0 0xd0:rs:a2:a1 0xd0:rt:a2:a1 0xd4:rt:v1:t9 0xd8:rs:a0:v1 0xd8:rt:a0:v1 0xdc:rs:a0:v1 0xdc:rt:a3:t1 0xe8:rt:a3:v1 0xec:rt:a0:t1 0xf0:rd:a2:a1 0xf4:rs:a3:t1 0xf4:rt:a3:t1 0xf8:rs:a0:v1 0xf8:rt:a0:v1 0xfc:rs:a0:v1 0xfc:rt:a1:a0 0x104:rs:a1:a0 0x104:rt:t0:a2 0x10c:rs:a0:v1 0x10c:rt:t9:t2 0x114:rs:t1:t2 0x114:rt:t9:t0 0x11c:rs:a1:a0 0x120:rt:a2:a1 0x124:rs:a0:v1 0x134:rs:a0:v1 0x134:rt:v1:t5 0x138:rs:t2:a3 0x13c:rt:a1:a0 0x140:rs:a2:a1 0x140:rt:a2:a1 0x144:rt:v1:t5 0x148:rs:a0:v1 0x148:rt:a0:v1 0x14c:rs:a0:v1 0x14c:rt:a3:t1 0x158:rt:a3:v1 0x15c:rt:a0:t1 0x160:rd:a2:a1 0x164:rs:a3:t1 0x164:rt:a3:t1 0x168:rs:a0:v1 0x168:rt:a0:v1 0x16c:rs:a0:v1 0x16c:rt:a1:a0 0x174:rs:a1:a0 0x174:rt:t0:a2 0x17c:rs:a0:v1 0x17c:rt:t5:t6 0x184:rs:t1:t6 0x184:rt:t5:t0 0x18c:rs:a1:a0 0x190:rd:t7:t8 0x190:rt:a2:a1 0x194:rs:a0:v1 0x194:rt:t6:t7 0x19c:rs:t6:t7 0x1a4:rs:a0:v1 0x1a4:rt:v1:t9 0x1a8:rs:t2:a3 0x1a8:rt:t7:t8 0x1ac:rt:a1:a0 0x1b0:rs:a2:a1 0x1b0:rt:a2:a1 0x1b4:rt:v1:t9 0x1b8:rs:a0:v1 0x1b8:rt:a0:v1 0x1bc:rs:a0:v1 0x1bc:rt:a3:t1 0x250:rt:t8:t2 0x254:rs:t8:t2 0x254:rt:t8:t2 0x258:rt:t4:t6 0x25c:rs:t4:t6 0x25c:rt:t4:t6 0x260:rs:t8:t2 0x260:rt:t9:t4 0x264:rs:t9:t4 0x264:rt:t3:t5 0x26c:rs:t4:t6 0x26c:rt:t5:t7 0x274:rt:t2:a3 0x278:rd:t6:t8 0x278:rs:t3:t5 0x278:rt:t5:t7 0x280:rs:t2:a3 0x280:rt:t2:a3 0x284:rt:t6:t8 0x288:rs:t2:a3 0x288:rt:t7:t5 0x28c:rt:t4:t3 0x290:rd:t3:t4 0x290:rt:t4:t3 0x294:rs:t3:t4 0x294:rt:t5:t6 0x298:rs:t7:t5 0x298:rt:t8:t7 0x29c:rd:t6:t8 0x29c:rs:t8:t6 0x29c:rt:t5:t7 0x2a0:rs:t2:a3 0x2a0:rt:t6:t8 0x2a4:rt:t4:t9 0x2a8:rs:t4:t9 0x2a8:rt:t4:t9 0x2ac:rs:t2:a3 0x2b0:rs:t4:t9 0x2b0:rt:t3:t4 0x2b4:rd:t8:t5 0x2b4:rt:t3:t4 0x2b8:rs:t8:t5 0x2b8:rt:t5:t6 0x2bc:rt:t9:t8 0x2c0:rd:t6:t9 0x2c0:rs:t9:t6 0x2c0:rt:t5:t8 0x2c4:rs:t2:a3 0x2c4:rt:t6:t9 0x2c8:rt:t7:t2 0x2cc:rs:t7:t2 0x2cc:rt:t7:t2 0x2d0:rt:t8:t7 0x2d4:rs:t8:t7 0x2d4:rt:t8:t7 0x2d8:rs:t7:t2 0x2dc:rt:t3:t5 0x2e0:rt:t4:t9 0x2e4:rs:t8:t7 0x2e4:rt:t9:t6 0x2e8:rs:t4:t9 0x2e8:rt:t4:t9 0x2ec:rd:t5:t8 0x2ec:rs:t3:t5 0x2ec:rt:t9:t6 0x2f4:rt:t5:t8 0x2f8:rs:t2:a3 0x2f8:rt:t6:t5 0x2fc:rs:t4:t9 0x2fc:rt:t8:t3 0x300:rd:t3:t4 0x300:rt:t8:t3 0x304:rs:t3:t4 0x304:rt:t9:t7 0x308:rs:t6:t5 0x308:rt:t7:t6 0x30c:rd:t5:t8 0x30c:rt:t9:t6 0x310:rs:t2:a3 0x310:rt:t5:t8 0x314:rt:t8:t9 0x318:rs:t8:t9 0x318:rt:t8:t9 0x31c:rs:t2:a3 0x320:rs:t8:t9 0x320:rt:t3:t4 0x324:rd:t7:t5 0x324:rt:t3:t4 0x32c:rs:t7:t5 0x32c:rt:t9:t7 0x330:rt:t4:t8 0x334:rd:t5:t9 0x334:rs:t4:t7 0x334:rt:t9:t8 0x338:rs:t2:a3 0x338:rt:t5:t9 */
#include "types.h"

extern u8 screen_w[];
extern char space_str[];

extern u8 name_ent_sei[];
extern u8 name_ent_mei[];
extern u8 name_ent_adana[];

extern u8 p_sei[];
extern u8 p_mei[];
extern u8 player_rec[];
extern u8 p_adana[];

extern u8 name_ent_rec_sei_fmt[];
extern u8 name_ent_rec_mei_fmt[];
extern u8 name_ent_rec_adana_fmt[];
extern u8 name_ent_rec_player_fmt[];

extern u8 name_ent_p_mon;
extern u8 name_ent_p_day;
extern u8 name_ent_p_blood;
extern u8 name_ent_fuji_mon;
extern u8 name_ent_fuji_day;
extern u8 name_ent_fuji_blood;

extern u8 p_birthday;
extern u8 girl0_birth;

extern void strncpy(u8 *a, u8 *b, s32 len);
extern void strncat(u8 *a, u8 *b, s32 len);
extern void printf(u8 *a, u8 *b);

void name_ent_record_set(void)
{
    u8 buf[16];
    u8 *lim;
    u8 *src;
    u8 *base;
    s32 i;
    s32 n;
    s32 c0;
    s32 c1;
    s32 b0;
    s32 b1;

    i = 0;
    do {
        i++;
        screen_w[i + 211] = 0;
        screen_w[i + 219] = 0;
    } while (i < 8);

    i = 0;
    do {
        i++;
        screen_w[i + 195] = 0;
        screen_w[i + 227] = 0;
    } while (&screen_w[i] < &screen_w[16]);

    b0 = (*(u8 *)&space_str[0]);
    b1 = (*(u8 *)&space_str[1]);
    base = screen_w;

    n = 0;
    lim = &name_ent_sei[6];
    src = name_ent_sei;
    do {
        c0 = src[0];
        if (c0 == b0) {
            if (src[1] == b1) {
                goto next0;
            }
        }
        if (c0 == 0) {
            if (src[1] == 0) {
                goto next0;
            }
        }
        c1 = *(volatile u8 *)(src + 1);
        base[n * 2 + 212] = c0;
        base[n * 2 + 213] = c1;
        n++;
    next0:
        src += 2;
    } while (src < lim);

    n = 0;
    lim = &name_ent_mei[6];
    src = name_ent_mei;
    do {
        c0 = src[0];
        if (c0 == b0) {
            if (src[1] == b1) {
                goto next1;
            }
        }
        if (c0 == 0) {
            if (src[1] == 0) {
                goto next1;
            }
        }
        c1 = *(volatile u8 *)(src + 1);
        base[n * 2 + 220] = c0;
        base[n * 2 + 221] = c1;
        n++;
    next1:
        src += 2;
    } while (src < lim);

    n = 0;
    lim = &name_ent_adana[10];
    src = name_ent_adana;
    do {
        c0 = src[0];
        if (c0 == b0) {
            if (src[1] == b1) {
                goto next2;
            }
        }
        if (c0 == 0) {
            if (src[1] == 0) {
                goto next2;
            }
        }
        c1 = *(volatile u8 *)(src + 1);
        base[n * 2 + 228] = c0;
        base[n * 2 + 229] = c1;
        n++;
    next2:
        src += 2;
    } while (src != lim);

    strncpy(buf, p_sei, 8);
    strncat(buf, p_mei, 8);
    strncpy(player_rec, buf, 16);

    printf(name_ent_rec_sei_fmt, p_sei);
    printf(name_ent_rec_mei_fmt, p_mei);
    printf(name_ent_rec_adana_fmt, p_adana);
    printf(name_ent_rec_player_fmt, player_rec);

    base = screen_w;

    p_birthday = (u8)(((name_ent_p_mon + 1) & 0xF) | (p_birthday & 0xFFF0));
    *(volatile u16 *)(base + 248) = (u16)((((name_ent_p_day + 1) << 4) & 0x1F0)
                               | (*(volatile u16 *)(base + 248) & 0xFE0F));
    *(volatile u8 *)(base + 249) = (u8)((((name_ent_p_blood + 1) << 1) & 0x6)
                     | (*(volatile u8 *)(base + 249) & 0xFFF9));

    girl0_birth = (u8)(((name_ent_fuji_mon + 1) & 0xF) | (girl0_birth & 0xFFF0));
    *(volatile u16 *)(base + 460) = (u16)((((name_ent_fuji_day + 1) << 4) & 0x1F0)
                               | (*(volatile u16 *)(base + 460) & 0xFE0F));
    *(volatile u8 *)(base + 461) = (u8)((((name_ent_fuji_blood + 1) << 1) & 0x6)
                     | (*(volatile u8 *)(base + 461) & 0xFFF9));
}

extern s32 tpage_set_adr;
extern s32 img_set_adr;
extern s32 anim_set_adr;
extern s32 clut_set_adr;
extern s32 bg_clut_adr;
extern s32 bg_set_adr;

void name_ent_res_adr_set(void)
{
    tpage_set_adr = (s32)0x801AAF58;
    img_set_adr = (s32)0x801AAF5C;
    anim_set_adr = (s32)0x801AAFD0;
    clut_set_adr = (s32)0x80190000;
    bg_clut_adr = (s32)0x801AA200;
    bg_set_adr = (s32)0x80192200;
}
