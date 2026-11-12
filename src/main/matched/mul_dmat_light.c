/* Function: mul_dmat_light */
/* Address: 0x8009F4DC */
/* Size: 112 bytes (28 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --jal-load-nop --fill-earliest */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    s32 field_00;
    s32 *field_04;
} RotInfo;

extern void gte_rotate_y_matrix(void *, s32);
extern void gte_rotate_x_matrix(void *, s32);
extern void gte_rotate_z_matrix(void *, s32);

void mul_dmat_light(void *arg0, RotInfo *arg1) {
    gte_rotate_y_matrix(arg0, ((s32 *)arg1->field_04)[1]);
    gte_rotate_x_matrix(arg0, ((s32 *)arg1->field_04)[0]);
    gte_rotate_z_matrix(arg0, ((s32 *)arg1->field_04)[2]);
}
