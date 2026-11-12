/* Address: 0x80049104 */
/* Size: 64 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
/* Post-processing required: patch_frame_size.py --old 32 --new 48
 *
 * Target disassembly:
 * 0x80049104: addiu sp,sp,-48      frame=48
 * 0x80049108: sw s1,24(sp)         save s1
 * 0x8004910C: sw s0,20(sp)         save s0
 * 0x80049110: sw ra,28(sp)         save ra
 * 0x80049114: li s0,96             s0 = 0x60
 * 0x80049118: li s1,160            s1 = 0xA0
 * 0x8004911C: jal 0x491f0          call class_clear_n
 * 0x80049120: move a0,s0           a0 = s0 (delay slot, OR encoding)
 * 0x80049124: addiu s0,s0,1        s0++
 * 0x80049128: bne s0,s1,0x18       if (s0 != s1) goto loop
 * 0x8004912C: nop
 * 0x80049130: lw ra,28(sp)         restore ra
 * 0x80049134: lw s0,20(sp)         restore s0
 * 0x80049138: lw s1,24(sp)         restore s1
 * 0x8004913C: jr ra                return
 * 0x80049140: addiu sp,sp,48       restore stack (delay slot)
 *
 * Loop from 96 (0x60) to 159 (0x9F), calling func 64 times
 */
typedef signed int s32;

extern void class_clear_n(s32 arg0);

void class_clear_f(void) {
    s32 i;
    for (i = 96; i != 160; i++) {
        class_clear_n(i);
    }
}
