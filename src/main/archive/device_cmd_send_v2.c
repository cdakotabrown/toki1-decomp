typedef unsigned int u32;
typedef unsigned short u16;

extern void SD_CmdPut(u16 arg);

int device_cmd_send(u32 arg) {
    u16 temp;
    temp = (u16)arg;
    SD_CmdPut(temp);
    return 1;
}
