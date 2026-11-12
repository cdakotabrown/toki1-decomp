/* Address: 0x8009FDB0, Size: 16 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern u8 graph_env;
u8 GetGraphType(void) {
    return graph_env;
}
