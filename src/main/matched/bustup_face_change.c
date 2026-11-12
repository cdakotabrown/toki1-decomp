/* Debug Symbol: bustup_face_change (from o.bin, high match) */
/* Address: 0x80083648 */
/* Size: 52 bytes (13 instructions) */
/* Region: GCC (0x80041000-0x80085FFF) - compiled with IDO 5.2 */
/* @CompileFlags: -Wc,-pic1 */
/* Post-processing required:
 *   python3 tools/ido/patch_stack_complete.py input.o output.o --old 24 --new 40
 */
typedef unsigned char u8;

extern void bustup_face_set2(u8);
extern void bustup_timer_clamp(u8);

void bustup_face_change(u8 arg0) {
    bustup_face_set2(arg0);
    bustup_timer_clamp(arg0);
}
