/* Address: 0x80095AF4 */
/* Size: 484 bytes (121 instructions) */
/* Region: Library (libspu) */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
/* @Measured: 2026-09-19 6/121 exact (5.6%), compiled 106 vs target 121 words, Fixup 2 -- SEMI */
#include "types.h"

extern void *_spu_RXX;
extern u16  spu_tsa;
extern s32  tcount;
extern u8   libspu_writebyio_wrdy_timeout_fmt[];
extern u8   libspu_writebyio_wrdy_wait_str[];
extern u8   libspu_writebyio_dmaf_timeout_fmt[];
extern u8   libspu_writebyio_dmaf_wait_str[];

extern void _waitfs(void);
extern void printf(const char *fmt, ...);

typedef struct {
    u8 pad[422];
    u16 ctrl422;
    u16 data424;
    u16 status426;
    u8 pad2[2];
    u16 stat430;
} SpuRegs;

void _spu_writeByIO(u16 *src, s32 len) {
    SpuRegs *regs = (SpuRegs *)_spu_RXX;
    u16 baseStat = regs->stat430 & 0x7FF;

    regs->ctrl422 = spu_tsa;
    _waitfs();

    if (len != 0) {
        while (len != 0) {
            s32 chunk = (len < 65) ? len : 64;
            s32 i;
            s32 waits = 0;

            for (i = 0; i < chunk; i += 2) {
                regs->data424 = *src++;
            }

            regs->status426 = (regs->status426 & 0xFFCF) | 0x10;
            _waitfs();

            while ((regs->stat430 & 0x400) != 0) {
                waits++;
                if (waits >= 5001) {
                    tcount = waits;
                    printf((const char *)libspu_writebyio_wrdy_timeout_fmt, libspu_writebyio_wrdy_wait_str);
                    break;
                }
            }

            _waitfs();
            _waitfs();
            len -= chunk;
        }
    }

    regs->status426 &= 0xFFCF;

    {
        s32 waits = 0;
        while ((regs->stat430 & 0x7FF) != baseStat) {
            waits++;
            if (waits >= 5001) {
                tcount = waits;
                printf((const char *)libspu_writebyio_dmaf_timeout_fmt, libspu_writebyio_dmaf_wait_str);
                return;
            }
        }
        tcount = waits;
    }
}
