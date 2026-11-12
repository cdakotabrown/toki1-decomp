/* Address: 0x8004A4BC */
/* Size: 84 bytes (21 instructions) */
/* Region: GCC (0x80041000-0x80080FFF) */
/* Compiler: IDO 5.2 -O2 -G0 -mips1 -EL -non_shared */
/* @CompileFlags: -Wc,-pic1 */
/* Post-processing required: patch_stack_complete.py --old 24 --new 40
 * @CompileFlags: -Wc,-pic1
 */
typedef signed short s16;
typedef signed int s32;

extern void _sprite_set_box_shade(s32 x, s32 y, s32 w, s32 h, s32 arg4, s32 arg5, s32 arg6);

void box_set(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s16 *p1;
    s16 *p2;

    p1 = (s16 *)&arg1;
    p2 = (s16 *)&arg2;

    _sprite_set_box_shade(p1[0], p1[1], p2[0], p2[1], 1, arg3, 1);
}
