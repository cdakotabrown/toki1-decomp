/* Address: 0x8008585C (true start) */
/* Size: 52 bytes (13 instructions) */
/* Compiler: IDO 5.2 */
/* Region: IDO (0x80081000-0x800BFFFF) */
extern int wink2_stop;
extern unsigned char cw97_mode;
extern void func_800829FC(void);

void func_8008585C(void) {
    wink2_stop = 1;
    cw97_mode = 5;
    func_800829FC();
}
