.set noreorder
.set noat

.text
.globl func_8015B530

/* Function: func_8015B530
 * Overlay: EVENT
 * Address: 0x8015B530 (runtime 0x8011F530: EVENT.EXN is linked at 0x800F6000)
 * Size: 16 bytes (4 instructions)
 * Kind: bios_trampoline -- Sony LIBAPI kernel trampoline, A(0x30) (srand in PSX-SPX)
 */
func_8015B530:
    addiu   $t2, $zero, 0xA0
    jr      $t2
    addiu   $t1, $zero, 0x30
    nop
