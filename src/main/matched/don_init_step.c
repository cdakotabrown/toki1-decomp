/* Address: 0x80085B00 */
/* Size: 40 bytes */
/* Compiler: IDO 5.2 */
/* Region: IDO (0x80081000-0x800BFFFF) */
/* Post-processing required:
 *   - patch_frame_size.py --old-size 24 --new-size 40
 * Region: IDO (0x80081000-0x800BFFFF)
 */
extern void don_init(void);
extern void sub_sub_step_up(void);

void don_init_step(void) {
    don_init();
    sub_sub_step_up();
}
