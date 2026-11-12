.set noreorder
.set noat

.text
.globl taiiku_pose_advance

/* Function: taiiku_pose_advance
 * Overlay: TAIIKU
 * Address: 0x80146D28
 * Size: 148 bytes (37 instructions)
 * Kind: merged — .sym blob-split re-merge of over-split jr-ra fragments
 */
taiiku_pose_advance:
    lui     $t6, 0x8015
    lw      $t6, -23088($t6)
    lui     $v1, 0x8015
    bne     $t6, $zero, 0x80146D44
    lui     $at, 0x8015
    jr      $ra
    sb      $zero, -23084($at)
    lw      $v1, -23080($v1)
    lui     $t7, 0x8015
    sltiu   $at, $v1, 6
    bne     $at, $zero, 0x80146D68
    sltiu   $at, $v1, 11
    lw      $t7, -23068($t7)
    nop
    bgtz    $t7, 0x80146D7C
    sltiu   $at, $v1, 11
    bne     $at, $zero, 0x80146DB4
    lui     $t8, 0x8015
    lw      $t8, -23064($t8)
    nop
    beq     $t8, $zero, 0x80146DB4
    lui     $v0, 0x8015
    lbu     $v0, -23084($v0)
    nop
    sltiu   $at, $v0, 7
    bne     $at, $zero, 0x80146DA8
    addiu   $t9, $v0, 1
    addiu   $v0, $zero, 2
    lui     $at, 0x8015
    sb      $v0, -23084($at)
    jr      $ra
    sw      $zero, -23080($at)
    lui     $at, 0x8015
    sb      $t9, -23084($at)
    sw      $zero, -23080($at)
    jr      $ra
    nop
