/* Address: 0x80059B88 */
/* Size: 20 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
extern unsigned short libapi_mouse_sense_x;
extern unsigned short libapi_mouse_sense_y;

void FUN_80059B88(unsigned short arg0, unsigned short arg1) {
    libapi_mouse_sense_x = arg0;
    libapi_mouse_sense_y = arg1;
}
