.set noreorder
.set noat

.text
.globl tt_shot_sprite_set

/* Function: tt_shot_sprite_set
 * Overlay: TT
 * Address: 0x80133C1C
 * Size: 248 bytes (62 instructions)
 * Kind: leaf — prologue-less leaf/init routine (no stack frame)
 */
tt_shot_sprite_set:
    addiu   $v0, $zero, 1
    sll     $t7, $a2, 2
    lui     $t8, 0x8015
    sb      $v0, 0($a0)
    sh      $a1, 2($a0)
    sh      $v0, 16($a0)
    addu    $t8, $t8, $t7
    lw      $t8, -7316($t8)
    addiu   $t0, $a2, -18
    addiu   $t9, $zero, 128
    sltiu   $at, $t0, 6
    sb      $t9, 64($a0)
    beq     $at, $zero, 0x80133D0C
    sw      $t8, 56($a0)
    sll     $t0, $t0, 2
    lui     $at, 0x8015
    addu    $at, $at, $t0
    lw      $t0, -11168($at)
    nop
    jr      $t0
    nop
    lui     $t3, 0x8015
    addiu   $t1, $zero, 10
    addiu   $t2, $zero, 15499
    addiu   $t3, $t3, 23394
    sh      $t1, 82($a0)
    sh      $t2, 84($a0)
    jr      $ra
    sw      $t3, 76($a0)
/* 0x80133C90 (merged block) */
    lui     $t6, 0x8015
    addiu   $t4, $zero, 10
    addiu   $t5, $zero, 15500
    addiu   $t6, $t6, 23394
    sh      $t4, 82($a0)
    sh      $t5, 84($a0)
    jr      $ra
    sw      $t6, 76($a0)
/* 0x80133CB0 (merged block) */
    lui     $t9, 0x8015
    addiu   $t7, $zero, 18
    addiu   $t8, $zero, 15571
    addiu   $t9, $t9, 23394
    sh      $t7, 82($a0)
    sh      $t8, 84($a0)
    jr      $ra
    sw      $t9, 76($a0)
/* 0x80133CD0 (merged block) */
    lui     $t2, 0x8015
    addiu   $t0, $zero, 18
    addiu   $t1, $zero, 15572
    addiu   $t2, $t2, 23388
    sh      $t0, 82($a0)
    sh      $t1, 84($a0)
    jr      $ra
    sw      $t2, 76($a0)
/* 0x80133CF0 (merged block) */
    lui     $t5, 0x8015
    addiu   $t3, $zero, 18
    addiu   $t4, $zero, 15573
    addiu   $t5, $t5, 23388
    sh      $t3, 82($a0)
    sh      $t4, 84($a0)
    sw      $t5, 76($a0)
    jr      $ra
    nop
