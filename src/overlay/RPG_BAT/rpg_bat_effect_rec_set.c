/* Address: 0x8013E54C */
/* @SymbolSize: 628 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0xf0 --offset2 0xf4 */
/* @PostProc: swap_instructions --offset1 0x17c --offset2 0x180 */
/* @PostProc: swap_instructions --offset1 0x208 --offset2 0x20c */
/* @PostProc: patch_insn_reg_field 0x18:rt:v0:t0 0x24:rt:v0:t0 0x30:rt:v0:t0 0x3c:rt:v0:a3 0x48:rt:a0:a1 0x4c:rt:a1:a0 0x50:rt:a2:v1 0x54:rt:v1:v0 0x58:rs:a0:v1 0x58:rt:a0:v1 0x5c:rs:a1:a0 0x5c:rt:a1:a0 0x60:rs:a2:a1 0x60:rt:a2:a1 0x64:rs:v1:v0 0x64:rt:v1:v0 0x68:rs:v0:t0 0x68:rt:v0:t0 0x6c:rt:t0:a3 0x70:rt:a3:a2 0x74:rs:v1:v0 0x74:rt:v1:v0 0x78:rs:v1:v0 0x78:rt:a0:v1 0x7c:rs:v1:v0 0x7c:rt:a1:a0 0x80:rs:v1:v0 0x80:rt:a2:a1 0x84:rs:v1:v0 0x84:rt:a3:a2 0x88:rs:v1:v0 0x88:rt:t0:a3 0x8c:rs:v1:v0 0x8c:rt:a0:v1 0x90:rs:v1:v0 0x90:rt:a1:a0 0x94:rs:v1:v0 0x94:rt:a2:a1 0x98:rs:v1:v0 0x98:rt:a3:a2 0x9c:rs:v1:v0 0x9c:rt:t0:a3 0xa0:rs:v1:v0 0xa0:rt:a0:v1 0xa4:rs:v1:v0 0xa4:rt:a1:a0 0xa8:rs:v1:v0 0xa8:rt:a2:a1 0xac:rs:v1:v0 0xac:rt:a3:a2 0xb0:rs:v1:v0 0xb0:rt:t0:a3 0xb4:rs:v1:v0 0xb4:rt:a0:v1 0xb8:rs:v1:v0 0xb8:rt:a1:a0 0xbc:rs:v1:v0 0xbc:rt:a2:a1 0xc0:rs:v1:v0 0xc0:rt:a3:a2 0xc4:rs:v1:v0 0xc4:rt:v0:t0 0xc8:rs:v1:v0 0xc8:rt:t0:a3 0xd4:rt:a0:a1 0xd8:rt:a1:a0 0xdc:rt:a2:v1 0xe0:rt:v1:v0 0xe4:rs:a0:v1 0xe4:rt:a0:v1 0xe8:rs:a1:a0 0xe8:rt:a1:a0 0xec:rs:a2:a1 0xec:rt:a2:a1 0xf0:rs:v1:v0 0xf0:rt:v1:v0 0xf4:rs:v0:t0 0xf4:rt:v0:t0 0xf8:rt:t0:a3 0xfc:rt:a3:a2 0x100:rs:v1:v0 0x100:rt:v1:v0 0x104:rs:v1:v0 0x104:rt:a0:v1 0x108:rs:v1:v0 0x108:rt:a1:a0 0x10c:rs:v1:v0 0x10c:rt:a2:a1 0x110:rs:v1:v0 0x110:rt:a3:a2 0x114:rs:v1:v0 0x114:rt:t0:a3 0x118:rs:v1:v0 0x118:rt:a0:v1 0x11c:rs:v1:v0 0x11c:rt:a1:a0 0x120:rs:v1:v0 0x120:rt:a2:a1 0x124:rs:v1:v0 0x124:rt:a3:a2 0x128:rs:v1:v0 0x128:rt:t0:a3 0x12c:rs:v1:v0 0x12c:rt:a0:v1 0x130:rs:v1:v0 0x130:rt:a1:a0 0x134:rs:v1:v0 0x134:rt:a2:a1 0x138:rs:v1:v0 0x138:rt:a3:a2 0x13c:rs:v1:v0 0x13c:rt:t0:a3 0x140:rs:v1:v0 0x140:rt:a0:v1 0x144:rs:v1:v0 0x144:rt:a1:a0 0x148:rs:v1:v0 0x148:rt:a2:a1 0x14c:rs:v1:v0 0x14c:rt:a3:a2 0x150:rs:v1:v0 0x150:rt:v0:t0 0x154:rs:v1:v0 0x154:rt:t0:a3 0x160:rt:a0:a1 0x164:rt:a1:a0 0x168:rt:a2:v1 0x16c:rt:v1:v0 0x170:rs:a0:v1 0x170:rt:a0:v1 0x174:rs:a1:a0 0x174:rt:a1:a0 0x178:rs:a2:a1 0x178:rt:a2:a1 0x17c:rs:v1:v0 0x17c:rt:v1:v0 0x180:rs:v0:t0 0x180:rt:v0:t0 0x184:rt:t0:a3 0x188:rt:a3:a2 0x18c:rs:v1:v0 0x18c:rt:v1:v0 0x190:rs:v1:v0 0x190:rt:a0:v1 0x194:rs:v1:v0 0x194:rt:a1:a0 0x198:rs:v1:v0 0x198:rt:a2:a1 0x19c:rs:v1:v0 0x19c:rt:a3:a2 0x1a0:rs:v1:v0 0x1a0:rt:t0:a3 0x1a4:rs:v1:v0 0x1a4:rt:a0:v1 0x1a8:rs:v1:v0 0x1a8:rt:a1:a0 0x1ac:rs:v1:v0 0x1ac:rt:a2:a1 0x1b0:rs:v1:v0 0x1b0:rt:a3:a2 0x1b4:rs:v1:v0 0x1b4:rt:t0:a3 0x1b8:rs:v1:v0 0x1b8:rt:a0:v1 0x1bc:rs:v1:v0 0x1bc:rt:a1:a0 0x1c0:rs:v1:v0 0x1c0:rt:a2:a1 0x1c4:rs:v1:v0 0x1c4:rt:a3:a2 0x1c8:rs:v1:v0 0x1c8:rt:t0:a3 0x1cc:rs:v1:v0 0x1cc:rt:a0:v1 0x1d0:rs:v1:v0 0x1d0:rt:a1:a0 0x1d4:rs:v1:v0 0x1d4:rt:a2:a1 0x1d8:rs:v1:v0 0x1d8:rt:a3:a2 0x1dc:rs:v1:v0 0x1dc:rt:v0:t0 0x1e0:rs:v1:v0 0x1e0:rt:t0:a3 0x1ec:rt:a0:a1 0x1f0:rt:a1:a0 0x1f4:rt:a2:v1 0x1f8:rt:v1:v0 0x1fc:rs:a0:v1 0x1fc:rt:a0:v1 0x200:rs:a1:a0 0x200:rt:a1:a0 0x204:rs:a2:a1 0x204:rt:a2:a1 0x208:rs:v1:v0 0x208:rt:v1:v0 0x20c:rs:v0:a3 0x20c:rt:v0:a3 0x210:rt:a3:a2 0x214:rs:v1:v0 0x214:rt:v1:v0 0x218:rs:v1:v0 0x218:rt:a0:v1 0x21c:rs:v1:v0 0x21c:rt:a1:a0 0x220:rs:v1:v0 0x220:rt:a2:a1 0x224:rs:v1:v0 0x228:rs:v1:v0 0x228:rt:a3:a2 0x22c:rs:v1:v0 0x230:rs:v1:v0 0x230:rt:a0:v1 0x234:rs:v1:v0 0x234:rt:a1:a0 0x238:rs:v1:v0 0x238:rt:a2:a1 0x23c:rs:v1:v0 0x240:rs:v1:v0 0x240:rt:a3:a2 0x244:rs:v1:v0 0x248:rs:v1:v0 0x248:rt:a0:v1 0x24c:rs:v1:v0 0x24c:rt:a1:a0 0x250:rs:v1:v0 0x250:rt:a2:a1 0x254:rs:v1:v0 0x258:rs:v1:v0 0x258:rt:a3:a2 0x25c:rs:v1:v0 0x260:rs:v1:v0 0x260:rt:a0:v1 0x264:rs:v1:v0 0x264:rt:a1:a0 0x268:rs:v1:v0 0x268:rt:a2:a1 0x26c:rs:v1:v0 0x270:rs:v1:v0 0x270:rt:a3:a2 0x274:rs:v1:v0 0x274:rt:v0:a3 0x278:rs:v1:v0 */
#include "types.h"
#include "tokimeki/class_w.h"
extern CLASS_W class_w[];

