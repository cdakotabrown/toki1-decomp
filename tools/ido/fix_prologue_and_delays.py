#!/usr/bin/env python3
"""
Post-process GCC-compiled MIPS object file:
1. Reorder prologue: move global lui+lw before stack arg loads
2. Fill jal/j delay slots: sw; jal; nop -> jal; sw  
3. Add nop for load delay before first branch if needed
"""
import struct
import sys
import shutil

def read_u32(data, off):
    return struct.unpack('<I', data[off:off+4])[0]

def get_op(w):
    return (w >> 26) & 0x3F

def get_rs(w):
    return (w >> 21) & 0x1F

def get_rt(w):
    return (w >> 16) & 0x1F

def is_sw(w): return get_op(w) == 0x2B
def is_lw(w): return get_op(w) == 0x23
def is_lui(w): return get_op(w) == 0x0F
def is_jal(w): return get_op(w) == 3
def is_j(w): return get_op(w) == 2
def is_nop(w): return w == 0
def is_branch(w): return get_op(w) in (4, 5, 6, 7, 1)
def is_addiu(w): return get_op(w) == 0x09

def get_branch_offset(w):
    imm = w & 0xFFFF
    if imm & 0x8000: imm -= 0x10000
    return imm

def set_branch_offset(w, off):
    if off < 0: off = off & 0xFFFF
    return (w & 0xFFFF0000) | (off & 0xFFFF)

