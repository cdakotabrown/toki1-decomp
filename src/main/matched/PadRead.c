/* Likely Symbol: startIntrVSync (from o.bin, offset +148 bytes from 0x800B42EC) */
/* Address: 0x800B4258 */
/* Size: 40 bytes */
/* Region: Library (>= 0x80086000) */
/* @Compiler: gcc272 */
/* @CompileFlags: --konami-lib-sched */
extern void PAD_dr(void);
extern int pad_buf;

int PadRead(void) {
    PAD_dr();
    return ~pad_buf;
}
