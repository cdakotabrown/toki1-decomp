/* Function: _spu_w */
/* Address: 0x80095D90 */
/* Size: 292 bytes (73 instructions) */
/* Region: Library (0x80086000+), compiler IDO 5.2 (no `j`, t6/t7/t8/t9 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_instructions --offset1 0x114 --offset2 0x118 */
/* @PostProc: patch_insn_reg_field --patch 0x80:rt:a2:a3 --patch 0x84:rt:a1:a2 --patch 0x8c:rt:a2:a3 --patch 0x94:rs:a1:a2 --patch 0x9c:rt:a1:a2 --patch 0xa4:rt:a0:v0 --patch 0xa8:rd:a0:v0 --patch 0xac:rt:t0:a0 --patch 0xb0:rs:t0:a0 --patch 0xb0:rt:t0:a0 --patch 0xb4:rt:v1:a1 --patch 0xb8:rs:t0:a0 --patch 0xbc:rs:v1:a1 --patch 0xbc:rt:v1:a1 --patch 0xc8:rs:t0:a0 --patch 0xcc:rs:v1:a1 --patch 0xdc:rs:v1:a1 --patch 0xe4:rd:v0:t0 --patch 0xe4:rt:a1:a2 --patch 0xe8:rd:v0:v1 --patch 0xe8:rs:v0:t0 --patch 0xe8:rt:a0:v0 --patch 0xf0:rt:a2:a3 --patch 0xf8:rt:v0:v1 --patch 0x128:rt:v0:v1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @CompileEnv: IDO52_AS1_KEEP_ANDI=all */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @CompileEnv: IDO52_UGEN_TEMP_REG_INPLACE=4,12 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=4:3,7:9,8:10,12:2,17:8 */
#include "types.h"

extern u16 spu_tsa;
extern void *_spu_RXX;
extern void *libspu_dma4_madr_reg_ptr;
extern void *libspu_dma4_bcr_reg_ptr;
extern void *libspu_dma4_chcr_reg_ptr;
extern void *libspu_dma_dpcr_reg_ptr;
extern void *libspu_dma_dicr_reg_ptr;

extern s32 spu_transfer_startaddr;
extern u32 spu_transfer_size;
extern s32 spu_dma_mode;
extern s32 spu_transfer_time;

extern void _waitfs(void);
extern void _spu_setDelayW(void);

typedef struct {
    char pad[422];
    u16 reg1a6;
    char pad2[2];
    u16 reg1aa;
} HWRegs;

void _spu_w(s32 arg0, u32 arg1) {
    HWRegs *hw;
    s32 sectors;
    s32 flag;
    s32 t;
    s32 *r84;
    s32 *r80;
    s32 *r74;
    s32 *r78;
    s32 *r7c;

    hw = (HWRegs *)_spu_RXX;
    hw->reg1a6 = (u16)(spu_tsa & 0xFFFF);
    _waitfs();
    _waitfs();

    hw = (HWRegs *)_spu_RXX;
    t = hw->reg1aa;
    t = t & 0xFFCF;
    t = t & 0xFFFF;
    t = t | 0x20;
    t = t & 0xFFFF;
    hw->reg1aa = (u16)t;
    _waitfs();
    _waitfs();
    _spu_setDelayW();

    spu_transfer_startaddr = arg0;
    spu_transfer_size = arg1;

    flag = (arg1 & 0x3F) ? 1 : 0;
    sectors = (s32)(arg1 >> 6) + flag;

    r84 = (s32 *)libspu_dma_dicr_reg_ptr;
    r80 = (s32 *)libspu_dma_dpcr_reg_ptr;
    *r84 = *r84 | 0x900000;
    *r80 = *r80 | 0x80000;

    r74 = (s32 *)libspu_dma4_madr_reg_ptr;
    *r74 = arg0;
    r78 = (s32 *)libspu_dma4_bcr_reg_ptr;
    *r78 = (sectors << 16) | 0x10;

    r7c = (s32 *)libspu_dma4_chcr_reg_ptr;
    spu_dma_mode = 0;
    *r7c = 0x01000201;
    spu_transfer_time = sectors;
}
