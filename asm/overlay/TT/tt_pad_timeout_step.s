.set noreorder
.set noat

.text
.globl tt_pad_timeout_step

/* Function: tt_pad_timeout_step
 * Overlay: TT
 * Address: 0x8013A040
 * Size: 156 bytes (39 instructions)
 * Kind: leaf — prologue-less leaf/init routine (no stack frame)
 */
tt_pad_timeout_step:
    lui     $t6, 0x8016
    lw      $t6, -30096($t6)
    lui     $v1, 0x8016
    lhu     $t7, 8($t6)
    lw      $v1, -30024($v1)
    lui     $v0, 0x8016
    srl     $t8, $t7, 4
    lw      $v0, -30092($v0)
    andi    $t9, $t8, 0x0001
    addiu   $a0, $t6, 16
    beq     $t9, $zero, 0x8013A07C
    addiu   $v1, $v1, 2520
    addiu   $a1, $zero, 1
    beq     $zero, $zero, 0x8013A084
    sh      $a1, 6($v1)
    sh      $zero, 6($v1)
    addiu   $a1, $zero, 1
    lbu     $t0, 34($v0)
    nop
    bne     $t0, $zero, 0x8013A0B4
    nop
    lhu     $v1, 4($a0)
    nop
    andi    $t1, $v1, 0x0800
    bne     $t1, $zero, 0x8013A0B0
    andi    $t2, $v1, 0x0020
    beq     $t2, $zero, 0x8013A0B4
    nop
    sb      $a1, 9($v0)
    lhu     $v1, 38($v0)
    nop
    sltiu   $at, $v1, 420
    bne     $at, $zero, 0x8013A0D0
    addiu   $t3, $v1, 1
    jr      $ra
    sb      $a1, 9($v0)
/* 0x8013A0D0 (merged block) */
    sh      $t3, 38($v0)
    jr      $ra
    nop
