/* Address: 0x80057420 */
/* Size: 164 bytes (41 instructions) */
/* Region: GCC (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 -O2 -G0 -mips1 -EL -non_shared */
/* @CompileFlags: -Wc,-pic1 */
typedef signed int s32;
typedef unsigned char u8;

extern void _sprite_set_box_shade(s32 x, s32 y, s32 w, s32 h, s32 arg4, s32 arg5, s32 arg6);

void song_base_set(s32 arg0, s32 arg1) {
    u8 val = (u8)arg1;

    _sprite_set_box_shade(-144, 91, 144, 14, 4, arg0, val);

    _sprite_set_box_shade(0, 91, 144, 14, 4, arg0, val);

    _sprite_set_box_shade(-144, 105, 256, 16, 1, 0, 1);
}
