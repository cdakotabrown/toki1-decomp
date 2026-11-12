/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0xc0 --offset2 0xc4 */
/* @PostProc: patch_insn_reg_field 0xc4:rt:v0:v1 0x108:rs:v0:v1 */
/* Address: 0x8013900C */
/* Size: 340 bytes (85 instructions) */
#include "types.h"

typedef struct { u32 w[39]; } Tbl39;
extern Tbl39 D_801214A0;
extern u8 D_8011F6A8[];
extern volatile u8 D_800B1AF6;
extern s32 ev_flag14;
extern u8 D_800EB02B;
extern u32 D_800B1AE4;
extern u32 D_800EC2F8;
extern u8 D_800F19AB;
extern u8 D_800EC30E;
extern u8 D_800EC30D;
extern u8 D_800EC30C;
extern u8 D_800EB02F;

typedef void (*handler_t)(s32);

extern void func_80078950(u8 *, s32);

void func_8013900C(void) {
    u32 flags;
    Tbl39 tbl;
    u32 mod;
    u32 fwd;

    tbl = D_801214A0;

    func_80078950(D_8011F6A8, D_800B1AF6);

    ((handler_t)tbl.w[D_800B1AF6])(128);

    if (ev_flag14 != 0) {
        flags = D_800EB02B | 0x80;
        mod = D_800B1AE4 % 40;
        D_800EB02B = flags;
        if (mod < 20) {
            D_800EC2F8 = D_800EC2F8 & 0x7FFFFFFF;
        } else {
            D_800EC2F8 = D_800EC2F8 | 0x80000000;
        }
        fwd = D_800F19AB;
        D_800EC30E = fwd;
        D_800EC30D = fwd;
        D_800EC30C = fwd;
        D_800EB02F = fwd;
    } else {
        D_800EB02B = D_800EB02B & 0xFF7F;
        D_800EC2F8 = D_800EC2F8 | 0x80000000;
    }
}
