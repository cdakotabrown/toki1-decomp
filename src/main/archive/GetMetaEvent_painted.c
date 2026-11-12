/* Function: func_8009465C (formerly carried the o.bin name func_8009465C, a phantom */
/* Address: 0x8009465C */
/* Size: 960 bytes (240 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: custom_script custom_8009465C.py */
#include "types.h"

extern u8 *_ss_score;
extern s32 VBLANK_MINUS;

extern s32 ReadDeltaValue(s32, s32);
extern void Snd_nextseq(s32, s32);
extern void SpuVmSeqKeyOff(s32, s32);

void func_8009465C(s32 track, s32 voice, s32 extra, u32 cmd) {
    u8 *base;
    u8 *v1_arr;
    u8 *ch;
    u8 *ptr;
    s32 t0_ticks;
    u8 b0, b1, b2;
    u32 tempo;
    s32 divisor;
    s32 sr, limit, prod;
    s16 curLoop, loopCount;
    u8 *flagPtr;
    u8 *rst;

    {
        void *dummy;
        dummy = &track;
        dummy = &voice;
        (void)dummy;
    }

    base = _ss_score + 4992;

    extra = extra + voice;
    v1_arr = base + (track << 2);
    ch = (u8 *)(*(s32 *)v1_arr) + ((((extra << 2) + voice) << 3));

    cmd = cmd & 0xFF;

    if (cmd == 47) {
        goto cmd_end;
    }
    if (cmd == 81) {
        goto cmd_tempo;
    }
    goto done;

cmd_tempo:

    ptr = *(u8 **)(ch + 4);
    t0_ticks = *(s16 *)(ch + 72);
    b0 = *ptr;
    *(u8 **)(ch + 4) = ptr + 1;

    ptr = *(u8 **)(ch + 4);
    b1 = *ptr;
    *(u8 **)(ch + 4) = ptr + 1;

    ptr = *(u8 **)(ch + 4);
    b2 = *ptr;

    tempo = ((u32)b0 << 16) | ((u32)b1 << 8) | b2;
    *(u8 **)(ch + 4) = ptr + 1;

    divisor = (s32)60000000 / (s32)tempo;
    *(s32 *)(ch + 140) = divisor;

    sr = VBLANK_MINUS;
    prod = t0_ticks * divisor;

    limit = ((sr << 4) - sr) << 2;
    prod = ((prod << 2) + prod) << 1;

    if ((u32)prod < (u32)limit) {

        divisor = *(s32 *)(ch + 140);
        prod = t0_ticks * divisor;

        limit = ((sr << 2) + sr) << 2;
        limit = (limit - sr) << 2;
        limit = (limit - sr) << 3;

        *(s16 *)(ch + 108) = (u32)limit / (u32)prod;
        *(s16 *)(ch + 110) = *(s16 *)(ch + 108);
    } else {

        *(s16 *)(ch + 108) = -1;

        divisor = *(s32 *)(ch + 140);
        prod = t0_ticks * divisor;
        prod = ((prod << 2) + prod) << 1;

        sr = VBLANK_MINUS;
        limit = ((sr << 4) - sr) << 2;
        *(s16 *)(ch + 110) = (u32)prod / (u32)limit;

        sr = VBLANK_MINUS;
        limit = ((sr << 4) - sr) << 2;
        if ((u32)(limit >> 1) < (u32)prod % (u32)limit) {
            *(s16 *)(ch + 110) = *(s16 *)(ch + 110) + 1;
        }
    }

    *(s32 *)(ch + 136) = ReadDeltaValue(track, voice);
    goto done;

cmd_end:
    curLoop = *(s16 *)(ch + 70);
    loopCount = *(s16 *)(ch + 68);
    *(s16 *)(ch + 70) = curLoop + 1;

    if (loopCount == 0) {

        *(s32 *)(ch + 128) = 0;
        *(u8 *)(ch + 39) = 0;
        *(s32 *)(ch + 136) = 0;
        *(u8 **)(ch + 4) = *(u8 **)(ch + 8);
        goto done;
    }

    if (curLoop + 1 < loopCount) {

        rst = *(u8 **)(ch + 8);
        *(s32 *)(ch + 128) = 0;
        *(u8 *)(ch + 39) = 0;
        *(s32 *)(ch + 136) = 0;
        *(u8 **)(ch + 4) = rst;
        *(u8 **)(ch + 12) = rst;
        goto done;
    }

    {
        volatile s32 *vp = (volatile s32 *)&voice;
        s32 v;

        v = *vp;
        flagPtr = (u8 *)(*(volatile s32 *)v1_arr) + (((((v << 2) + v) << 2) + v) << 3);
        *(s32 *)(flagPtr + 144) &= ~1;

        v = *vp;
        flagPtr = (u8 *)(*(volatile s32 *)v1_arr) + (((((v << 2) + v) << 2) + v) << 3);
        *(s32 *)(flagPtr + 144) &= ~8;

        v = *vp;
        flagPtr = (u8 *)(*(volatile s32 *)v1_arr) + (((((v << 2) + v) << 2) + v) << 3);
        *(s32 *)(flagPtr + 144) &= ~2;

        v = *vp;
        flagPtr = (u8 *)(*(volatile s32 *)v1_arr) + (((((v << 2) + v) << 2) + v) << 3);
        *(s32 *)(flagPtr + 144) |= 0x200;

        v = *vp;
        flagPtr = (u8 *)(*(volatile s32 *)v1_arr) + (((((v << 2) + v) << 2) + v) << 3);
        *(s32 *)(flagPtr + 144) |= 0x4;
    }

    *(u8 **)(ch + 12) = *(u8 **)(ch + 8);
    *(u8 *)(ch + 43) = 0;

    if (*(s8 *)(ch + 0) != -1) {
        Snd_nextseq(*(s8 *)(ch + 0), voice);
    }

    SpuVmSeqKeyOff((voice << 8) | track, voice);
    *(s32 *)(ch + 136) = *(s16 *)(ch + 110);

done:
    return;
}
