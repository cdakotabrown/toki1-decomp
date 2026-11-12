/* Address: 0x80063A30, Size: 52 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_registers --from v1 --to v0 */
/* @PostProc: swap_registers --from t8 --to t9 */
#include "types.h"
extern u8 voice_level;
extern u8 bustup2_cw_mode;
void bustup_speech(void) {
    if (voice_level == 0) {
        u8 v = bustup2_cw_mode;
        if ((v & 1) == 0) {
            bustup2_cw_mode = (u8)(v | 0x11);
        }
    }
}
