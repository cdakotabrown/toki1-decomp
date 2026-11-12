/* Function: _spu_init  (really Psy-Q LIBSPU `_spu_init(int mode)`) */
/* Address: 0x800957AC (recovered head; .sym entry was 0x800957BC) */
/* Size: 560 bytes (140 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 (as1-assembled, addu moves) */
/* `@CompileEnv: IDO52_AS1_XBB_SKIP_CAND=1,3,4` restores retail's missing `li` */
/* @SymbolStart: 0x800957AC */
/* @SymbolSize: 560 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x94 */
/* @PostProc: swap_instructions --offset1 0x158 --offset2 0x160 */
/* @PostProc: swap_instructions --offset1 0x160 --offset2 0x168 */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:3,lod:4 IDO52_AS1_XBB_SKIP_CAND=all IDO52_AS1_FTF_BACKWARD=occ:0 IDO52_AS1_TIE_SRC_ORDER=bb:6 IDO52_AS1_TIE_TAKE=33,52,148 IDO52_AS1_TIE_REJECT=16,25,59,99,186 */
#include "types.h"

extern void _waitfs(void);
extern void printf(void *, void *);
extern void _spu_writeByIO(void *, s32);
extern void _spu_setVoiceAttr(void *);

extern s32 spu_transMode;
extern s32 spu_addrMode;
extern s16 spu_tsa;
static s32 tcount;
static s32 *libspu_dma_dpcr_reg_ptr = (s32 *)0x1F8010F0;
extern u8 *_spu_RXX;
extern s32 _spu_inTransfer;
extern s32 _spu_transferCallback;
extern s32 _spu_IRQCallback;

extern u8 libspu_init_io_buf;
extern u8 libspu_init_timeout_fmt;
extern u8 libspu_init_reset_wait_str;

typedef struct {
    s32 field_00;
    s32 field_04;
    s16 field_08;
    s16 field_0A;
    s32 field_0C;
    s16 field_10;
    s16 field_12;
    s32 field_14;
    s32 field_18;
    s32 field_1C;
} InitStruct;

s32 _spu_init(s32 mode, volatile u8 *rxx) {
    InitStruct initData;

    spu_transMode = 1;
    spu_addrMode = 0;

    rxx = (volatile u8 *)_spu_RXX;

    spu_tsa = 0;

    *libspu_dma_dpcr_reg_ptr |= 0xB0000;

    *(volatile s16 *)(rxx + 426) = 0;

    _waitfs();

    tcount = 0;

    if ((*(volatile u16 *)(rxx + 430) & 0x7FF) == 0) {
    } else {
        do {
            tcount++;
            if (tcount >= 5001) {
                printf(&libspu_init_timeout_fmt, &libspu_init_reset_wait_str);
                rxx = (volatile u8 *)_spu_RXX;
                goto after_init;
            }
        } while ((*(volatile u16 *)(rxx + 430) & 0x7FF) != 0);
    }

after_init:
    *(volatile s16 *)(rxx + 428) = 4;
    *(volatile s16 *)(rxx + 384) = 0;
    *(volatile s16 *)(rxx + 386) = 0;
    *(volatile s16 *)(rxx + 388) = 0;
    *(volatile s16 *)(rxx + 390) = 0;
    *(volatile u16 *)(rxx + 396) = 0xFFFF;
    *(volatile u16 *)(rxx + 398) = 0xFFFF;
    *(volatile s16 *)(rxx + 408) = 0;
    *(volatile s16 *)(rxx + 410) = 0;

    if (mode == 0) {
    *(volatile s16 *)(rxx + 400) = 0;
    *(volatile s16 *)(rxx + 402) = 0;
    *(volatile s16 *)(rxx + 404) = 0;
    *(volatile s16 *)(rxx + 406) = 0;
    *(volatile s16 *)(rxx + 432) = 0;
    *(volatile s16 *)(rxx + 434) = 0;
    *(volatile s16 *)(rxx + 436) = 0;
    *(volatile s16 *)(rxx + 438) = 0;

    spu_tsa = 512;
    _spu_writeByIO(&libspu_init_io_buf, 16);

    initData.field_08 = 0;
    initData.field_0A = 0;
    initData.field_1C = 0;
    initData.field_00 = -1;
    initData.field_04 = 31;
    initData.field_10 = 16383;
    initData.field_14 = 512;

    spu_addrMode = 0;

    _spu_setVoiceAttr(&initData);

    rxx = (volatile u8 *)_spu_RXX;

    *(volatile u16 *)(rxx + 392) |= 0xFFFF;
    *(volatile u16 *)(rxx + 394) |= 0xFF;

    _waitfs();
    _waitfs();
    _waitfs();
    _waitfs();

    *(volatile u16 *)(rxx + 396) |= 0xFFFF;
    *(volatile u16 *)(rxx + 398) |= 0xFF;

    _waitfs();
    _waitfs();
    _waitfs();
    _waitfs();
    }

    *(volatile u16 *)(rxx + 426) = 0xC000u;
    _spu_inTransfer = 1;
    _spu_transferCallback = 0;
    _spu_IRQCallback = 0;

    return 0;
}
