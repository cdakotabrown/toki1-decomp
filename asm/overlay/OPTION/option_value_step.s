.set noreorder
.set noat

.text
.globl option_value_step

/* Function: option_value_step
 * Overlay: OPTION
 * Address: 0x8013A570
 * Size: 180 bytes (45 instructions)
 * Kind: leaf — prologue-less leaf/init routine (no stack frame)
 * Note: merged with the over-split fragment at 0x8013A5F8 (early `jr ra`).
 */
option_value_step:
    lui     $v0, 0x800F
    lw      $v0, -2328($v0)
    lui     $v1, 0x800F
    andi    $t6, $v0, 0x0080
    beq     $t6, $zero, 0x8013A5A8
    andi    $t9, $v0, 0x0010
    addiu   $a0, $a0, 1
    slt     $at, $a1, $a0
    beq     $at, $zero, 0x8013A59C
    lui     $v1, 0x800F
    or      $a0, $a2, $zero
    lw      $v1, -2336($v1)
    beq     $zero, $zero, 0x8013A5D8
    nop
    lw      $v1, -2336($v1)
    nop
    andi    $t7, $v1, 0x0080
    beq     $t7, $zero, 0x8013A5D8
    andi    $t8, $v1, 0x0005
    beq     $t8, $zero, 0x8013A5D8
    nop
    addiu   $a0, $a0, 1
    slt     $at, $a1, $a0
    beq     $at, $zero, 0x8013A5D8
    nop
    or      $a0, $a2, $zero
    beq     $t9, $zero, 0x8013A5F8
    andi    $t0, $v1, 0x0010
    addiu   $a0, $a0, -1
    slt     $at, $a0, $a2
    beq     $at, $zero, 0x8013A61C
    nop
    jr      $ra
    or      $v0, $a1, $zero
    beq     $t0, $zero, 0x8013A61C
    andi    $t1, $v1, 0x0005
    beq     $t1, $zero, 0x8013A61C
    nop
    addiu   $a0, $a0, -1
    slt     $at, $a0, $a2
    beq     $at, $zero, 0x8013A61C
    nop
    or      $a0, $a1, $zero
    jr      $ra
    or      $v0, $a0, $zero
