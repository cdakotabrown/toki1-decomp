.set noreorder
.set noat

.text
.globl taiiku_balance_advance

/* Function: taiiku_balance_advance
 * Overlay: TAIIKU
 * Address: 0x8013D1D0
 * Size: 396 bytes (99 instructions)
 * Kind: merged — .sym blob-split re-merge of over-split jr-ra fragments
 */
taiiku_balance_advance:
    lui     $v0, 0x8015
    lw      $v0, -25728($v0)
    lui     $t6, 0x8015
    beq     $v0, $zero, 0x8013D230
    lui     $t8, 0x8015
    lw      $t6, -25796($t6)
    addiu   $v1, $zero, 4
    bne     $t6, $v1, 0x8013D230
    lui     $t7, 0x8015
    lw      $t7, -25792($t7)
    lui     $at, 0x8015
    bne     $t7, $v1, 0x8013D230
    nop
    sw      $zero, -25752($at)
    addiu   $at, $zero, 1
    bne     $v0, $at, 0x8013D224
    lui     $a1, 0x0001
    lui     $a0, 0xFFFF
    lui     $at, 0x8015
    jr      $ra
    sw      $a0, -25756($at)
    lui     $at, 0x8015
    jr      $ra
    sw      $a1, -25756($at)
    lhu     $t8, -25758($t8)
    lui     $v1, 0x8015
    sltiu   $at, $t8, 11
    bne     $at, $zero, 0x8013D354
    lui     $t9, 0x8015
    lw      $v1, -25752($v1)
    lui     $at, 0x8015
    sh      $zero, -25758($at)
    lw      $t9, -25764($t9)
    lui     $a2, 0x0001
    addu    $v1, $v1, $t9
    bgez    $v1, 0x8013D268
    ori     $a2, $a2, 0x0001
    or      $v1, $zero, $zero
    slt     $at, $v1, $a2
    bne     $at, $zero, 0x8013D278
    lui     $v0, 0x8015
    lui     $v1, 0x0001
    lw      $v0, -25756($v0)
    lui     $a1, 0x0001
    blez    $v0, 0x8013D2BC
    lui     $a0, 0xFFFF
    lui     $t0, 0x8015
    lw      $t0, -25768($t0)
    lui     $at, 0x8015
    addu    $v0, $v0, $t0
    bgtz    $v0, 0x8013D304
    nop
    sw      $zero, -25724($at)
    lui     $at, 0x8015
    sw      $zero, -25704($at)
    lui     $at, 0x8015
    sw      $zero, -25768($at)
    beq     $zero, $zero, 0x8013D304
    or      $v0, $zero, $zero
    bgez    $v0, 0x8013D2F8
    lui     $t2, 0x8015
    lui     $t1, 0x8015
    lw      $t1, -25768($t1)
    lui     $at, 0x8015
    addu    $v0, $v0, $t1
    bltz    $v0, 0x8013D304
    nop
    sw      $zero, -25724($at)
    lui     $at, 0x8015
    sw      $zero, -25704($at)
    lui     $at, 0x8015
    sw      $zero, -25768($at)
    beq     $zero, $zero, 0x8013D304
    or      $v0, $zero, $zero
    lw      $t2, -25768($t2)
    nop
    addu    $v0, $v0, $t2
    slt     $at, $v0, $a0
    beq     $at, $zero, 0x8013D314
    nop
    or      $v0, $a0, $zero
    lui     $at, 0x8015
    sw      $v0, -25756($at)
    slt     $at, $v0, $a2
    bne     $at, $zero, 0x8013D32C
    lui     $at, 0x8015
    sw      $a1, -25756($at)
    slt     $at, $v1, $a0
    beq     $at, $zero, 0x8013D33C
    nop
    or      $v1, $a0, $zero
    lui     $at, 0x8015
    sw      $v1, -25752($at)
    slt     $at, $v1, $a2
    bne     $at, $zero, 0x8013D354
    lui     $at, 0x8015
    sw      $a1, -25752($at)
    jr      $ra
    nop
