/* Likely Symbol: cursor_image_load (from o.bin, offset +68 bytes from 0x8005AFF0) */
/* Address: 0x8005AFAC */
/* Size: 84 bytes (21 instructions) */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80041000-0x80080FFF) */
/* Post-processing required: patch_frame_size.py --old 32 --new 48
 * Region: GCC (0x80041000-0x80080FFF)
 *
 * Calls:
 *   load_image_rect(1008, 352, 16, 24, &cursor_image_load_tbl0)
 *   palette_upload_enqueue(&cursor_image_load_tbl1, 15, 1, 1, 0)
 */
extern void load_image_rect(int a0, int a1, int a2, int a3, void *stack_arg);
extern void palette_upload_enqueue(void *a0, int a1, int a2, int a3, int stack_arg);

extern char cursor_image_load_tbl0;
extern char cursor_image_load_tbl1;

void cursor_image_load(void) {
    load_image_rect(1008, 352, 16, 24, &cursor_image_load_tbl0);
    palette_upload_enqueue(&cursor_image_load_tbl1, 15, 1, 1, 0);
}
