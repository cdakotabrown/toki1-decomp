.set noreorder
.set noat

.text
.globl tt_seq_step

/* Function: tt_seq_step
 * Overlay: TT
 * Address: 0x80142AE0
 * Size: 384 bytes (96 instructions)
 * Kind: leaf — prologue-less leaf/init routine (no stack frame)
 */
tt_seq_step:
    lui     $t6, 0x8016
    lw      $v0, -30052($t6)
    sll     $t7, $a1, 2
    addu    $t7, $t7, $a1
    sll     $t7, $t7, 2
    lbu     $t8, 0($v0)
    addu    $a2, $v0, $t7
    addiu   $a2, $a2, 56
    or      $a3, $a0, $zero
    beq     $t8, $zero, 0x80142C58
    or      $v1, $a2, $zero
    lui     $t9, 0x8016
    lw      $t9, -30052($t9)
    nop
    addu    $a0, $t9, $a1
    lbu     $t2, 784($a0)
    nop
    beq     $t2, $zero, 0x80142C58
    nop
    lbu     $t0, 8($a2)
    nop
    andi    $t3, $t0, 0x0080
    beq     $t3, $zero, 0x80142B70
    andi    $t9, $t0, 0x0020
    lbu     $t4, 8($v1)
    addu    $t8, $v0, $a1
    andi    $t5, $t4, 0xFF7F
    sb      $t5, 8($v1)
    lh      $t7, 0($a3)
    nop
    sb      $t7, 6($v1)
    lh      $t6, 2($a3)
    nop
    sb      $t6, 9($v1)
    jr      $ra
    sb      $zero, 808($t8)
/* 0x80142B70 (merged block) */
    beq     $t9, $zero, 0x80142B90
    nop
    lbu     $t2, 8($v1)
    addu    $t4, $v0, $a1
    andi    $t3, $t2, 0xFFDF
    sb      $t3, 8($v1)
    jr      $ra
    sb      $zero, 808($t4)
/* 0x80142B90 (merged block) */
    lbu     $t5, 9($a2)
    nop
    bne     $t5, $zero, 0x80142C48
    nop
    lbu     $a2, 808($a0)
    addiu   $at, $zero, -1
    addiu   $a2, $a2, 2
    sll     $t7, $a2, 1
    addu    $t0, $a3, $t7
    lh      $t1, 0($t0)
    nop
    bne     $t1, $at, 0x80142BEC
    addiu   $at, $zero, -2
    lbu     $t6, 8($v1)
    addu    $a0, $v0, $a1
    ori     $t9, $t6, 0x0001
    sb      $t9, 8($v1)
    ori     $t2, $t9, 0x0040
    sb      $t2, 8($v1)
    lh      $t3, -4($t0)
    beq     $zero, $zero, 0x80142C40
    sb      $t3, 6($v1)
    addiu   $at, $zero, -2
    bne     $t1, $at, 0x80142C0C
    addu    $a0, $v0, $a1
    lh      $t4, 0($a3)
    or      $a2, $zero, $zero
    sb      $t4, 6($v1)
    lh      $t5, 2($a3)
    beq     $zero, $zero, 0x80142C40
    sb      $t5, 9($v1)
    addiu   $at, $zero, -3
    bne     $t1, $at, 0x80142C30
    addu    $a0, $v0, $a1
    sb      $zero, 784($a0)
    lbu     $t7, 8($v1)
    nop
    ori     $t6, $t7, 0x0001
    beq     $zero, $zero, 0x80142C40
    sb      $t6, 8($v1)
    sb      $t1, 6($v1)
    lh      $t8, 2($t0)
    addu    $a0, $v0, $a1
    sb      $t8, 9($v1)
    jr      $ra
    sb      $a2, 808($a0)
/* 0x80142C48 (merged block) */
    lbu     $t9, 9($v1)
    nop
    addiu   $t2, $t9, -1
    sb      $t2, 9($v1)
    jr      $ra
    nop
