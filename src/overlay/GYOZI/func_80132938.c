/* @RodataBase: 0x80145868 */
/* @PostProc: pad_trailing_nop --count 3 */
/* @PostProc: patch_insn_reg_field 0x1bc:rt:t8:t9 0x1c4:rs:t8:t9 0x1cc:rt:t9:t0 0x1d0:rs:t9:t0 0x1d0:rt:t9:t0 0x1d8:rt:t0:t1 0x1dc:rs:t0:t1 0x1dc:rt:t0:t1 0x1e0:rt:t9:t0 0x1e8:rt:t1:t2 0x1ec:rs:t1:t2 0x1ec:rt:t1:t2 0x1f0:rt:t0:t1 0x1fc:rt:t1:t2 0x200:rt:t2:t3 0x204:rs:t2:t3 0x204:rt:t2:t3 0x20c:rt:t3:t4 0x210:rs:t3:t4 0x210:rt:t3:t4 0x214:rt:t2:t3 0x21c:rt:t4:t5 0x220:rs:t4:t5 0x220:rt:t4:t5 0x224:rt:t3:t4 0x230:rt:t4:t5 0x234:rt:t5:t6 0x238:rs:t5:t6 0x238:rt:t5:t6 0x240:rt:t6:t7 0x244:rs:t6:t7 0x244:rt:t6:t7 0x248:rt:t5:t6 0x250:rt:t7:t8 0x254:rs:t7:t8 0x254:rt:t7:t8 0x258:rt:t6:t7 0x264:rt:t7:t8 0x268:rt:t8:t9 0x26c:rs:t8:t9 0x26c:rt:t8:t9 0x274:rt:t9:t0 0x278:rs:t9:t0 0x278:rt:t9:t0 0x27c:rt:t8:t9 0x284:rt:t0:t1 0x288:rs:t0:t1 0x288:rt:t0:t1 0x28c:rt:t9:t0 0x298:rt:t0:t1 0x29c:rt:t1:t2 0x2a0:rs:t1:t2 0x2a0:rt:t1:t2 0x2a8:rt:t2:t3 0x2ac:rs:t2:t3 0x2ac:rt:t2:t3 0x2b0:rt:t1:t2 0x2b8:rt:t3:t4 0x2bc:rs:t3:t4 0x2bc:rt:t3:t4 0x2c0:rt:t2:t3 0x2c8:rt:t3:t4 */
/* Address: 0x80132938 */
/* Size: 716 bytes (179 instructions) + 3 trailing alignment nops = 728 */
#include "types.h"

extern s32 system_config_disp(s32);

extern u8 D_800F62CF;

extern s32 D_80145E04;
extern s32 D_80145E08;
extern s32 D_80145E0C;
extern s32 D_80145E10;
extern s32 D_80145E14;
extern s32 D_80145E18;
extern s32 D_80145E1C;
extern s32 D_80145E20;
extern s32 D_80145E24;
extern s32 D_80145E28;
extern s32 D_80145E2C;
extern s32 D_80145E30;

extern s32 D_80145E3C;
extern s32 D_80145E40;
extern s32 D_80145E44;
extern s32 D_80145E48;
extern s32 D_80145E4C;
extern s32 D_80145E50;
extern s32 D_80145E54;
extern s32 D_80145E58;
extern s32 D_80145E5C;
extern s32 D_80145E60;
extern s32 D_80145E64;
extern s32 D_80145E68;

extern s32 D_80145E74;
extern s32 D_80145E78;
extern s32 D_80145E7C;
extern s32 D_80145E80;
extern s32 D_80145E84;
extern s32 D_80145E88;
extern s32 D_80145E8C;
extern s32 D_80145E90;
extern s32 D_80145E94;
extern s32 D_80145E98;
extern s32 D_80145E9C;
extern s32 D_80145EA0;

extern s32 D_80145EA8;
extern s32 D_80145EAC;
extern s32 D_80145EB0;

void func_80132938(void)
{
    s32 ret;

    ret = system_config_disp(D_800F62CF);

    switch (D_800F62CF) {
    case 0:
        D_80145EA8 = D_80145E08;
        D_80145EAC = D_80145E40;
        D_80145EB0 = D_80145E78;
        break;
    case 1:
        D_80145EA8 = D_80145E18;
        D_80145EAC = D_80145E50;
        D_80145EB0 = D_80145E88;
        break;
    case 2:
        D_80145EA8 = D_80145E0C;
        D_80145EAC = D_80145E44;
        D_80145EB0 = D_80145E7C;
        break;
    case 3:
        D_80145EA8 = D_80145E10;
        D_80145EAC = D_80145E48;
        D_80145EB0 = D_80145E80;
        break;
    case 4:
        D_80145EA8 = D_80145E2C;
        D_80145EAC = D_80145E64;
        D_80145EB0 = D_80145E9C;
        break;
    case 5:
        D_80145EA8 = D_80145E1C;
        D_80145EAC = D_80145E54;
        D_80145EB0 = D_80145E8C;
        break;
    case 6:
        D_80145EA8 = D_80145E24;
        D_80145EAC = D_80145E5C;
        D_80145EB0 = D_80145E94;
        if ((u32)(ret & 0x7F) < 2) {
            D_80145EA8 = D_80145E20;
            D_80145EAC = D_80145E58;
            D_80145EB0 = D_80145E90;
        }
        break;
    case 7:
        D_80145EA8 = D_80145E14;
        D_80145EAC = D_80145E4C;
        D_80145EB0 = D_80145E84;
        break;
    case 8:
        D_80145EA8 = D_80145E04;
        D_80145EAC = D_80145E3C;
        D_80145EB0 = D_80145E74;
        break;
    case 9:
        D_80145EA8 = D_80145E28;
        D_80145EAC = D_80145E60;
        D_80145EB0 = D_80145E98;
        break;
    case 10:
        D_80145EA8 = D_80145E30;
        D_80145EAC = D_80145E68;
        D_80145EB0 = D_80145EA0;
        break;
    }
}
