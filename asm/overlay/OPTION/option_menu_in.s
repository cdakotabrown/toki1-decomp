.set noreorder
.set noat

.text
.globl option_menu_in

/* Function: option_menu_in
 * Overlay: OPTION
 * Address: 0x80136BE8
 * Size: 780 bytes (195 instructions)
 * Kind: prologue — standard function (the first two words are the step_mode
 * guard read that IDO hoists above the `addiu sp`; merged with the .sym
 * fragment previously listed as func_80136BF0).
 */
option_menu_in:
    lui     $t6, 0x800F
    lbu     $t6, -1939($t6)
    addiu   $sp, $sp, -112
    bne     $t6, $zero, 0x80136C50
    sw      $ra, 28($sp)
    addiu   $t7, $zero, 6
    lui     $at, 0x8013
    sh      $t7, -29200($at)
    lui     $at, 0x8013
    addiu   $t8, $zero, -160
    sh      $t8, -29186($at)
    lui     $at, 0x8013
    addiu   $t9, $zero, -120
    sh      $t9, -29182($at)
    lui     $at, 0x8013
    sb      $zero, -29221($at)
    lui     $at, 0x8013
    addiu   $t2, $zero, 32767
    sh      $t2, -29196($at)
    lui     $at, 0x800F
    addiu   $t3, $zero, 1
    sb      $t3, -1939($at)
    addiu   $v1, $zero, 32767
    lui     $at, 0x8013
    beq     $zero, $zero, 0x80136EDC
    sh      $v1, -29198($at)
    lui     $v1, 0x8013
    lh      $v1, -29198($v1)
    addiu   $v0, $zero, 17
    div     $v1, $v0
    lui     $t7, 0x8013
    bne     $v0, $zero, 0x80136C70
    nop
    break   0x1C00
    addiu   $at, $zero, -1
    bne     $v0, $at, 0x80136C88
    lui     $at, 0x8000
    bne     $v1, $at, 0x80136C88
    nop
    break   0x1800
    lh      $t7, -29196($t7)
    addiu   $t2, $zero, 4096
    addiu   $a0, $zero, -64
    addiu   $a1, $zero, 40
    addiu   $a2, $zero, -64
    addiu   $a3, $zero, 56
    addiu   $t0, $zero, -64
    addiu   $t1, $zero, 72
    addiu   $t3, $zero, 96
    mflo    $v1
    sll     $t4, $v1, 4
    sll     $t5, $t4, 16
    div     $t7, $v0
    sra     $t6, $t5, 16
    or      $v1, $t6, $zero
    bne     $v0, $zero, 0x80136CD0
    nop
    break   0x1C00
    addiu   $at, $zero, -1
    bne     $v0, $at, 0x80136CE8
    lui     $at, 0x8000
    bne     $t7, $at, 0x80136CE8
    nop
    break   0x1800
    lui     $at, 0x8013
    addiu   $v0, $zero, -64
    addiu   $t4, $zero, 16
    lui     $t6, 0x8013
    addiu   $t5, $zero, 52
    mflo    $t8
    sll     $t9, $t8, 4
    sh      $t9, -29196($at)
    lui     $at, 0x8013
    sh      $v1, -29198($at)
    slti    $at, $v1, 4097
    beq     $at, $zero, 0x80136EDC
    addiu   $v1, $zero, 4096
    lui     $at, 0x8013
    sh      $t2, -29196($at)
    lui     $at, 0x8013
    sh      $v1, -29198($at)
    lui     $at, 0x8013
    sh      $t5, -29132($at)
    lui     $at, 0x8013
    sh      $v0, -29118($at)
    addiu   $v1, $zero, 24
    lui     $at, 0x8013
    sh      $v1, -29114($at)
    lbu     $t6, -29153($t6)
    lui     $at, 0x8013
    ori     $t7, $t6, 0x0080
    sb      $t7, -29153($at)
    lui     $at, 0x8013
    addiu   $t2, $zero, 54
    sh      $t2, -29064($at)
    lui     $at, 0x8013
    sh      $a0, -29050($at)
    lui     $at, 0x8013
    sh      $t3, 60($sp)
    lui     $t3, 0x8013
    sh      $a1, -29046($at)
    lbu     $t3, -29085($t3)
    sh      $t4, 48($sp)
    lui     $at, 0x8013
    ori     $t4, $t3, 0x0080
    sb      $t4, -29085($at)
    lui     $at, 0x8013
    addiu   $t7, $zero, 56
    sh      $t7, -28996($at)
    lui     $at, 0x8013
    sh      $a2, -28982($at)
    addiu   $t8, $zero, 96
    sh      $t8, 62($sp)
    lui     $at, 0x8013
    sh      $a3, -28978($at)
    lui     $t8, 0x8013
    lbu     $t8, -29017($t8)
    addiu   $t9, $zero, 16
    sh      $t9, 50($sp)
    lui     $at, 0x8013
    ori     $t9, $t8, 0x0080
    sb      $t9, -29017($at)
    lui     $at, 0x8013
    addiu   $t4, $zero, 58
    sh      $t4, -28928($at)
    lui     $at, 0x8013
    sh      $t0, -28914($at)
    addiu   $t5, $zero, 96
    sh      $t5, 64($sp)
    lui     $at, 0x8013
    sh      $t1, -28910($at)
    lui     $t5, 0x8013
    lui     $t7, 0x800F
    lbu     $t5, -28949($t5)
    lbu     $t7, 559($t7)
    addiu   $t6, $zero, 16
    sh      $t6, 52($sp)
    lui     $at, 0x8013
    addiu   $t3, $zero, 16
    addiu   $t2, $zero, 96
    ori     $t6, $t5, 0x0080
    andi    $t8, $t7, 0x0001
    sb      $t6, -28949($at)
    sh      $t2, 66($sp)
    sh      $t3, 54($sp)
    sh      $v1, 72($sp)
    sh      $v0, 84($sp)
    sh      $a0, 86($sp)
    sh      $a1, 74($sp)
    sh      $a2, 88($sp)
    sh      $a3, 76($sp)
    sh      $t0, 90($sp)
    bne     $t8, $zero, 0x80136E70
    sh      $t1, 78($sp)
    lui     $t9, 0x800F
    lw      $t9, 560($t9)
    lui     $at, 0x8013
    andi    $t2, $t9, 0x0001
    bne     $t2, $zero, 0x80136E70
    nop
    sh      $zero, 60($sp)
    sb      $zero, -29153($at)
    jal     0x80136EF4
    nop
    bne     $v0, $zero, 0x80136E98
    lui     $t3, 0x800F
    lui     $at, 0x8013
    sb      $zero, -29085($at)
    lui     $at, 0x8013
    sh      $zero, 62($sp)
    sh      $zero, 64($sp)
    sb      $zero, -29017($at)
    lbu     $t3, 535($t3)
    addiu   $a0, $zero, 1
    bne     $t3, $zero, 0x80136EB4
    addiu   $a1, $zero, 4
    lui     $at, 0x8013
    sh      $zero, 66($sp)
    sb      $zero, -28949($at)
    addiu   $t4, $sp, 60
    addiu   $t5, $sp, 48
    sw      $t5, 20($sp)
    sw      $t4, 16($sp)
    addiu   $a2, $sp, 84
    jal     0x8004FC90
    addiu   $a3, $sp, 72
    lui     $at, 0x800F
    jal     0x800429FC
    sb      $zero, -2060($at)
    jal     0x801324EC
    nop
    lw      $ra, 28($sp)
    addiu   $sp, $sp, 112
    jr      $ra
    nop
