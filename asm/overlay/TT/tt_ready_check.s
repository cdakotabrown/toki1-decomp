.set noreorder
.set noat

.text
.globl tt_ready_check

/* Function: tt_ready_check
 * Overlay: TT
 * Address: 0x80148754
 * Size: 108 bytes (27 instructions)
 * Kind: leaf — prologue-less leaf/init routine (no stack frame)
 */
tt_ready_check:
    lui     $v1, 0x8015
    lw      $v1, 27136($v1)
    nop
    lbu     $t6, 5($v1)
    nop
    bne     $t6, $zero, 0x80148788
    nop
    lbu     $t7, 6($v1)
    nop
    bne     $t7, $zero, 0x80148788
    nop
    jr      $ra
    addiu   $v0, $zero, 1
    lbu     $t8, 5($v1)
    addiu   $v0, $zero, 3
    bne     $v0, $t8, 0x801487B0
    nop
    lbu     $t9, 6($v1)
    nop
    bne     $v0, $t9, 0x801487B4
    or      $v0, $zero, $zero
    jr      $ra
    addiu   $v0, $zero, 2
    or      $v0, $zero, $zero
    jr      $ra
    nop
    nop
