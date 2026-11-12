/* Address: 0x800A05C8 */
/* Size: 76 bytes (19 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=4:t7,5:t8 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

typedef struct {
    u32 w[23];
} Blk92;

extern Blk92 libgpu_cur_drawenv;

Blk92 *GetDrawEnv(Blk92 *env) {
    *env = libgpu_cur_drawenv;
    return env;
}
