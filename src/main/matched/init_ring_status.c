/* Function: init_ring_status */
/* Address: 0x8008B250 */
/* Size: 60 bytes (15 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O1 -fno-delayed-branch --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: maspsx --aspsx-version=2.67 */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

extern s32 StRingAddr;

void init_ring_status(s32 start, u32 count) {
    u32 i;
    s32 pad;
    (void)&pad;

    i = 0;
    if (count == 0) {
        return;
    }
    do {
        *(s32 *)(StRingAddr + (i + start) * 32) = 0;
        i++;
    } while (i < count);
}
