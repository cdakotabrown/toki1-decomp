/* Likely Symbol: cal_menu_exit (from o.bin, offset +216 bytes from 0x8005BC00) */
/* Address: 0x8005BB28 */
/* Size: 64 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
/* Post-processing required: patch_frame_size.py --old 24 --new 40
 *
 * Target disassembly:
 * 0x8005BB28: addiu sp,sp,-40      frame=40
 * 0x8005BB2C: sw ra,20(sp)         save ra
 * 0x8005BB30: jal 0x4f04c          call k_disp_inc
 * 0x8005BB34: nop
 * 0x8005BB38: lui t6,0x800f
 * 0x8005BB3C: lw t6,-2328(t6)      t6 = pad_trig
 * 0x8005BB40: nop
 * 0x8005BB44: andi t7,t6,0x40      t7 = t6 & 0x40
 * 0x8005BB48: beqz t7,0x34         if (t7 == 0) goto end
 * 0x8005BB4C: lw ra,20(sp)         restore ra (delay slot)
 * 0x8005BB50: jal 0x429fc          call sub_sub_step_up
 * 0x8005BB54: nop
 * 0x8005BB58: lw ra,20(sp)         restore ra again
 * 0x8005BB5C: addiu sp,sp,40       restore stack
 * 0x8005BB60: jr ra                return
 * 0x8005BB64: nop
 */
typedef signed int s32;

extern s32 pad_trig;
extern void k_disp_inc(void);
extern void sub_sub_step_up(void);

void cal_menu_main(void) {
    k_disp_inc();
    if (pad_trig & 0x40) {
        sub_sub_step_up();
    }
}
