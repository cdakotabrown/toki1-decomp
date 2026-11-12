/* Address: 0x80043274 */
/* Size: 12 bytes (+ 4 byte nop padding = 16 on boundary) */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
extern int pad_prev;

void FUN_80043274(int val) {
    pad_prev = val;
}
