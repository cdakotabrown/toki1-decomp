.set noreorder
.set noat

.text
.globl bg_layer_scroll

/* Function: bg_layer_scroll
 * Overlay: TAIIKU
 * Address: 0x8013C810
 * Size: 204 bytes (51 instructions)
 * Kind: merged — .sym blob-split re-merge of over-split jr-ra fragments
 */
bg_layer_scroll:
    lui     $v0, 0x8015
    lh      $v0, -25708($v0)
    lui     $t6, 0x8013
    lh      $t6, -29798($t6)
    addiu   $t7, $v0, 2
    slt     $at, $t7, $t6
    beq     $at, $zero, 0x8013C84C
    lui     $t4, 0x8013
    lui     $v1, 0x8012
    addiu   $v1, $v1, 29104
    lw      $t8, 6632($v1)
    nop
    subu    $t9, $t8, $a0
    beq     $zero, $zero, 0x8013C87C
    sw      $t9, 6632($v1)
    lui     $v1, 0x8012
    addiu   $v1, $v1, 29104
    lh      $t0, 6634($v1)
    nop
    addiu   $t1, $t0, 2
    slt     $at, $t1, $v0
    beq     $at, $zero, 0x8013C87C
    nop
    lw      $t2, 6632($v1)
    nop
    addu    $t3, $t2, $a0
    sw      $t3, 6632($v1)
    lui     $v0, 0x8015
    lh      $v0, -25706($v0)
    lh      $t4, -29794($t4)
    addiu   $t5, $v0, 2
    slt     $at, $t5, $t4
    beq     $at, $zero, 0x8013C8AC
    nop
    lw      $t6, 6636($v1)
    nop
    subu    $t7, $t6, $a0
    jr      $ra
    sw      $t7, 6636($v1)
    lh      $t8, 6638($v1)
    nop
    addiu   $t9, $t8, 2
    slt     $at, $t9, $v0
    beq     $at, $zero, 0x8013C8D4
    nop
    lw      $t0, 6636($v1)
    nop
    addu    $t1, $t0, $a0
    sw      $t1, 6636($v1)
    jr      $ra
    nop
