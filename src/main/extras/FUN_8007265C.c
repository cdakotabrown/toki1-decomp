/* Address: 0x8007265C */
/* Size: 20 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
extern unsigned char day_flag;
extern unsigned char day_cnt;

void FUN_8007265C(unsigned char arg0) {
    day_flag = arg0;
    day_cnt = arg0;
}
