/* Address: 0x80053174 - 0x800531C4 (80 bytes) */
/* Region: GCC (0x80041000 - 0x80080FFF) */
/* Compiler: IDO 5.2 */
/* @CompileFlags: -Wc,-pic1 */
typedef signed char s8;
typedef signed int s32;

extern s32 get_h_yuukou(s32 index);

void get_h_yuukou_table(s8 *dest) {
    s32 i;

    for (i = 0; i < 11; i++) {
        dest[i] = (s8)get_h_yuukou(i);
    }
}
