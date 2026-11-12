/* Address: 0x80044AAC */
/* Size: 20 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
extern unsigned char device_load_start_tbl7;
extern unsigned char device_port_init_tbl0;

void FUN_80044AAC(void) {
    device_load_start_tbl7 = 0;
    device_port_init_tbl0 = 0;
}
