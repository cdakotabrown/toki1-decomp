.set noreorder
.set noat

.text
.globl tt_ent_move

/* Function: tt_ent_move
 * Overlay: TT
 * Address: 0x801474FC
 * Size: 188 bytes (47 instructions)
 * Kind: leaf — prologue-less leaf/init routine (no stack frame)
 */
tt_ent_move:
    lw      $t9, 36($a0)
    lw      $t0, 44($a0)
    lw      $t6, 32($a0)
    addu    $t1, $t9, $t0
    sw      $t1, 36($a0)
    lw      $t7, 40($a0)
    lh      $t3, 38($a0)
    addu    $t8, $t6, $t7
    sh      $t3, 22($a0)
    lh      $t4, 22($a0)
    sw      $t8, 32($a0)
    lh      $t2, 34($a0)
    slti    $at, $t4, 257
    bne     $at, $zero, 0x80147560
    sh      $t2, 20($a0)
    sb      $zero, 0($a0)
    sh      $zero, 4($a0)
    sb      $zero, 12($a0)
    sh      $zero, 10($a0)
    sh      $zero, 8($a0)
    sh      $zero, 6($a0)
    sb      $zero, 15($a0)
    sb      $zero, 14($a0)
    jr      $ra
    sb      $zero, 13($a0)
/* 0x80147560 (merged block) */
    lh      $v0, 82($a0)
    addiu   $t5, $zero, 1
    slti    $at, $v0, 2561
    beq     $at, $zero, 0x80147584
    slti    $at, $v0, 4096
    lh      $v0, 82($a0)
    beq     $zero, $zero, 0x80147594
    sb      $t5, 84($a0)
    slti    $at, $v0, 4096
    bne     $at, $zero, 0x80147594
    addiu   $t6, $zero, -1
    lh      $v0, 82($a0)
    sb      $t6, 84($a0)
    lb      $t7, 84($a0)
    nop
    sll     $t8, $t7, 2
    subu    $t8, $t8, $t7
    sll     $t8, $t8, 4
    addu    $t9, $v0, $t8
    sh      $t9, 82($a0)
    jr      $ra
    nop
