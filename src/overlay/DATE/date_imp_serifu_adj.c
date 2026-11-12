/* @SymbolStart: 0x80139FAC */
/* @RodataBase: 0x8015AF48 */
/* @SymbolSize: 388 */
/* @PostProc: patch_insn_reg_field 0x98:rs:v0:v1 0x98:rt:v1:v0 0xc4:rs:v0:v1 0xc4:rt:v1:v0 0xd4:rs:t4:v1 0xd4:rt:v1:t4 0xfc:rs:v0:v1 0xfc:rt:v1:v0 0x164:rs:v0:v1 0x164:rt:v1:v0 */
/* Address: 0x80139FB4, Size: 380 bytes (95 insns) */
#include "types.h"

extern u8 now_girl;
extern s16 imp;
extern s16 imp_1;
extern s16 now_serifu;

extern void date_trans_set(void);

void date_imp_serifu_adj(void)
{
    s32 v;

    switch (now_girl) {
    case 0:
    case 1:
    case 7:
        v = imp;
        if (v == 4) {
            now_serifu = now_serifu + 1;
        } else if (v == 2) {
            now_serifu = now_serifu + 2;
        }
        break;
    case 4:
    case 6:
    case 9:
        v = imp;
        if (v == 3 && imp_1 == 2) {
            now_serifu = now_serifu + 1;
        } else if (v == 3 && imp_1 == 3) {
            now_serifu = now_serifu + 2;
        }
        break;
    case 5:
        v = imp;
        if (v == 3) {
            now_serifu = now_serifu + 1;
        } else if (v == 2) {
            now_serifu = now_serifu + 2;
        }
        break;
    case 2:
    case 3:
    case 8:
    default:
        v = imp;
        if (v == 4) {
            now_serifu = now_serifu + 1;
        } else if (v == 3) {
            now_serifu = now_serifu + 2;
        }
        break;
    }

    date_trans_set();
}
