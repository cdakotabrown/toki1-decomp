/* Function: _mk_spr_packet */
/* Address: 0x8009E2F0 */
/* Size: 64 bytes (16 instructions) */
/* Region: Library (0x80086000-0x800BFFFF) */
/* Compiler: Psy-Q GCC 2.6.0 */
/* @Compiler: gcc260 */
/* @CompileFlags: -O1 -G0 -mips1 */
extern void _mk_xpndsp(void);
extern void _mk_normsp(void);

void _mk_spr_packet(int arg0, int *arg1) {
    if (arg1[6] != 0) {
        _mk_xpndsp();
    } else {
        _mk_normsp();
    }
}
