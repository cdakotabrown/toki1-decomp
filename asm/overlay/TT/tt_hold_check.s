.set noreorder
.set noat

.text
.globl tt_hold_check

/* Function: tt_hold_check
 * Overlay: TT
 * Address: 0x8013EE80
 * Size: 176 bytes (44 instructions)
 * Kind: leaf — prologue-less leaf/init routine (no stack frame)
 */
tt_hold_check:
    lui     $t6, 0x8016
    lw      $t6, -30092($t6)
    lui     $a0, 0x8016
    lw      $a0, -30096($a0)
    lbu     $t7, 14($t6)
    addiu   $a0, $a0, 16
    or      $v1, $a0, $zero
    beq     $t7, $zero, 0x8013EF0C
    or      $v0, $t6, $zero
    lhu     $t8, 0($a0)
    addiu   $a1, $zero, 2316
    andi    $t9, $t8, 0x090C
    bne     $a1, $t9, 0x8013EED0
    nop
    lhu     $t0, 2($a0)
    addiu   $t2, $zero, 1
    andi    $t1, $t0, 0x090C
    beq     $a1, $t1, 0x8013EED0
    nop
    sb      $t2, 15($t6)
    lbu     $t3, 15($v0)
    nop
    beq     $t3, $zero, 0x8013EF08
    nop
    lhu     $t4, 0($v1)
    nop
    andi    $t5, $t4, 0x090C
    bne     $a1, $t5, 0x8013EF08
    nop
    lhu     $t6, 12($v0)
    nop
    addiu   $t7, $t6, 1
    beq     $zero, $zero, 0x8013EF0C
    sh      $t7, 12($v0)
    sh      $zero, 12($v0)
    lhu     $t8, 12($v0)
    or      $v0, $zero, $zero
    sltiu   $at, $t8, 31
    bne     $at, $zero, 0x8013EF28
    nop
    jr      $ra
    addiu   $v0, $zero, 1
/* 0x8013EF28 (merged block) */
    jr      $ra
    nop
