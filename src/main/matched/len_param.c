/* Address: 0x8009AAE0, Size: 28 bytes */
/* @Compiler: gcc260 */
typedef signed int s32;

s32 len_param(s32 a) {
    s32 count = 0;
    if (a > 0) {
        do {
            a >>= 1;
            count++;
        } while (a > 0);
    }
    return count;
}
