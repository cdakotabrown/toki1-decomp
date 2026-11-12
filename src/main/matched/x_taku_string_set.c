/* Likely Symbol: gnsx (from o.bin, offset +92 bytes from 0x800511F4) */
/* Address: 0x80051198 */
/* Size: 132 bytes (33 instructions) */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @CompileFlags: -Wc,-pic1 */
/* Post-processing required:
 *   Patch offsets: -32 -> -48, 32 -> 48, 36 -> 52, 40 -> 56, 44 -> 60
 *
 * Example patch script:
 *   offset_map = {-32: -48, 32: 48, 36: 52, 40: 56, 44: 60}
 *   # Apply to all sw/lw with sp-relative addressing
 *
 * Three calls to set_kanji_string with different parameters.
 * Third call is conditional on arg3 == 3 and arg2 != 0.
 * @CompileFlags: -Wc,-pic1
 */
typedef signed int s32;

extern void set_kanji_string(s32 a0, s32 a1, s32 a2, s32 a3, s32 stack0);

void x_taku_string_set(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {

    set_kanji_string(-0x78, 0x32, 0, arg0, 0);

    set_kanji_string(-0x78, 0x42, 0, arg1, 0);

    if (arg3 == 3) {
        if (arg2 != 0) {
            set_kanji_string(-0x78, 0x52, 0, arg2, 0);
        }
    }
}
