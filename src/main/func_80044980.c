typedef unsigned int u32;
typedef unsigned short u16;

extern void SD_CmdPut(u16 arg);

int device_cmd_send(u32 arg) {
    SD_CmdPut((u16)arg);
    return 1;
}
