.set noreorder
.set noat

.text
.globl option_nop0

/* Function: option_nop0
 * Overlay: OPTION
 * Address: 0x801320B4
 * Size: 16 bytes (4 instructions)
 * Kind: orphan — gap fragment recovered by build_clean_exn.py --gen-gap-asm
 */
option_nop0:
    jr      $ra
    nop
    jr      $ra
    nop
