.set noreorder
.set noat

.text
.globl tt_box_overlap_check

/* Function: tt_box_overlap_check
 * Overlay: TT
 * Address: 0x801454F8
 * Size: 192 bytes (48 instructions)
 * Kind: leaf — prologue-less leaf/init routine (no stack frame)
 */
tt_box_overlap_check:
    lh      $t6, 0($a0)
    lh      $t7, 0($a1)
    lh      $t3, 2($a0)
    lh      $t4, 2($a1)
    addu    $v0, $t6, $t7
    sll     $t8, $v0, 16
    sra     $v0, $t8, 16
    addu    $v1, $t3, $t4
    lh      $t8, 0($a3)
    lh      $t7, 0($a2)
    sll     $t5, $v1, 16
    sra     $v1, $t5, 16
    lh      $t5, 2($a3)
    lh      $t4, 2($a2)
    addu    $t0, $t7, $t8
    sll     $t9, $t0, 16
    sra     $t3, $t9, 16
    addu    $t1, $t4, $t5
    sll     $t6, $t1, 16
    subu    $t2, $v0, $t3
    bgez    $t2, 0x80145558
    sra     $t1, $t6, 16
    beq     $zero, $zero, 0x8014555C
    subu    $v0, $zero, $t2
    or      $v0, $t2, $zero
    lbu     $t8, 4($a1)
    lbu     $t9, 4($a3)
    subu    $a0, $v1, $t1
    addu    $t3, $t8, $t9
    sltu    $at, $v0, $t3
    beq     $at, $zero, 0x801455B0
    or      $v0, $zero, $zero
    bgez    $a0, 0x80145588
    or      $v0, $a0, $zero
    beq     $zero, $zero, 0x80145588
    subu    $v0, $zero, $a0
    lbu     $t4, 5($a1)
    lbu     $t5, 5($a3)
    nop
    addu    $t6, $t4, $t5
    sltu    $at, $v0, $t6
    beq     $at, $zero, 0x801455B0
    or      $v0, $zero, $zero
    jr      $ra
    addiu   $v0, $zero, 1
/* 0x801455AC (merged block) */
    or      $v0, $zero, $zero
    jr      $ra
    nop
