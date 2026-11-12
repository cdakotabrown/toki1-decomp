/* Debug Symbol: SD_SerialVolInit (from o.bin, high match) */
/* Address: 0x8007BAE4 */
/* Size: 88 bytes (22 instructions) */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) - uses OR encoding, 40-byte frames */
extern unsigned char serial_vol_arg;
extern void SsSetSerialAttr(int, int, int);
extern void SsSetSerialVol(int, int, int);
extern void cd_command_issue(int, int, unsigned char*);

void SD_SerialVolInit(void) {
    serial_vol_arg = 200;
    SsSetSerialAttr(0, 0, 0);
    SsSetSerialVol(0, 0, 0);
    cd_command_issue(14, 0, &serial_vol_arg);
}
