/* Function: CdControlB */
/* Address:  0x80087C34 */
/* Size:     340 bytes (85 instructions) */
/* Region:   IDO (0x80081000-0x800BFFFF) */
#include "common.h"

  andi $s3, $s4, 0x00FF
      lui $s5, 0x8013
      lw $s5, 2856($s5)
      addiu $s7, $zero, -1
      addiu $fp, $zero, 1
      addiu $s0, $zero, 3
      beq $s3, $fp, 0x80087D0C
      jal 0x8008A1F8
      j 0x80087D28
      addiu $s0, $s0, -1
      beq $s1, $zero, 0x80087D00
      nop
      nop
      jal 0x8008A1F8
      nop
      jal 0x8008A1F8
      addiu $s0, $s0, -1
      bne $s0, $s7, 0x80087C94
      jal 0x80089E50
      .word 0x38420002
      j 0x80087D58
 */

void CdControlB(void) {

}