extern s32 rpg_bat_res_img[];
extern s32 rpg_bat_res_anim[];
extern s32 rpg_bat_res_hdr[];

void rpg_bat_effect_rec_set(s32 sel)
{
    if (sel == 3) goto arm3;
    if (sel == 4) goto arm4;
    if (sel == 5) goto arm5;
    if (sel != 255) return;
    goto arm255;
arm3:
    {
        u8 *end = (&class_w[16].active);
        u8 *p = (&class_w[4].active);
        s32 a = rpg_bat_res_img[4];
        s32 b = rpg_bat_res_anim[4];
        s32 c = rpg_bat_res_hdr[4];

        do {
            p += 272;
            *(s32 *)(p + 6336) = a;
            *(s32 *)(p + 6340) = b;
            *(s32 *)(p + 6376) = c;
            p[6325] = 5;
            p[6391] = 29;
            *(s32 *)(p + 6404) = a;
            *(s32 *)(p + 6408) = b;
            *(s32 *)(p + 6444) = c;
            p[6393] = 5;
            p[6459] = 29;
            *(s32 *)(p + 6472) = a;
            *(s32 *)(p + 6476) = b;
            *(s32 *)(p + 6512) = c;
            p[6461] = 5;
            p[6527] = 29;
            *(s32 *)(p + 6268) = a;
            *(s32 *)(p + 6272) = b;
            *(s32 *)(p + 6308) = c;
            p[6257] = 5;
            p[6323] = 29;
        } while (p != end);
        return;
    }
arm4:
    {
        u8 *end = (&class_w[16].active);
        u8 *p = (&class_w[4].active);
        s32 a = rpg_bat_res_img[1];
        s32 b = rpg_bat_res_anim[1];
        s32 c = rpg_bat_res_hdr[1];

        do {
            p += 272;
            *(s32 *)(p + 6336) = a;
            *(s32 *)(p + 6340) = b;
            *(s32 *)(p + 6376) = c;
            p[6325] = 5;
            p[6391] = 18;
            *(s32 *)(p + 6404) = a;
            *(s32 *)(p + 6408) = b;
            *(s32 *)(p + 6444) = c;
            p[6393] = 5;
            p[6459] = 18;
            *(s32 *)(p + 6472) = a;
            *(s32 *)(p + 6476) = b;
            *(s32 *)(p + 6512) = c;
            p[6461] = 5;
            p[6527] = 18;
            *(s32 *)(p + 6268) = a;
            *(s32 *)(p + 6272) = b;
            *(s32 *)(p + 6308) = c;
            p[6257] = 5;
            p[6323] = 18;
        } while (p != end);
        return;
    }
arm5:
    {
        u8 *end = (&class_w[16].active);
        u8 *p = (&class_w[4].active);
        s32 a = rpg_bat_res_img[5];
        s32 b = rpg_bat_res_anim[5];
        s32 c = rpg_bat_res_hdr[5];

        do {
            p += 272;
            *(s32 *)(p + 6336) = a;
            *(s32 *)(p + 6340) = b;
            *(s32 *)(p + 6376) = c;
            p[6325] = 5;
            p[6391] = 28;
            *(s32 *)(p + 6404) = a;
            *(s32 *)(p + 6408) = b;
            *(s32 *)(p + 6444) = c;
            p[6393] = 5;
            p[6459] = 28;
            *(s32 *)(p + 6472) = a;
            *(s32 *)(p + 6476) = b;
            *(s32 *)(p + 6512) = c;
            p[6461] = 5;
            p[6527] = 28;
            *(s32 *)(p + 6268) = a;
            *(s32 *)(p + 6272) = b;
            *(s32 *)(p + 6308) = c;
            p[6257] = 5;
            p[6323] = 28;
        } while (p != end);
        return;
    }
arm255:
    {
        u8 *end = (&class_w[16].active);
        u8 *p = (&class_w[4].active);
        s32 a = rpg_bat_res_img[0];
        s32 b = rpg_bat_res_anim[0];
        s32 c = rpg_bat_res_hdr[0];

        do {
            p += 272;
            *(s32 *)(p + 6336) = a;
            *(s32 *)(p + 6340) = b;
            *(s32 *)(p + 6376) = c;
            p[6391] = 0;
            p[6325] = 3;
            p[6327] = 0;
            *(s32 *)(p + 6404) = a;
            *(s32 *)(p + 6408) = b;
            *(s32 *)(p + 6444) = c;
            p[6459] = 0;
            p[6393] = 3;
            p[6395] = 0;
            *(s32 *)(p + 6472) = a;
            *(s32 *)(p + 6476) = b;
            *(s32 *)(p + 6512) = c;
            p[6527] = 0;
            p[6461] = 3;
            p[6463] = 0;
            *(s32 *)(p + 6268) = a;
            *(s32 *)(p + 6272) = b;
            *(s32 *)(p + 6308) = c;
            p[6323] = 0;
            p[6257] = 3;
            p[6259] = 0;
        } while (p != end);
    }
}
