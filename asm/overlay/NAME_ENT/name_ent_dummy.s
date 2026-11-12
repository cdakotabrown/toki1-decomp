.set noreorder
.set noat

.text
.globl name_ent_dummy

/* Function: name_ent_dummy
 * Overlay: NAME_ENT
 * Address: 0x801383E0
 * Size: 8 bytes (2 instructions)
 * Kind: orphan — gap fragment recovered by build_clean_exn.py --gen-gap-asm
 */
name_ent_dummy:
    jr      $ra
    nop
