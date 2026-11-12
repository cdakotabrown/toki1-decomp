/* Address: 0x8004B038 */
/* Size: 144 bytes (36 instructions) */
/* Compiler: IDO 5.2 -O2 -G0 -mips1 */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @CompileFlags: -Wc,-pic1 */
typedef signed int s32;

extern void _sprite_set_box_shade(s32 x, s32 y, s32 w, s32 h, s32 arg4, s32 arg5, s32 arg6);

extern void dtd_on_tpage(s32 a0, s32 a1, s32 a2, s32 a3, s32 stack_arg);

void screen_color_show(s32 arg0) {

    _sprite_set_box_shade(-256, -120, 256, 240, 1, arg0, 129);

    _sprite_set_box_shade(0, -120, 256, 240, 1, arg0, 129);

    dtd_on_tpage(0, 0, 1, 1, 1);
}
