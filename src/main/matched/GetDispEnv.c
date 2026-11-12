/* Address: 0x800A0B20 */
/* Size: 124 bytes (31 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @CompileEnv: IDO52_UGEN_MOVE_LOOP_SHIFT=2 IDO52_UGEN_TEMP_REG=4:t7,5:t8 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

typedef struct {
    s16 h[10];
} DispEnv20;

typedef struct {
    u8 pad0[0x6C];
    DispEnv20 disp;
} GpuInfo;

extern GpuInfo graph_env;

DispEnv20 *GetDispEnv(DispEnv20 *env) {
    *env = graph_env.disp;
    return env;
}
