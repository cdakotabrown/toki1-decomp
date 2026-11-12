#!/usr/bin/env python3
import sys, struct

def pos2off(n):
    return 0x10 + n * 4

def find_text_section(data):
    e_shoff = struct.unpack_from('<I', data, 32)[0]
    e_shentsize = struct.unpack_from('<H', data, 46)[0]
    e_shnum = struct.unpack_from('<H', data, 48)[0]
    e_shstrndx = struct.unpack_from('<H', data, 50)[0]
    shstr_off = struct.unpack_from('<I', data,
                                   e_shoff + e_shstrndx * e_shentsize + 16)[0]
    for i in range(e_shnum):
        sh_off = e_shoff + i * e_shentsize
        sh_name = struct.unpack_from('<I', data, sh_off)[0]
        sh_offset = struct.unpack_from('<I', data, sh_off + 16)[0]
        sh_size = struct.unpack_from('<I', data, sh_off + 20)[0]
        name_end = data.index(b'\0', shstr_off + sh_name)
        name = data[shstr_off + sh_name:name_end].decode('ascii')
        if name == '.text':
            return sh_offset, sh_size
    return None, None

input_path, output_path = sys.argv[1], sys.argv[2]
with open(input_path, 'rb') as f:
    data = bytearray(f.read())

text_offset, text_size = find_text_section(data)

debug = []
debug.append(f"input: {input_path}")
debug.append(f"output: {output_path}")
debug.append(f"data_len: {len(data)}")
debug.append(f"text_offset: {hex(text_offset) if text_offset else 'None'}")
debug.append(f"text_size: {text_size}")

# Just copy with no patches - to see if the copy itself works
with open(output_path, 'wb') as f:
    f.write(data)

debug.append("COPIED (no patches applied)")

with open('/tmp/custom_safe_debug.txt', 'w') as f:
    f.write('\n'.join(debug))
