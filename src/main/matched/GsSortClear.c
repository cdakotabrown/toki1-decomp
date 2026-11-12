/* Function: GsSortClear  (was registered as "SsQuit" at 0x8009932C/48 -- */
/* Address: 0x80099280 */
/* Size: 220 bytes (55 instructions) */
/* Region: Library (0x80086000+) */
/* @SymbolStart: 0x80099280 */
/* @SymbolSize: 220 */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 --load-dest-temp --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8 pad[4];
    u8 b4;
    u8 b5;
    u8 b6;
    u8 b7;
    u16 h8;
    u16 hA;
    u16 hC;
    u16 hE;
} VoiceRec;

extern VoiceRec libgs_clear_blkfill_tbl[];
extern s16 PSDIDX;
extern u16 VWD0;
extern u16 HWD0;
extern u16 PSDOFSX[];
extern u16 PSDOFSY[];

extern void AddPrim(s32 arg0, VoiceRec *arg1);

void GsSortClear(u8 arg0, u8 arg1, u8 arg2, s32 *arg3)
{
    libgs_clear_blkfill_tbl[PSDIDX].b4 = arg0;
    libgs_clear_blkfill_tbl[PSDIDX].b5 = arg1;
    libgs_clear_blkfill_tbl[PSDIDX].b6 = arg2;
    libgs_clear_blkfill_tbl[PSDIDX].h8 = PSDOFSX[PSDIDX];
    libgs_clear_blkfill_tbl[PSDIDX].hA = PSDOFSY[PSDIDX];
    libgs_clear_blkfill_tbl[PSDIDX].hE = VWD0;
    libgs_clear_blkfill_tbl[PSDIDX].hC = HWD0;
    AddPrim(arg3[4], &libgs_clear_blkfill_tbl[PSDIDX]);
}
