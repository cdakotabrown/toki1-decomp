typedef unsigned int u32;
typedef unsigned short u16;

extern void SD_CmdPut(u16 arg);

int device_cmd_send(u32 arg) {
    u32 temp = arg & 0xFFFF;
    u32 pad1, pad2, pad3, pad4;

    pad1 = 0; pad2 = 0; pad3 = 0; pad4 = 0;

    SD_CmdPut(temp);
    return 1;
}
