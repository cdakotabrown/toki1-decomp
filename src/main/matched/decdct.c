/* Address: 0x80053A8C */
/* Size: 332 bytes (83 instructions) -- the 8 bytes to 0x80053BE0 are alignment nops */
/* @SymbolSize: 332 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=1:t5,11:t3,12:t5 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0xac:rd:v1:v0 0xb4:rt:v1:v0 0xb8:rs:t4:s0 0xb8:rt:s0:t4 */
/* it was registered. The `@PostProc: custom_script custom_func_80053A8C.py` directive */
#include "types.h"

extern s32 *mdec_bs;
extern s32 *mdec_rl;
extern s32 *mdec_image;
extern char decdct_str0[];

extern void printf(char *);
extern void DecDCTReset(s32);
extern void DecDCToutCallback(s32);
extern void DecDCTinCallback(s32);
extern void DecDCTvlc(s32 *, s32 *);
extern void DecDCTin(s32 *, s32);
extern void DecDCTout(s32 *, s32);
extern void DecDCToutSync(s32);
extern void LoadImage(s16 *, s32 *);

s32 decdct(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s16 rect[4];
    s32 pad[2];
    s32 *pData;
    s32 x;
    u32 end;

    pData = mdec_bs;
    x = arg0 & 0xFFFF;
    arg3 &= 0xFFFF;

    if ((pData[0] & 0xFFFF0000u) != 0x38000000u ||
        (pData[1] & 0xFF010000u) != 0x00010000u) {
        printf(decdct_str0);
        return -1;
    }

    DecDCTReset(0);
    DecDCToutCallback(0);
    DecDCTinCallback(0);
    DecDCTvlc(mdec_bs, mdec_rl);
    DecDCTin(mdec_rl, 2);

    rect[1] = *(u16 *)&arg1;
    rect[2] = 16;
    x = (s16)x;
    end = (arg0 & 0xFFFF) + *(u16 *)&arg2;
    rect[0] = x;
    rect[3] = arg3;

    if ((u32)x < end) {
        x = (((u32)arg3 + 8) >> 4) << 7;
        do {
            DecDCTout(mdec_image, x);
            DecDCToutSync(0);
            LoadImage(rect, mdec_image);
            rect[0] = rect[0] + 16;
        } while ((u32)(s16)rect[0] < end);
    }

    return 1;
}
