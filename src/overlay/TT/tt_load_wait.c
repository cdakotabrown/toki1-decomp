/* @SymbolSize: 240 */
#include "types.h"

extern s32 device_cmd_send();
extern s32 device_ready_check();
extern s32 change_trans_pointer();
extern s32 SD_CmdProc();
extern s32 VSync();

void tt_load_wait(void)
{
    s32 i;
    s32 limit;

    device_cmd_send(127);
    change_trans_pointer((void *)0x80162000);
    device_cmd_send(17);
    device_cmd_send(34);
    device_cmd_send(33);
    device_cmd_send(47);
    i = 0;
    limit = 0x10000;
    do {
        SD_CmdProc();
        VSync(0);
    } while (i++ < limit && device_ready_check(0) == 0);

    device_cmd_send(127);
    change_trans_pointer((void *)0x8019A000);
    device_cmd_send(18);
    device_cmd_send(35);
    device_cmd_send(36);
    device_cmd_send(46);
    i = 0;
    do {
        SD_CmdProc();
        VSync(0);
    } while (i++ < limit && device_ready_check(1) == 0);
}
