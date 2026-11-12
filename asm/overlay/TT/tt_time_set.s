.set noreorder
.set noat

.text
.globl tt_time_set

/* Function: tt_time_set
 * Overlay: TT
 * Address: 0x80148574
 * Size: 340 bytes (85 instructions)
 * Kind: leaf — prologue-less leaf/init routine (no stack frame)
 */
tt_time_set:
    sll     $t7, $a1, 2
    lui     $v0, 0x8015
    addu    $v0, $v0, $t7
    andi    $t6, $a0, 0xFFFF
    lw      $v0, 26980($v0)
    ori     $at, $zero, 0x8CA0
    bne     $t6, $at, 0x801485B4
    or      $a0, $t6, $zero
    addiu   $v1, $zero, 9
    addiu   $t8, $zero, 5
    sb      $v1, 0($v0)
    sb      $t8, 2($v0)
    sb      $v1, 3($v0)
    sb      $v1, 5($v0)
    jr      $ra
    sb      $v1, 6($v0)
/* 0x801485B4 (merged block) */
    addiu   $v1, $zero, 3600
    divu    $a0, $v1
    addiu   $a1, $zero, 600
    addiu   $a2, $zero, 60
    addiu   $a3, $zero, 10
    addiu   $at, $zero, 6
    bne     $v1, $zero, 0x801485D8
    nop
    break   0x1C00
    mflo    $t9
    mfhi    $t0
    sb      $t9, 0($v0)
    nop
    divu    $t0, $a1
    bne     $a1, $zero, 0x801485F8
    nop
    break   0x1C00
    mflo    $t1
    sb      $t1, 2($v0)
    nop
    divu    $a0, $a1
    bne     $a1, $zero, 0x80148614
    nop
    break   0x1C00
    mfhi    $t2
    nop
    nop
    divu    $t2, $a2
    bne     $a2, $zero, 0x80148630
    nop
    break   0x1C00
    mflo    $t3
    sb      $t3, 3($v0)
    nop
    divu    $a0, $a2
    bne     $a2, $zero, 0x8014864C
    nop
    break   0x1C00
    mfhi    $t4
    nop
    nop
    multu   $t4, $a3
    mflo    $t5
    nop
    nop
    divu    $t5, $a2
    bne     $a2, $zero, 0x80148678
    nop
    break   0x1C00
    mflo    $t6
    sb      $t6, 5($v0)
    nop
    multu   $a0, $a3
    mflo    $t7
    nop
    nop
    divu    $t7, $at
    mflo    $t8
    nop
    nop
    divu    $t8, $a3
    bne     $a3, $zero, 0x801486B4
    nop
    break   0x1C00
    mfhi    $t9
    sb      $t9, 6($v0)
    nop
    jr      $ra
    nop
