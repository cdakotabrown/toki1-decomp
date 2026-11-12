/* Function: CD_datasync */
/* Address:  0x8008AF08 */
/* Size:     344 bytes (86 instructions) */
/* Region:   IDO (0x80081000-0x800BFFFF) */
#include "common.h"

  jal 0x800B4020
      lui $s3, 0x800E
      lui $s0, 0x800E
      addiu $s0, $s0, 11148
      addiu $s3, $s3, 11020
      lui $s4, 0x00F0
      lui $s2, 0x0100
      jal 0x800B4020
      nop
      nop
      jal 0x800B4850
      jal 0x8008A5C4
      nop
      j 0x8008B010
      nop
      nop
      beq $s1, $zero, 0x8008AF6C
 */

void CD_datasync(void) {

}
