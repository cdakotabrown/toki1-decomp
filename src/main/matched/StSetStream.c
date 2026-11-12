/* Function: StSetStream */
/* Address: 0x800882E0 */
/* Size: 132 bytes (33 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O1 -fno-delayed-branch --fill-epilogue */
/* @PostProc: maspsx --aspsx-version=2.67 */
/* @PostProc: scheduler_normalizer */
typedef signed short s16;
typedef signed int s32;

extern s32 StRgb24;
extern s32 StEmu_Addr;
extern s32 StFunc1;
extern s32 StFunc2;
extern s32 Stframe_no;
extern s16 Stsector_offset;
extern s32 CChannel;
extern s32 StCHANNEL;
extern void StSetMask(s32);

void StSetStream(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    StSetMask(1);
    StEmu_Addr = 0;
    StFunc1 = arg3;
    StRgb24 = arg0 & 1;
    StCHANNEL = 0;
    CChannel = 0;
    Stsector_offset = 0;
    Stframe_no = 0;
    StFunc2 = arg4;
}