def main():
    in_path, out_path = sys.argv[1], sys.argv[2]
    with open(in_path, 'rb') as f:
        elf = bytearray(f.read())
    
    if elf[:4] != b'\x7fELF':
        print("Not ELF"); shutil.copy2(in_path, out_path); return
    
    # Parse ELF sections
    e_shoff = struct.unpack('<I', elf[32:36])[0]
    e_shentsize = struct.unpack('<H', elf[46:48])[0]
    e_shnum = struct.unpack('<H', elf[48:50])[0]
    e_shstrndx = struct.unpack('<H', elf[50:52])[0]
    
    shstrtab_off = e_shoff + e_shstrndx * e_shentsize
    shstrtab_offset = struct.unpack('<I', elf[shstrtab_off+16:shstrtab_off+20])[0]
    shstrtab_size = struct.unpack('<I', elf[shstrtab_off+20:shstrtab_off+24])[0]
    shstrtab = elf[shstrtab_offset:shstrtab_offset+shstrtab_size]
    
    text_sec = None
    rel_secs = []
    symtab_sec = None
    for i in range(e_shnum):
        sh_off = e_shoff + i * e_shentsize
        sh_name_idx = struct.unpack('<I', elf[sh_off:sh_off+4])[0]
        sh_type = struct.unpack('<I', elf[sh_off+4:sh_off+8])[0]
        sh_offset = struct.unpack('<I', elf[sh_off+16:sh_off+20])[0]
        sh_size = struct.unpack('<I', elf[sh_off+20:sh_off+24])[0]
        sh_info = struct.unpack('<I', elf[sh_off+28:sh_off+32])[0]
        sh_entsize = struct.unpack('<I', elf[sh_off+36:sh_off+40])[0]
        
        name_end = shstrtab.index(b'\x00', sh_name_idx)
        name = shstrtab[sh_name_idx:name_end].decode('ascii')
        
        if name == '.text':
            text_sec = {'index': i, 'sh_off': sh_off, 'offset': sh_offset, 'size': sh_size}
        if sh_type in (9, 4):
            rel_secs.append({'name': name, 'sh_off': sh_off, 'offset': sh_offset, 
                           'size': sh_size, 'info': sh_info, 'entsize': sh_entsize or 8})
        if sh_type == 2:  # SHT_SYMTAB
            symtab_sec = {'offset': sh_offset, 'size': sh_size, 'entsize': sh_entsize or 16}
    
    if not text_sec:
        print("No .text"); shutil.copy2(in_path, out_path); return
    
    t_off = text_sec['offset']
    t_size = text_sec['size']
    n = t_size // 4
    words = [read_u32(elf, t_off + i*4) for i in range(n)]
    
    # Parse relocations to track which instructions have them
    reloc_at = {}  # word_index -> list of reloc entries
    for rs in rel_secs:
        if rs['info'] != text_sec['index']: continue
        num = rs['size'] // rs['entsize']
        for j in range(num):
            ent_off = rs['offset'] + j * rs['entsize']
            r_offset = struct.unpack('<I', elf[ent_off:ent_off+4])[0]
            word_idx = r_offset // 4
            if word_idx not in reloc_at:
                reloc_at[word_idx] = []
            reloc_at[word_idx].append(ent_off)
    
    # Step 1: Identify prologue structure
    # Expected: addiu sp; [stack loads]; lui+lw(global); [more stack loads]; sw ra; beq; ...
    # Target:   addiu sp; lui+lw(global); [stack loads]; nop; beq; sw ra(delay); ...
    
    # Find the first branch
    first_branch_idx = None
    for i in range(n):
        if is_branch(words[i]):
            first_branch_idx = i
            break
    
    if first_branch_idx is None:
        print("No branch found"); shutil.copy2(in_path, out_path); return
    
    # Identify prologue components (between addiu sp and first branch)
    # Skip first instruction (addiu sp, sp, -N)
    prologue_start = 1
    prologue_end = first_branch_idx
    
    stack_loads = []  # (index, word) -- lw reg, N(sp)
    global_loads = []  # (index, word) -- lui+lw pairs
    sw_ra = None  # (index, word) -- sw ra, N(sp)
    other = []
    
    i = prologue_start
    while i < prologue_end:
        w = words[i]
        if is_lui(w) and get_rt(w) != 1:  # lui (not $at)
            # Check if next is lw from same register
            if i + 1 < prologue_end and is_lw(words[i+1]) and get_rs(words[i+1]) == get_rt(w):
                global_loads.append((i, w, i+1, words[i+1]))
                i += 2
                continue
        if is_lw(w) and get_rs(w) == 29:  # lw reg, N(sp)
            stack_loads.append((i, w))
            i += 1
            continue
        if is_sw(w) and get_rt(w) == 31 and get_rs(w) == 29:  # sw ra, N(sp)
            sw_ra = (i, w)
            i += 1
            continue
        other.append((i, w))
        i += 1
    
    # Check what's in the branch delay slot
    branch_delay = words[first_branch_idx + 1] if first_branch_idx + 1 < n else None
    
    print(f"Prologue: {len(stack_loads)} stack loads, {len(global_loads)} global loads, sw_ra={'yes' if sw_ra else 'no'}")
    print(f"First branch at index {first_branch_idx}")
    
    # Build new prologue order: global_loads first, then stack_loads, then nop (for load delay)
    # Branch: beq/beqz with sw ra in delay slot
    
    new_words = [words[0]]  # addiu sp
    
    # Build mapping old_idx -> new_idx
    old_to_new = {0: 0}
    
    # Emit global loads (lui + lw pairs)
    for (lui_idx, lui_w, lw_idx, lw_w) in global_loads:
        old_to_new[lui_idx] = len(new_words)
        new_words.append(lui_w)
        old_to_new[lw_idx] = len(new_words)
        new_words.append(lw_w)
    
    # Emit stack loads
    for (idx, w) in stack_loads:
        old_to_new[idx] = len(new_words)
        new_words.append(w)
    
    # Add nop for load delay (last stack load -> branch needs a cycle)
    new_words.append(0)  # nop
    
    # Emit branch with sw ra in delay slot
    old_to_new[first_branch_idx] = len(new_words)
    new_words.append(words[first_branch_idx])
    
    if sw_ra:
        old_to_new[sw_ra[0]] = len(new_words)
        new_words.append(sw_ra[1])
    elif branch_delay is not None and is_sw(branch_delay) and get_rt(branch_delay) == 31:
        # sw ra was already in delay slot
        old_to_new[first_branch_idx + 1] = len(new_words)
        new_words.append(branch_delay)
    
    # Copy rest of function starting from after the original delay slot
    rest_start = first_branch_idx + 2  # Skip branch + its delay slot
    
    # Map other prologue instructions
    for (idx, w) in other:
        if idx not in old_to_new:
            old_to_new[idx] = len(new_words)
            new_words.append(w)
    
    # Find what was the original delay slot content
    orig_delay_idx = first_branch_idx + 1
    
    for i in range(rest_start, n):
        if i in old_to_new:
            continue  # Already mapped
        
        w = words[i]
        
        # Check for jal/j delay slot fill opportunity
        if i + 1 < n and (is_jal(w) or is_j(w)) and is_nop(words[i+1]):
            # Look for preceding sw to fill delay slot
            if len(new_words) > 0 and is_sw(new_words[-1]):
                # Move the sw into the delay slot
                sw_word = new_words.pop()
                # Find old index of the sw we just popped
                sw_old = None
                for ok, nk in list(old_to_new.items()):
                    if nk == len(new_words):  # This was the index we popped from
                        sw_old = ok
                        break
                
                old_to_new[i] = len(new_words)
                new_words.append(w)  # jal/j
                if sw_old is not None:
                    old_to_new[sw_old] = len(new_words)
                new_words.append(sw_word)  # sw in delay slot
                old_to_new[i+1] = None  # nop removed
                i_skip = i + 1
                continue
            else:
                old_to_new[i] = len(new_words)
                new_words.append(w)
                continue
        
        old_to_new[i] = len(new_words)
        new_words.append(w)
    
    # Hmm, the loop above doesn't handle skipping properly. Let me redo this.
    # Start over with a cleaner approach.
    
    # APPROACH: 
    # 1. Take the -fno-delayed-branch output (all nop delay slots)
    # 2. Reorder prologue
    # 3. Fill jal/j delay slots with preceding sw
    # 4. Put sw ra in beqz delay slot
    
    print("Reprocessing...")
    
    # Reset
    new_words = [words[0]]  # addiu sp
    old_to_new = {0: 0}
    
    # Emit global loads first
    for (lui_idx, lui_w, lw_idx, lw_w) in global_loads:
        old_to_new[lui_idx] = len(new_words)
        new_words.append(lui_w)
        old_to_new[lw_idx] = len(new_words)
        new_words.append(lw_w)
    
    # Emit stack loads
    for (idx, w) in stack_loads:
        old_to_new[idx] = len(new_words)
        new_words.append(w)
    
    # Emit nop for load delay
    nop_idx = len(new_words)
    new_words.append(0)
    
    # Emit branch
    old_to_new[first_branch_idx] = len(new_words)
    new_words.append(words[first_branch_idx])
    
    # Put sw ra in delay slot
    if sw_ra:
        old_to_new[sw_ra[0]] = len(new_words)
        new_words.append(sw_ra[1])
    else:
        # The delay slot was already filled with sw ra by -O2
        delay_word = words[first_branch_idx + 1]
        old_to_new[first_branch_idx + 1] = len(new_words)
        new_words.append(delay_word)
    
    # Now copy the rest, applying jal delay slot fills
    rest_start = first_branch_idx + 2
    # Mark already-mapped indices
    mapped = set(old_to_new.keys())
    
    i = rest_start
    while i < n:
        if i in mapped:
            i += 1
            continue
        
        w = words[i]
        
        # Check for sw; jal/j; nop pattern
        if i + 2 < n and is_sw(w) and (is_jal(words[i+1]) or is_j(words[i+1])) and is_nop(words[i+2]):
            # Fill delay slot: emit jal, then sw
            old_to_new[i+1] = len(new_words)
            new_words.append(words[i+1])  # jal/j
            old_to_new[i] = len(new_words)
            new_words.append(w)  # sw (delay slot)
            old_to_new[i+2] = None  # nop removed
            i += 3
            continue
        
        old_to_new[i] = len(new_words)
        new_words.append(w)
        i += 1
    
    saved = n - len(new_words)
    print(f"Old: {n} words, New: {len(new_words)} words (saved {saved})")
    
    # Adjust branches
    rev_map = {v: k for k, v in old_to_new.items() if v is not None}
    for new_i in range(len(new_words)):
        w = new_words[new_i]
        if is_branch(w):
            old_i = rev_map.get(new_i)
            if old_i is None: continue
            old_off = get_branch_offset(w)
            old_target = old_i + 1 + old_off
            if old_target in old_to_new and old_to_new[old_target] is not None:
                new_target = old_to_new[old_target]
            else:
                new_target = None
                for k in sorted(old_to_new.keys()):
                    if k >= old_target and old_to_new[k] is not None:
                        new_target = old_to_new[k]
                        break
            if new_target is not None:
                new_off = new_target - (new_i + 1)
                new_words[new_i] = set_branch_offset(w, new_off)
    
    # Adjust relocations
    for rs in rel_secs:
        if rs['info'] != text_sec['index']: continue
        num = rs['size'] // rs['entsize']
        for j in range(num):
            ent_off = rs['offset'] + j * rs['entsize']
            r_offset = struct.unpack('<I', elf[ent_off:ent_off+4])[0]
            old_word = r_offset // 4
            if old_word in old_to_new and old_to_new[old_word] is not None:
                new_byte = old_to_new[old_word] * 4
                struct.pack_into('<I', elf, ent_off, new_byte)
    
    # Write new .text
    new_bytes = b''.join(struct.pack('<I', w) for w in new_words)
    padded = new_bytes + b'\x00' * (t_size - len(new_bytes))
    elf[t_off:t_off+t_size] = padded
    struct.pack_into('<I', elf, text_sec['sh_off']+20, len(new_bytes))
    
    with open(out_path, 'wb') as f:
        f.write(elf)
    print(f"Written to {out_path}")

if __name__ == '__main__':
    main()
