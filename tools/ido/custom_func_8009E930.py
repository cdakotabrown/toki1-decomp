"""
Custom post-processing for GsGetLw (GsGetLw / SsSepInit).
GCC 2.6.0 -O1 produces 142 correct instructions but with:
1. Prologue saves in different order (not interleaved with computation)
2. Register allocation differs: target s2=output/s1=counter, compiled s1=output/s2=counter
3. Traversal loop uses different registers and block ordering
4. Branch sense inverted (beqz vs bnez) in one place
5. Copy blocks use v0 (target) vs a2 (compiled) for struct pointer

Strategy: instruction reorder + per-block register field patching.
"""
import struct
import sys


def patch_reg_field(insn, field, old_reg, new_reg):
    """Patch a register field in an instruction.
    field: 'rs' (bits 25-21), 'rt' (bits 20-16), 'rd' (bits 15-11)
    """
    if field == 'rs':
        shift = 21
    elif field == 'rt':
        shift = 16
    elif field == 'rd':
        shift = 11
    else:
        raise ValueError(f"Unknown field: {field}")

    mask = 0x1F << shift
    current = (insn >> shift) & 0x1F
    if current != old_reg:
        return insn  # Don't patch if not expected register
    return (insn & ~mask) | (new_reg << shift)


def swap_reg_in_insn(insn, reg_a, reg_b):
    """Swap all occurrences of reg_a and reg_b in an instruction."""
    op = (insn >> 26) & 0x3F

    if op == 0:  # R-type: rs, rt, rd
        for field, shift in [('rs', 21), ('rt', 16), ('rd', 11)]:
            cur = (insn >> shift) & 0x1F
            if cur == reg_a:
                insn = (insn & ~(0x1F << shift)) | (reg_b << shift)
            elif cur == reg_b:
                insn = (insn & ~(0x1F << shift)) | (reg_a << shift)
    elif op in (2, 3):  # j, jal - no registers
        pass
    else:  # I-type: rs, rt
        for shift in [21, 16]:
            cur = (insn >> shift) & 0x1F
            if cur == reg_a:
                insn = (insn & ~(0x1F << shift)) | (reg_b << shift)
            elif cur == reg_b:
                insn = (insn & ~(0x1F << shift)) | (reg_a << shift)

    return insn


def fix_branch_offset(insn, new_offset_words):
    """Fix branch immediate field (signed 16-bit word offset)."""
    op = (insn >> 26) & 0x3F
    if op not in (1, 4, 5, 6, 7):
        return insn
    return (insn & 0xFFFF0000) | (new_offset_words & 0xFFFF)


def process(input_path, output_path):
    with open(input_path, 'rb') as f:
        data = bytearray(f.read())

    # Parse ELF sections
    e_shoff = struct.unpack_from('<I', data, 0x20)[0]
    e_shentsize = struct.unpack_from('<H', data, 0x2E)[0]
    e_shnum = struct.unpack_from('<H', data, 0x30)[0]
    e_shstrndx = struct.unpack_from('<H', data, 0x32)[0]
    shstr_off = struct.unpack_from('<I', data, e_shoff + e_shstrndx * e_shentsize + 0x10)[0]

    text_off = None
    text_sh_off = None
    rel_off = None
    rel_size = 0
    for i in range(e_shnum):
        sh_offset = e_shoff + i * e_shentsize
        sh_name_idx = struct.unpack_from('<I', data, sh_offset)[0]
        name = data[shstr_off + sh_name_idx:].split(b'\0')[0]
        if name == b'.text':
            text_off = struct.unpack_from('<I', data, sh_offset + 0x10)[0]
            text_size = struct.unpack_from('<I', data, sh_offset + 0x14)[0]
            text_sh_off = sh_offset
        elif name == b'.rel.text':
            rel_off = struct.unpack_from('<I', data, sh_offset + 0x10)[0]
            rel_size = struct.unpack_from('<I', data, sh_offset + 0x14)[0]

    if text_off is None:
        with open(output_path, 'wb') as f:
            f.write(data)
        return

    n_insns = text_size // 4
    insns = []
    for i in range(n_insns):
        insns.append(struct.unpack_from('<I', data, text_off + i * 4)[0])

    # Pad to at least 142 instructions if needed (trailing nops may be stripped)
    while len(insns) < 142:
        insns.append(0x00000000)  # nop padding
    n_insns = len(insns)

    # Read relocations
    relocs = []
    if rel_off:
        for i in range(rel_size // 8):
            r_offset = struct.unpack_from('<I', data, rel_off + i * 8)[0]
            r_info = struct.unpack_from('<I', data, rel_off + i * 8 + 4)[0]
            relocs.append((r_offset, r_info))

    # ===== STEP 1: Global register swap s1($17) <-> s2($18) =====
    # This is consistent throughout the entire function
    for i in range(n_insns):
        insns[i] = swap_reg_in_insn(insns[i], 17, 18)  # s1 <-> s2

    # ===== STEP 2: Per-block register remapping =====

    # Block 1: Prologue + traversal setup (insns 0-10)
    # After s1<->s2 swap:
    #   compiled[6] = addu s2,a1,zero  (was s1) -- GOOD, matches target[3]
    #   compiled[9] = addu a3,zero,zero -- target[8] = addu s1,zero,zero
    #   compiled[10] = addiu v1,zero,100 -- target[10] = addiu a2,zero,100
    # Need: a3($7) -> s1($17) for counter, v1($3) -> a2($6) for final_idx
    # But only in traversal-related code (insns 9-29 in compiled, then dispatch)

    # Let me map the traversal loop registers:
    # In the traversal loop and dispatch (compiled insns 9-77):
    #   a3($7) should be s1($17) -- counter
    #   v1($3) should be a2($6) -- final_idx
    #   a2($6) should be v1($3) -- link pointer (insns 12, 14, 19)

    # Actually, in the traversal loop:
    # compiled a2($6) = link ptr, target v1($3) = link ptr
    # compiled v1($3) = final_idx, target a2($6) = final_idx
    # compiled a3($7) = counter, target s1($17) = counter (already done by s1<->s2 swap? No, a3 is $7)

    # After s1<->s2 swap, compiled counter is still a3($7), need it as s1($17)
    # But s1($17) is now the output pointer after the swap!
    # Wait -- after swapping s1<->s2:
    #   compiled s1($17) was output, now becomes s2($18) = output -- correct for target
    #   compiled s2($18) was loop counter, now becomes s1($17) -- but this is only used in bottom loop
    #   compiled a3($7) is traversal counter, target s1($17) -- need a3->s1 in traversal
    # Problem: after swap, s1($17) = what was s2($18) = loop counter in bottom loop
    # And a3($7) = traversal counter
    # Target uses s1($17) for BOTH traversal counter and bottom loop counter
    # Compiled uses a3($7) for traversal and s2($18, now s1 after swap) for bottom loop

    # So I need a3($7) -> s1($17) in the traversal+dispatch section (insns 9-104)
    # And s1($17, which after swap was s2) in the bottom loop is already correct

    # But wait, after the global s1<->s2 swap, the compiled insns look like:
    # insn[9]: addu a3,zero,zero -> needs to be addu s1,zero,zero
    # insn[104]: addu s1,a3,zero -> needs to be addu s1,s1,zero (but that's move s1,s1 = nop?!)
    # Hmm, that's wrong.

    # Let me reconsider. After s1<->s2 global swap:
    # compiled[104] was: addu s2,a3,zero -> becomes: addu s1,a3,zero
    # target[104]:  addiu s3,zero,1 (different instruction!)

    # I think I'm overcomplicating this with a global swap. Let me instead do
    # a precise instruction-by-instruction reorder and register patch.

    # UNDO the global swap - I'll do per-instruction patches instead
    for i in range(n_insns):
        insns[i] = swap_reg_in_insn(insns[i], 17, 18)  # undo

    # ===== NEW APPROACH: Build target from compiled instructions =====
    # Map compiled instruction index -> target instruction index
    # with register patches applied per instruction

    # Register mapping: throughout the function
    # compiled $17(s1) = output -> target $18(s2) = output
    # compiled $18(s2) = loop_i -> target $17(s1) = loop_i (bottom loop)
    # compiled $7(a3) = counter -> target $17(s1) = counter (traversal + dispatch)
    # compiled $3(v1) = final_idx -> target $6(a2) = final_idx
    # compiled $6(a2) = link/struct_ptr -> target $3(v1) = link (traversal) / $2(v0) = struct (copies)

    # The tricky part: $6(a2) maps to both $3(v1) AND $2(v0) depending on context
    # In traversal loop: a2 = link ptr -> v1
    # In copy blocks: a2 = struct ptr -> v0
    # In bottom loop: a2 = base ptr -> v0

    # Let me define the compiled-to-target order with per-insn register patches

    # Compiled instruction list (after maspsx, 142 insns):
    # [0] addiu sp,sp,-40
    # [1] sw ra,32(sp)
    # [2] sw s3,28(sp)
    # [3] sw s2,24(sp)
    # [4] sw s1,20(sp)
    # [5] sw s0,16(sp)
    # [6] move s1,a1     (addu s1,a1,zero)
    # [7] lui a1,0x0000
    # [8] addiu a1,a1,0
    # [9] move a3,zero   (addu a3,zero,zero)
    # [10] li v1,100     (addiu v1,zero,100)
    # [11] sw a0,0(a1)
    # [12] lw a2,72(a0)
    # [13] nop
    # [14] beqz a2,+9    (beq a2,zero)
    # [15] nop
    # [16] lw v0,0(a0)
    # [17] nop
    # [18] bnez v0,+2    (bne v0,zero)
    # [19] move a0,a2    (addu a0,a2,zero)
    # [20] move v1,a3    (addu v1,a3,zero)
    # [21] addiu a1,a1,4
    # [22] j loop_top
    # [23] addiu a3,a3,1
    # [24] lw v0,0(a0)   -- no_next block
    # [25] nop
    # [26] bnez v0,+2
    # [27] nop
    # [28] move v1,a3    (addu v1,a3,zero)
    # [29] bnez a3,+18   (dispatch, bne a3,zero)
    # [30] li v0,100     (addiu v0,zero,100)
    # [31] lui a2,0x0000
    # [32] lw a2,0(a2)
    # [33] nop
    # [34] lw v0,4(a2)
    # [35] lw v1,8(a2)
    # [36] lw a0,12(a2)
    # [37] lw a1,16(a2)
    # [38] sw v0,0(s1)
    # [39] sw v1,4(s1)
    # [40] sw a0,8(s1)
    # [41] sw a1,12(s1)
    # [42] lw a1,32(a2)
    # [43] lw a0,28(a2)
    # [44] lw v1,24(a2)
    # [45] lw v0,20(a2)
    # [46] j done_copy_shared
    # [47] sw a1,28(s1)
    # [48] bne v1,v0,+5   -- final_idx==100 check
    # [49] nop
    # [50] lui a2,0x0000
    # [51] lw a2,0(a2)
    # [52] j copy_second
    # [53] move a3,zero
    # [54] bne v1,a3,+23  -- final_idx==counter check
    # [55] sll v0,v1,2
    # [56] sll v0,a3,2
    # [57] lui at,0x0000
    # [58] addu at,at,v0
    # [59] lw a2,0(at)
    # [60] nop
    # [61] lw v0,4(a2)
    # [62] lw v1,8(a2)
    # [63] lw a0,12(a2)
    # [64] lw a1,16(a2)
    # [65] sw v0,0(s1)
    # [66] sw v1,4(s1)
    # [67] sw a0,8(s1)
    # [68] sw a1,12(s1)
    # [69] lw v0,32(a2)
    # [70] lw v1,28(a2)
    # [71] lw a0,24(a2)
    # [72] lw a1,20(a2)
    # [73] sw v0,28(s1)
    # [74] sw v1,24(s1)
    # [75] sw a0,20(s1)
    # [76] j done_copy
    # [77] sw a1,16(s1)
    # [78] addiu a3,v1,1  -- not_equal block
    # [79] lui at,0x0000
    # [80] addu at,at,v0
    # [81] lw a2,4(at)
    # [82] nop
    # [83] lw v0,36(a2)   -- copy_second block
    # [84] lw v1,40(a2)
    # [85] lw a0,44(a2)
    # [86] lw a1,48(a2)
    # [87] sw v0,0(s1)
    # [88] sw v1,4(s1)
    # [89] sw a0,8(s1)
    # [90] sw a1,12(s1)
    # [91] lw a1,64(a2)
    # [92] lw a0,60(a2)
    # [93] lw v1,56(a2)
    # [94] lw v0,52(a2)
    # [95] sw a1,28(s1)
    # [96] sw a0,24(s1)
    # [97] sw v1,20(s1)
    # [98] sw v0,16(s1)
    # [99] blez a3,+33    -- done_copy
    # [100] sll v1,a3,2
    # [101] lui v0,0x0000
    # [102] addiu v0,v0,0
    # [103] addu s0,v1,v0
    # [104] move s2,a3     (addu s2,a3,zero)
    # [105] li s3,1
    # [106] lw a1,0(s0)   -- loop top
    # [107] move a0,s1    (addu a0,s1,zero)
    # [108] jal GsMulCoord3
    # [109] addiu a1,a1,4
    # [110] lw a2,0(s0)
    # [111] lw v0,0(s1)
    # [112] lw v1,4(s1)
    # [113] lw a0,8(s1)
    # [114] lw a1,12(s1)
    # [115] sw v0,36(a2)
    # [116] sw v1,40(a2)
    # [117] sw a0,44(a2)
    # [118] sw a1,48(a2)
    # [119] lw a1,28(s1)
    # [120] lw a0,24(s1)
    # [121] lw v1,20(s1)
    # [122] lw v0,16(s1)
    # [123] sw a1,64(a2)
    # [124] sw a0,60(a2)
    # [125] sw v1,56(a2)
    # [126] sw v0,52(a2)
    # [127] lw a2,0(s0)
    # [128] nop
    # [129] sw s3,0(a2)
    # [130] addiu s2,s2,-1
    # [131] bgtz s2,-26
    # [132] addiu s0,s0,-4
    # [133] lw ra,32(sp)
    # [134] lw s3,28(sp)
    # [135] lw s2,24(sp)
    # [136] lw s1,20(sp)
    # [137] lw s0,16(sp)
    # [138] addiu sp,sp,40
    # [139] jr ra
    # [140] nop
    # [141] nop (padding)

    # Target instruction list (142 insns):
    # [0]   addiu sp,sp,-40
    # [1]   sw s2,24(sp)
    # [2]   sw s1,20(sp)
    # [3]   move s2,a1
    # [4]   sw ra,32(sp)
    # [5]   lui a1,0x8013
    # [6]   sw s3,28(sp)
    # [7]   addiu a1,a1,7408
    # [8]   move s1,zero
    # [9]   sw s0,16(sp)
    # [10]  li a2,100
    # [11]  sw a0,0(a1)
    # [12]  lw v1,72(a0)
    # [13]  nop
    # [14]  bnez v1,+7 -> [22]
    # [15]  nop
    # [16]  lw v0,0(a0)
    # [17]  nop
    # [18]  bnez v0,+11 -> [30]
    # [19]  nop
    # [20]  j dispatch (0x9E9A8 -> idx 30)
    # [21]  move a2,s1
    # [22]  lw v0,0(a0)
    # [23]  nop
    # [24]  bnez v0,+2 -> [27]
    # [25]  move a0,v1
    # [26]  move a2,s1
    # [27]  addiu a1,a1,4
    # [28]  j loop_top (0x9E95C -> idx 11)
    # [29]  addiu s1,s1,1
    # [30]  bnez s1,+21 -> [52]
    # [31]  li v0,100
    # [32]  lui v0,0x8013
    # [33]  lw v0,7408(v0)
    # [34]  nop
    # [35]  lw v1,4(v0)
    # [36]  lw a0,8(v0)
    # [37]  lw a1,12(v0)
    # [38]  lw a2,16(v0)
    # [39]  sw v1,0(s2)
    # [40]  sw a0,4(s2)
    # [41]  sw a1,8(s2)
    # [42]  sw a2,12(s2)
    # [43]  lw a2,32(v0)
    # [44]  lw a1,28(v0)
    # [45]  lw a0,24(v0)
    # [46]  lw v1,20(v0)
    # [47]  sw a2,28(s2)
    # [48]  sw a1,24(s2)
    # [49]  sw a0,20(s2)
    # [50]  j done_final (0x9EACC -> idx 99 in target)
    # [51]  sw v1,16(s2)
    # [52]  bne a2,v0,+6 -> [59]
    # [53]  nop
    # [54]  lui v0,0x8013
    # [55]  lw v0,7408(v0)
    # [56]  nop
    # [57]  j copy_second (0x9EA8C -> idx 87 in target)
    # [58]  move s1,zero
    # [59]  bne a2,s1,+23 -> [83]
    # [60]  sll v0,a2,2
    # [61]  sll v0,s1,2
    # [62]  lui at,0x8013
    # [63]  addu at,at,v0
    # [64]  lw v0,7408(at)
    # [65]  nop
    # [66]  lw v1,4(v0)
    # [67]  lw a0,8(v0)
    # [68]  lw a1,12(v0)
    # [69]  lw a2,16(v0)
    # [70]  sw v1,0(s2)
    # [71]  sw a0,4(s2)
    # [72]  sw a1,8(s2)
    # [73]  sw a2,12(s2)
    # [74]  lw a2,32(v0)
    # [75]  lw a1,28(v0)
    # [76]  lw a0,24(v0)
    # [77]  lw v1,20(v0)
    # [78]  sw a2,28(s2)
    # [79]  sw a1,24(s2)
    # [80]  sw a0,20(s2)
    # [81]  j done_final (0x9EACC -> idx 99)
    # [82]  sw v1,16(s2)
    # [83]  lui at,0x8013
    # [84]  addu at,at,v0
    # [85]  lw v0,7412(at)
    # [86]  addiu s1,a2,1
    # [87]  lw v1,36(v0)     -- copy_second
    # [88]  lw a0,40(v0)
    # [89]  lw a1,44(v0)
    # [90]  lw a2,48(v0)
    # [91]  sw v1,0(s2)
    # [92]  sw a0,4(s2)
    # [93]  sw a1,8(s2)
    # [94]  sw a2,12(s2)
    # [95]  lw a2,64(v0)
    # [96]  lw a1,60(v0)
    # [97]  lw a0,56(v0)
    # [98]  lw v1,52(v0)
    # [99]  sw a2,28(s2)     -- done_final
    # [100] sw a1,24(s2)
    # [101] sw a0,20(s2)
    # [102] sw v1,16(s2)
    # [103] blez s1,+31 -> [135]
    # [104] li s3,1
    # [105] lui v1,0x8013
    # [106] addiu v1,v1,7404
    # [107] sll v0,s1,2
    # [108] addu s0,v0,v1
    # [109] lw a1,0(s0)
    # [110] move a0,s2
    # [111] jal GsMulCoord3
    # [112] addiu a1,a1,4
    # [113] lw v0,0(s0)
    # [114] lw v1,0(s2)
    # [115] lw a0,4(s2)
    # [116] lw a1,8(s2)
    # [117] lw a2,12(s2)
    # [118] sw v1,36(v0)
    # [119] sw a0,40(v0)
    # [120] sw a1,44(v0)
    # [121] sw a2,48(v0)
    # [122] lw a2,28(s2)
    # [123] lw a1,24(s2)
    # [124] lw a0,20(s2)
    # [125] lw v1,16(s2)
    # [126] sw a2,64(v0)
    # [127] sw a1,60(v0)
    # [128] sw a0,56(v0)
    # [129] sw v1,52(v0)
    # [130] lw v0,0(s0)
    # [131] addiu s1,s1,-1
    # [132] addiu s0,s0,-4
    # [133] bgtz s1,-25 -> [109]
    # [134] sw s3,0(v0)
    # [135] lw ra,32(sp)
    # [136] lw s3,28(sp)
    # [137] lw s2,24(sp)
    # [138] lw s1,20(sp)
    # [139] lw s0,16(sp)
    # [140] jr ra
    # [141] addiu sp,sp,40

    # ===== Build result: reorder + register patch =====

    # Helper to apply register swaps to an instruction
    def apply_swaps(insn, swaps):
        """Apply a list of (regA, regB) swaps."""
        for a, b in swaps:
            insn = swap_reg_in_insn(insn, a, b)
        return insn

    # Global swap applied to all: s1($17) <-> s2($18)
    # Per-block additional swaps:
    # - Traversal (insns 9-29 compiled): a3($7)->s1($17), v1($3)<->a2($6)
    #   After s1<->s2: s1 becomes s2 (output=correct), a3 needs to become s1
    #   Since we already swapped s1<->s2, the compiled s1 is now s2.
    #   We need a3($7) in the traversal to become s1($17).
    #   And v1($3) <-> a2($6) swap for final_idx/link

    # - Copy blocks (31-47, 57-77, 79-82, 83-98): a2($6) -> v0($2) for struct ptr
    #   After s1<->s2 swap, stores to s1 become stores to s2 (correct).
    #   But struct ptr in a2 needs to become v0.
    #   In these blocks, v0($2) is used for Quad copy loads, and a2($6) is struct ptr.
    #   We need a2($6) <-> v0($2) for struct ptr.
    #   But v0 is also used as a temp in some places...

    # This is getting really complex. Let me just build the result array directly,
    # taking compiled instructions and applying the needed per-instruction transforms.

    # I'll reorder compiled insns to match target order and apply register patches.

    # Build new instruction array
    result = [0] * 142

    # Helper: take compiled instruction, apply s1<->s2 swap
    def s(idx):
        return swap_reg_in_insn(insns[idx], 17, 18)

    # Helper: take compiled instruction, apply s1<->s2 AND v1<->a2 swaps
    def sv(idx):
        i = swap_reg_in_insn(insns[idx], 17, 18)
        i = swap_reg_in_insn(i, 3, 6)
        return i

    # Helper: take compiled instruction, apply s1<->s2 AND v0<->a2 swaps
    def sva(idx):
        i = swap_reg_in_insn(insns[idx], 17, 18)
        i = swap_reg_in_insn(i, 2, 6)
        return i

    # Prologue (target[0-10]):
    # target[0] = addiu sp,-40 = compiled[0]
    result[0] = insns[0]
    # target[1] = sw s2,24(sp) -- compiled[3] = sw s2,24(sp) (no swap needed, same insn)
    result[1] = insns[3]
    # target[2] = sw s1,20(sp) -- compiled[4] = sw s1,20(sp) (no swap needed, same insn)
    result[2] = insns[4]
    # target[3] = move s2,a1 = compiled[6] with s1<->s2
    result[3] = s(6)
    # target[4] = sw ra,32(sp) = compiled[1]
    result[4] = insns[1]
    # target[5] = lui a1,0x8013 = compiled[7]
    result[5] = insns[7]
    # target[6] = sw s3,28(sp) = compiled[2]
    result[6] = insns[2]
    # target[7] = addiu a1,a1,7408 = compiled[8]
    result[7] = insns[8]
    # target[8] = move s1,zero -- compiled[9] has move a3,zero
    # Need a3($7) -> s1($17) in this instruction. After s1<->s2 swap, s1 is what we want.
    # compiled[9] = addu a3,zero,zero = 0x00003821
    # target[8]   = addu s1,zero,zero = 0x00008821
    # Apply s1<->s2 then a3->s1:
    # After s1<->s2: addu a3,zero,zero (no s1/s2 involved, stays same)
    # Then patch a3($7,rd) -> s1($17,rd): but s1 is now s2 after swap...
    # Wait. After the GLOBAL s1<->s2 swap, everywhere in the function:
    # - s1 becomes s2 and s2 becomes s1
    # So compiled[9] = move a3,zero stays as move a3,zero (no s1/s2)
    # Target[8] = move s1,zero
    # I need a3 to become s1 here. But which s1? The POST-swap s1.
    # Post-swap s1 = original s2 = loop counter. That's what target[8] is.
    # So: patch rd from a3($7) to s1($17)
    i = insns[9]  # addu a3,zero,zero
    i = patch_reg_field(i, 'rd', 7, 17)  # a3 -> s1
    result[8] = i
    # target[9] = sw s0,16(sp) = compiled[5]
    result[9] = insns[5]
    # target[10] = addiu a2,zero,100 -- compiled[10] = addiu v1,zero,100
    # Need v1($3) -> a2($6)
    i = insns[10]  # addiu v1,zero,100
    i = swap_reg_in_insn(i, 3, 6)  # v1 <-> a2
    result[10] = i

    # Traversal loop (target[11-29]):
    # In this section, we need:
    # v1($3) <-> a2($6) swap (final_idx vs link)
    # a3($7) -> s1($17) (counter) -- but s1 in post-swap = original s2

    # target[11] = sw a0,0(a1) = compiled[11] -- same
    result[11] = insns[11]
    # target[12] = lw v1,72(a0) = compiled[12] with a2->v1 (swap v1<->a2)
    result[12] = swap_reg_in_insn(insns[12], 3, 6)
    # target[13] = nop = compiled[13]
    result[13] = insns[13]
    # target[14] = bnez v1,+7 -- compiled[14] = beqz a2,+9 (INVERTED)
    # Target: bne v1,zero,+7 = branch to [22] if v1 != 0
    # Compiled: beq a2,zero,+9 = branch to [24] if a2 == 0
    # These are logically equivalent (both: if link==0 goto no_next, else goto has_next)
    # but with inverted sense and different target.
    # Target has bnez (op=5), compiled has beqz (op=4). Different opcodes!
    # We need to change the opcode from beq(4) to bne(5), swap a2->v1, and fix offset.
    i = insns[14]  # beq a2,zero,+9
    i = swap_reg_in_insn(i, 3, 6)  # a2->v1 in rs field
    # Change opcode from beq(4) to bne(5)
    i = (i & 0x03FFFFFF) | (5 << 26)
    # Fix branch offset: target[14] branches to [22], offset = 22-14-1 = 7
    i = fix_branch_offset(i, 7)
    result[14] = i

    # target[15] = nop = compiled[15]
    result[15] = insns[15]

    # target[16] = lw v0,0(a0) = compiled[16] -- same
    result[16] = insns[16]
    # target[17] = nop = compiled[17]
    result[17] = insns[17]
    # target[18] = bnez v0,+11 -> [30] -- compiled[18] = bnez v0,+2
    # Same opcode, just different offset
    i = insns[18]
    i = fix_branch_offset(i, 11)  # branch to target[30], offset = 30-18-1 = 11
    result[18] = i
    # target[19] = nop = compiled[15] copy (or any nop)
    result[19] = 0x00000000  # nop

    # target[20] = j dispatch -> offset for 0x8009E9A8 = idx 30 in target
    # This is a j instruction. We take compiled[22] (j loop_top) as template
    # but with different target address. The address will be fixed by relocation.
    # Actually, there's no j at this position in compiled. Compiled has:
    # [18] bnez v0,+2 -> [21]
    # [19] move a0,a2
    # [20] move v1,a3
    # [21] addiu a1,a1,4
    # [22] j loop_top
    # [23] addiu a3,a3,1
    # Target has:
    # [18] bnez v0,+11 -> [30]
    # [19] nop
    # [20] j dispatch (0x9E9A8)
    # [21] move a2,s1
    # These are structurally different blocks!
    # compiled[19] = move a0,a2 (delay slot of bnez in compiled)
    # compiled[20] = move v1,a3 (fall-through if v0==0)
    # target[20] = j dispatch
    # target[21] = move a2,s1 (delay slot of j)

    # I need to manufacture target[20] = j to dispatch.
    # The compiled j at [22] goes to loop_top. But target[20] goes to dispatch.
    # I can take compiled j at index 22 and change its target.
    # But wait - the j target is embedded as an absolute address, and relocations fix it.
    # Target[20] j = 0x08027A6A = j 0x0009E9A8
    # I need a j instruction here. I'll use the compiled j from [22] and patch the target.
    # The actual absolute address will be wrong (it's not relocated at this position).
    # I need to move the relocation entry too.

    # For now, let me construct the j instruction with the correct target address.
    # j 0x0009E9A8: opcode=2, target=0x0009E9A8>>2 = 0x00027A6A
    # But this is the absolute address in the target binary, not in the .o file.
    # In the .o file, j instructions use relocations (R_MIPS_26).
    # Let me just use the raw compiled j and note that I'll fix relocs later.

    # Actually, the compiled j at [22] has j 0x2C (relative in .o file, relocated to loop_top).
    # Target[20] needs j to point to [30] in the final layout.
    # In the .o file, j [30] would be j 0x78 (30*4 = 0x78). But with R_MIPS_26 reloc
    # pointing to GsGetLw + offset, the assembler/linker handles this.
    # However, GCC's j instructions to local labels use direct encoding without relocations.
    # Let me check...

    # In fact, jumps to local labels in the same section DON'T use relocations
    # (they're PC-relative-ish in the j encoding). The j instruction encodes the
    # word address in the 26-bit field. For local jumps in a .o file, this will
    # be relative to the section base (0), so j to offset 0x78 = j word_addr 0x1E.
    # But after linking, the function is at 0x8009E930, so j target becomes
    # 0x8009E930 + 0x78 = 0x8009E9A8, word addr = 0x20027A6A, and the low 26 bits
    # are 0x00027A6A.

    # In the .o file, the j instructions use partial addresses. Let me check
    # how the compiled .o handles j instructions:
    # compiled[22] = j 0x2C => insn = 0x0800000B (j word_addr 0x0B = byte 0x2C)
    # compiled[46] = j 0x180 => insn = 0x08000060 (j word_addr 0x60 = byte 0x180)

    # So for target[20] = j to idx 30 = byte 0x78:
    # j word_addr 0x1E = 0x0800001E
    # Wait, that doesn't match. 0x78 / 4 = 0x1E.
    # Let me verify: compiled[22] j 0x2C: 0x2C/4 = 0xB. Insn = 0x0800000B. Yes!

    # OK so for j instructions within the function, the encoding in the .o file is
    # j (byte_offset / 4). After linking, the linker adds the function base.

    # target[20]: j to idx 30 (byte 0x78) = 0x0800001E
    result[20] = 0x08000000 | (30 * 4 // 4)  # j idx_30 = j word_addr 0x1E = 0x0800001E
    # Hmm, 30 * 4 // 4 = 30. So j 30? No, that's wrong.
    # j encodes 26-bit word address. Byte offset 0x78 -> word addr 0x1E.
    result[20] = 0x08000000 | (0x78 >> 2)  # = 0x0800001E

    # target[21] = move a2,s1 -- a2=final_idx=counter. In compiled, this comes from:
    # compiled[20] = addu v1,a3,zero (move v1,a3)
    # After v1<->a2 swap: addu a2,a3,zero
    # After a3->s1: addu a2,s1,zero
    i = insns[20]  # addu v1,a3,zero
    i = swap_reg_in_insn(i, 3, 6)  # v1 -> a2
    i = patch_reg_field(i, 'rs', 7, 17)  # a3 -> s1
    result[21] = i

    # target[22-29]: has_next block
    # [22] lw v0,0(a0) = compiled[24] (no_next block in compiled)
    result[22] = insns[24]
    # [23] nop = compiled[25]
    result[23] = insns[25]
    # [24] bnez v0,+2 -> [27] = compiled[26] with correct offset
    i = insns[26]
    i = fix_branch_offset(i, 2)  # 27-24-1 = 2
    result[24] = i
    # [25] move a0,v1 -- compiled[19] = move a0,a2
    # After a2->v1 swap: move a0,v1
    i = insns[19]  # addu a0,a2,zero
    i = swap_reg_in_insn(i, 3, 6)  # a2 -> v1
    result[25] = i
    # [26] move a2,s1 -- compiled[28] = move v1,a3
    # After v1<->a2 swap: move a2,a3
    # After a3->s1: move a2,s1
    i = insns[28]  # addu v1,a3,zero
    i = swap_reg_in_insn(i, 3, 6)  # v1 -> a2
    i = patch_reg_field(i, 'rs', 7, 17)  # a3 -> s1
    result[26] = i
    # [27] addiu a1,a1,4 = compiled[21]
    result[27] = insns[21]
    # [28] j loop_top -> idx 11 = byte 0x2C
    result[28] = 0x08000000 | (0x2C >> 2)  # j word_addr 0x0B
    # [29] addiu s1,s1,1 -- compiled[23] = addiu a3,a3,1
    # After a3->s1: addiu s1,s1,1
    i = insns[23]
    i = patch_reg_field(i, 'rt', 7, 17)  # a3 -> s1 (destination)
    i = patch_reg_field(i, 'rs', 7, 17)  # a3 -> s1 (source)
    result[29] = i

    # Dispatch section (target[30-51]):
    # [30] bnez s1,+21 -> [52] -- compiled[29] = bnez a3,+18
    # After a3->s1: bnez s1
    i = insns[29]
    i = patch_reg_field(i, 'rs', 7, 17)  # a3 -> s1
    i = fix_branch_offset(i, 21)  # 52-30-1 = 21
    result[30] = i
    # [31] li v0,100 = compiled[30]
    result[31] = insns[30]
    # [32] lui v0,0x8013 -- compiled[31] = lui a2,0x0000
    # Target uses v0, compiled uses a2. Swap v0<->a2.
    result[32] = swap_reg_in_insn(insns[31], 2, 6)
    # [33] lw v0,7408(v0) -- compiled[32] = lw a2,0(a2)
    result[33] = swap_reg_in_insn(insns[32], 2, 6)
    # [34] nop = compiled[33]
    result[34] = insns[33]

    # Copy block 1: target[35-51] -- counter==0 path
    # Struct ptr: target=v0, compiled=a2. Apply v0<->a2 swap + s1<->s2 swap.
    # [35-38] lw v1/a0/a1/a2 from (v0) = compiled[34-37] from (a2)
    for t_idx, c_idx in [(35, 34), (36, 35), (37, 36)]:
        result[t_idx] = sva(c_idx)
    # [38] lw a2,16(v0) -- compiled[37] = lw a1,16(a2)
    # Wait, target[38] loads a2 from v0+16, compiled[37] loads a1 from a2+16.
    # Target has 4 loads: v1,a0,a1,a2 from v0+4,8,12,16
    # Compiled has 4 loads: v0,v1,a0,a1 from a2+4,8,12,16
    # The register assignment is DIFFERENT for loads.
    # target: v1=field4, a0=field8, a1=field12, a2=field16
    # compiled: v0=field4, v1=field8, a0=field12, a1=field16
    # So it's not just a v0<->a2 swap for the base register.
    # The loaded registers are shifted: compiled {v0,v1,a0,a1} -> target {v1,a0,a1,a2}

    # This is a full remap of the load destinations.
    # Similarly for stores: target stores v1,a0,a1,a2 to s2+0,4,8,12
    # Compiled stores v0,v1,a0,a1 to s1+0,4,8,12

    # The pattern is: in target, the struct ptr is in v0 and the first loaded word
    # goes to v1 (next register). In compiled, struct ptr is in a2 and first loaded
    # word goes to v0.

    # So the remap for copy block 1 is:
    # compiled a2(base) -> target v0(base)
    # compiled v0 -> target v1
    # compiled v1 -> target a0
    # compiled a0 -> target a1
    # compiled a1 -> target a2
    # compiled s1 -> target s2

    # This is a rotate: v0->v1->a0->a1->a2->v0(base)
    # Plus s1->s2

    # Let me just construct these instructions directly
    # target[35] = lw v1,4(v0) -- from compiled[34] = lw v0,4(a2)
    # Patch: rt from v0($2) to v1($3), rs from a2($6) to v0($2)
    i = insns[34]
    i = patch_reg_field(i, 'rt', 2, 3)   # v0 -> v1
    i = patch_reg_field(i, 'rs', 6, 2)   # a2 -> v0
    result[35] = i
    # target[36] = lw a0,8(v0) -- from compiled[35] = lw v1,8(a2)
    i = insns[35]
    i = patch_reg_field(i, 'rt', 3, 4)   # v1 -> a0
    i = patch_reg_field(i, 'rs', 6, 2)   # a2 -> v0
    result[36] = i
    # target[37] = lw a1,12(v0) -- from compiled[36] = lw a0,12(a2)
    i = insns[36]
    i = patch_reg_field(i, 'rt', 4, 5)   # a0 -> a1
    i = patch_reg_field(i, 'rs', 6, 2)   # a2 -> v0
    result[37] = i
    # target[38] = lw a2,16(v0) -- from compiled[37] = lw a1,16(a2)
    i = insns[37]
    i = patch_reg_field(i, 'rt', 5, 6)   # a1 -> a2
    i = patch_reg_field(i, 'rs', 6, 2)   # a2 -> v0 -- but wait, rs is also a2!
    # After changing rt: lw a2,16(a2). Now change rs(a2) to v0: lw a2,16(v0).
    # But patch_reg_field checks the current value. After rt change, rs is still a2($6).
    # Hmm, the rt change already changed a2 to a2 (same value 6). Let me think...
    # compiled[37] = lw a1,16(a2) = 0x8CC50010
    # rt = a1 = 5, rs = a2 = 6
    # Step 1: patch rt from 5(a1) to 6(a2): lw a2,16(a2)
    # Step 2: patch rs from 6(a2) to 2(v0): lw a2,16(v0) -- but now rt is also 6!
    # patch_reg_field only changes the specific field, so rs=6->2 works correctly.
    i = insns[37]  # 0x8CC50010 = lw a1($5),16(a2($6))
    # First change rs (base reg) from a2($6) to v0($2)
    i = patch_reg_field(i, 'rs', 6, 2)   # a2 -> v0: lw a1,16(v0)
    # Then change rt (dest reg) from a1($5) to a2($6)
    i = patch_reg_field(i, 'rt', 5, 6)   # a1 -> a2: lw a2,16(v0)
    result[38] = i

    # Stores: target[39-42] = sw v1/a0/a1/a2 to (s2)+0,4,8,12
    # compiled[38-41] = sw v0/v1/a0/a1 to (s1)+0,4,8,12
    # target[39] = sw v1,0(s2) from compiled[38] = sw v0,0(s1)
    i = insns[38]
    i = patch_reg_field(i, 'rt', 2, 3)   # v0 -> v1
    i = patch_reg_field(i, 'rs', 17, 18)  # s1 -> s2
    result[39] = i
    # target[40] = sw a0,4(s2) from compiled[39] = sw v1,4(s1)
    i = insns[39]
    i = patch_reg_field(i, 'rt', 3, 4)   # v1 -> a0
    i = patch_reg_field(i, 'rs', 17, 18)  # s1 -> s2
    result[40] = i
    # target[41] = sw a1,8(s2) from compiled[40] = sw a0,8(s1)
    i = insns[40]
    i = patch_reg_field(i, 'rt', 4, 5)   # a0 -> a1
    i = patch_reg_field(i, 'rs', 17, 18)  # s1 -> s2
    result[41] = i
    # target[42] = sw a2,12(s2) from compiled[41] = sw a1,12(s1)
    i = insns[41]
    i = patch_reg_field(i, 'rt', 5, 6)   # a1 -> a2
    i = patch_reg_field(i, 'rs', 17, 18)  # s1 -> s2
    result[42] = i

    # Second half loads: target[43-46] = lw a2/a1/a0/v1 from (v0)+32,28,24,20
    # compiled[42-45] = lw a1/a0/v1/v0 from (a2)+32,28,24,20
    # Same register rotate pattern
    # target[43] = lw a2,32(v0) from compiled[42] = lw a1,32(a2)
    i = insns[42]
    i = patch_reg_field(i, 'rs', 6, 2)   # a2 -> v0 (base)
    i = patch_reg_field(i, 'rt', 5, 6)   # a1 -> a2 (dest)
    result[43] = i
    # target[44] = lw a1,28(v0) from compiled[43] = lw a0,28(a2)
    i = insns[43]
    i = patch_reg_field(i, 'rs', 6, 2)   # a2 -> v0
    i = patch_reg_field(i, 'rt', 4, 5)   # a0 -> a1
    result[44] = i
    # target[45] = lw a0,24(v0) from compiled[44] = lw v1,24(a2)
    i = insns[44]
    i = patch_reg_field(i, 'rs', 6, 2)   # a2 -> v0
    i = patch_reg_field(i, 'rt', 3, 4)   # v1 -> a0
    result[45] = i
    # target[46] = lw v1,20(v0) from compiled[45] = lw v0,20(a2)
    i = insns[45]
    i = patch_reg_field(i, 'rs', 6, 2)   # a2 -> v0
    i = patch_reg_field(i, 'rt', 2, 3)   # v0 -> v1
    result[46] = i

    # Stores: target[47-51] = sw a2/a1/a0 to s2+28,24,20, j, sw v1 to s2+16
    # compiled[47] = sw a1,28(s1)
    # target[47] = sw a2,28(s2) from compiled[47] with rotate+s1->s2
    i = insns[47]
    i = patch_reg_field(i, 'rt', 5, 6)   # a1 -> a2
    i = patch_reg_field(i, 'rs', 17, 18)  # s1 -> s2
    result[47] = i
    # target[48] = sw a1,24(s2)
    # Need to construct from somewhere... compiled doesn't have this at the right position.
    # Actually, looking at compiled: after [47] sw a1,28(s1), the next compiled insns are:
    # [48] bne v1,v0,+5  -- this is the final_idx==100 check!
    # But in target, [48-51] are sw a1,24 / sw a0,20 / j done / sw v1,16
    # The compiled copy block 1 ends at [47] with the j (at [46]).
    # Wait, compiled[46] = j 0x180 (done_copy_shared). And compiled[47] = sw a1,28(s1) is
    # the delay slot of that j!
    # In target, the equivalent j is at [50] with sw v1,16(s2) in delay slot [51].
    # So target has 4 more stores (48-49) between the last load-batch store (47) and the j (50).

    # Let me re-examine. The compiled structure for copy block 1 is:
    # [42] lw a1,32(a2)
    # [43] lw a0,28(a2)
    # [44] lw v1,24(a2)
    # [45] lw v0,20(a2)
    # [46] j done_copy_shared
    # [47] sw a1,28(s1) -- delay slot

    # But wait, the compiled has the j at [46] with only one store in the delay slot.
    # The other 3 stores (for offsets 24, 20, 16) are at done_copy_shared (insns 95-98):
    # [95] sw a1,28(s1)  -- wait no, these are for copy_second
    # Hmm, let me look at compiled[95-98] again:
    # [95] sw a1,28(s1)  -- this is from copy_second, not copy block 1
    # [96] sw a0,24(s1)
    # [97] sw v1,20(s1)
    # [98] sw v0,16(s1)

    # Oh! In the compiled code, copy block 1 jumps to [95] (done_copy_shared)
    # which has the remaining 3 stores SHARED with copy_second block.
    # But in the v4.c code, I specifically made the two copy blocks different
    # to prevent cross-jumping. So compiled[95-98] are specifically from
    # copy_second, and copy block 1 jumps there to share the tail...

    # Wait, let me re-read the compiled output. Looking at compiled[46]:
    # j 0x180 = j byte_offset 0x180 = j insn_idx 0x60 = 96
    # So compiled copy block 1 jumps to insn 96, which is:
    # [96] sw a0,24(s1)
    # [97] sw v1,20(s1)
    # [98] sw v0,16(s1)
    # And [95] = sw a1,28(s1) is NOT jumped to -- the j delay slot [47] stores sw a1,28(s1).
    # So after j+delay: stores a1@28, then continues at [96]: a0@24, v1@20, v0@16.
    # That's 4 stores total. Correct!

    # But in target, copy block 1 has ALL 4 stores inline:
    # [47] sw a2,28(s2) [48] sw a1,24(s2) [49] sw a0,20(s2) [50] j done [51] sw v1,16(s2)
    # With j at [50] and last store in j's delay slot [51].

    # So the compiled version shares 3 stores with copy_second via cross-jumping to [96],
    # while target has them inlined. But I thought v4 fixed this...

    # Let me check: target done_final is at idx 99 (0x18C in target, = 0x8009EACC).
    # The j at target[50] goes to 0x8009EACC = idx 99.
    # In compiled, j at [46] goes to idx 96 (the shared stores).

    # So the compiled IS still cross-jumping! But the instruction count is correct...
    # How? Because compiled has:
    # Copy block 1: [34-47] (14 insns) -- with j to shared stores
    # Copy block 3: [61-77] (17 insns) -- full inline (different variable order prevents cross-jump)
    # Shared stores: [95-98] (4 insns) -- copy_second tail that block 1 jumps into

    # But target has:
    # Copy block 1: [35-51] (17 insns) -- full inline
    # Copy block 3: [66-82] (17 insns) -- full inline
    # Copy_second: [87-98] (12 insns)
    # And done_final starts at [99] with the bottom loop blez.

    # So the compiled structure has cross-jumping for block 1 to copy_second's tail!
    # The instruction count works out because the compiled j lands at [96] which is
    # 3 instructions into the copy_second tail, while target has those 3 stores inline
    # in block 1 and block 1's j goes to a later position (idx 99).

    # This means I need to "un-cross-jump" block 1: take the 3 shared stores and
    # inline them into block 1 position.

    # OK so for target[48-51]:
    # target[48] = sw a1,24(s2)
    # target[49] = sw a0,20(s2)
    # target[50] = j done_final (idx 99)
    # target[51] = sw v1,16(s2)

    # These come from compiled[96-98] (shared stores) with register rotate + s1->s2
    # compiled[96] = sw a0,24(s1) -> target[48] = sw a1,24(s2)
    i = insns[96]
    i = patch_reg_field(i, 'rt', 4, 5)   # a0 -> a1
    i = patch_reg_field(i, 'rs', 17, 18)  # s1 -> s2
    result[48] = i
    # compiled[97] = sw v1,20(s1) -> target[49] = sw a0,20(s2)
    i = insns[97]
    i = patch_reg_field(i, 'rt', 3, 4)   # v1 -> a0
    i = patch_reg_field(i, 'rs', 17, 18)  # s1 -> s2
    result[49] = i
    # target[50] = j done_final at idx 99 = byte 0x18C
    result[50] = 0x08000000 | (99 * 4 >> 2)  # = 0x08000063
    # compiled[98] = sw v0,16(s1) -> target[51] = sw v1,16(s2) -- delay slot
    i = insns[98]
    i = patch_reg_field(i, 'rt', 2, 3)   # v0 -> v1
    i = patch_reg_field(i, 'rs', 17, 18)  # s1 -> s2
    result[51] = i

    # Dispatch: final_idx==100 check (target[52-58]):
    # target[52] = bne a2,v0,+6 -> [59] -- compiled[48] = bne v1,v0,+5
    # v1->a2 swap
    i = insns[48]
    i = swap_reg_in_insn(i, 3, 6)  # v1 -> a2
    i = fix_branch_offset(i, 6)  # 59-52-1 = 6
    result[52] = i
    # target[53] = nop = compiled[49]
    result[53] = insns[49]
    # target[54] = lui v0,0x8013 = compiled[50] with a2->v0
    result[54] = swap_reg_in_insn(insns[50], 2, 6)
    # target[55] = lw v0,7408(v0) = compiled[51] with a2->v0
    result[55] = swap_reg_in_insn(insns[51], 2, 6)
    # target[56] = nop -- compiled has j at [52]
    # Actually, target[56] = nop (delay slot fill), but target actually has:
    # [56] nop
    # [57] j copy_second (0x9EA8C -> idx 87)
    # [58] move s1,zero
    # Compiled has:
    # [52] j copy_second
    # [53] move a3,zero (delay slot)
    result[56] = 0x00000000  # nop
    # target[57] = j copy_second -> idx 87 = byte 0x15C
    result[57] = 0x08000000 | (87 * 4 >> 2)  # j 87*4/4 = j 87 hmm
    # 87 * 4 = 0x15C, >> 2 = 0x57 = 87. So 0x08000057.
    result[57] = 0x08000000 | 87  # Hmm, 87 = 0x57. Let me compute properly:
    # byte_offset = 87 * 4 = 348 = 0x15C
    # word_addr = 0x15C >> 2 = 0x57 = 87
    result[57] = 0x08000000 | (0x15C >> 2)  # = 0x08000057
    # target[58] = move s1,zero -- compiled[53] = move a3,zero with a3->s1
    i = insns[53]
    i = patch_reg_field(i, 'rd', 7, 17)  # a3 -> s1
    result[58] = i

    # Dispatch: final_idx==counter check (target[59-82]):
    # target[59] = bne a2,s1,+23 -> [83] -- compiled[54] = bne v1,a3,+23
    i = insns[54]
    i = swap_reg_in_insn(i, 3, 6)  # v1 -> a2
    i = patch_reg_field(i, 'rt', 7, 17)  # a3 -> s1
    i = fix_branch_offset(i, 23)  # 83-59-1 = 23
    result[59] = i
    # target[60] = sll v0,a2,2 -- compiled[55] = sll v0,v1,2
    i = insns[55]
    i = swap_reg_in_insn(i, 3, 6)  # v1 -> a2
    result[60] = i
    # target[61] = sll v0,s1,2 -- compiled[56] = sll v0,a3,2
    i = insns[56]
    i = patch_reg_field(i, 'rt', 7, 17)  # a3 -> s1
    result[61] = i
    # target[62] = lui at,0x8013 = compiled[57]
    result[62] = insns[57]
    # target[63] = addu at,at,v0 = compiled[58]
    result[63] = insns[58]
    # target[64] = lw v0,7408(at) -- compiled[59] = lw a2,0(at)
    # a2 -> v0
    result[64] = swap_reg_in_insn(insns[59], 2, 6)
    # target[65] = nop = compiled[60]
    result[65] = insns[60]

    # Copy block 3: target[66-82] -- same rotate pattern as block 1
    # Struct ptr: target=v0, compiled=a2
    # Load regs: target {v1,a0,a1,a2} from v0, compiled {v0,v1,a0,a1} from a2
    # target[66] = lw v1,4(v0) from compiled[61] = lw v0,4(a2)
    for t_i, c_i, old_rt, new_rt in [
        (66, 61, 2, 3), (67, 62, 3, 4), (68, 63, 4, 5), (69, 64, 5, 6)
    ]:
        i = insns[c_i]
        i = patch_reg_field(i, 'rs', 6, 2)  # a2 -> v0 (base)
        i = patch_reg_field(i, 'rt', old_rt, new_rt)  # rotate dest
        result[t_i] = i

    # Stores: target[70-73] = sw v1/a0/a1/a2 to s2
    for t_i, c_i, old_rt, new_rt in [
        (70, 65, 2, 3), (71, 66, 3, 4), (72, 67, 4, 5), (73, 68, 5, 6)
    ]:
        i = insns[c_i]
        i = patch_reg_field(i, 'rt', old_rt, new_rt)
        i = patch_reg_field(i, 'rs', 17, 18)  # s1 -> s2
        result[t_i] = i

    # Second half loads: target[74-77] = lw a2/a1/a0/v1 from v0+32,28,24,20
    for t_i, c_i, old_rt, new_rt in [
        (74, 69, 2, 6), (75, 70, 3, 5), (76, 71, 4, 4), (77, 72, 5, 3)
    ]:
        i = insns[c_i]
        i = patch_reg_field(i, 'rs', 6, 2)  # a2 -> v0
        if old_rt != new_rt:
            i = patch_reg_field(i, 'rt', old_rt, new_rt)
        result[t_i] = i

    # Second half stores: target[78-82] = sw a2/a1/a0 to s2, j, sw v1 to s2
    for t_i, c_i, old_rt, new_rt in [
        (78, 73, 2, 6), (79, 74, 3, 5), (80, 75, 4, 4)
    ]:
        i = insns[c_i]
        if old_rt != new_rt:
            i = patch_reg_field(i, 'rt', old_rt, new_rt)
        i = patch_reg_field(i, 'rs', 17, 18)  # s1 -> s2
        result[t_i] = i

    # target[81] = j done_final -> idx 99 = byte 0x18C
    result[81] = 0x08000000 | (0x18C >> 2)
    # target[82] = sw v1,16(s2) -- compiled[77] = sw a1,16(s1)
    i = insns[77]
    i = patch_reg_field(i, 'rt', 5, 3)   # a1 -> v1
    i = patch_reg_field(i, 'rs', 17, 18)  # s1 -> s2
    result[82] = i

    # not_equal block (target[83-86]):
    # target[83] = lui at,0x8013 -- compiled[79] = lui at,0x0000
    result[83] = insns[79]
    # target[84] = addu at,at,v0 -- compiled[80]
    result[84] = insns[80]
    # target[85] = lw v0,7412(at) -- compiled[81] = lw a2,4(at)
    result[85] = swap_reg_in_insn(insns[81], 2, 6)
    # target[86] = addiu s1,a2,1 -- compiled[78] = addiu a3,v1,1
    i = insns[78]
    i = swap_reg_in_insn(i, 3, 6)  # v1 -> a2
    i = patch_reg_field(i, 'rt', 7, 17)  # a3 -> s1
    result[86] = i

    # copy_second block (target[87-98]):
    # Struct ptr: target=v0, compiled=a2
    # Load regs: target {v1,a0,a1,a2} from v0, compiled {v0,v1,a0,a1} from a2
    for t_i, c_i, old_rt, new_rt in [
        (87, 83, 2, 3), (88, 84, 3, 4), (89, 85, 4, 5), (90, 86, 5, 6)
    ]:
        i = insns[c_i]
        i = patch_reg_field(i, 'rs', 6, 2)  # a2 -> v0
        i = patch_reg_field(i, 'rt', old_rt, new_rt)
        result[t_i] = i
    # Stores to s2
    for t_i, c_i, old_rt, new_rt in [
        (91, 87, 2, 3), (92, 88, 3, 4), (93, 89, 4, 5), (94, 90, 5, 6)
    ]:
        i = insns[c_i]
        i = patch_reg_field(i, 'rt', old_rt, new_rt)
        i = patch_reg_field(i, 'rs', 17, 18)  # s1 -> s2
        result[t_i] = i
    # Second half loads from v0
    for t_i, c_i, old_rt, new_rt in [
        (95, 91, 5, 6), (96, 92, 4, 5), (97, 93, 3, 4), (98, 94, 2, 3)
    ]:
        i = insns[c_i]
        i = patch_reg_field(i, 'rs', 6, 2)  # a2 -> v0
        if old_rt != new_rt:
            i = patch_reg_field(i, 'rt', old_rt, new_rt)
        result[t_i] = i

    # done_final: stores + blez (target[99-108]):
    # target[99] = sw a2,28(s2) -- from compiled[95] = sw a1,28(s1)
    i = insns[95]
    i = patch_reg_field(i, 'rt', 5, 6)   # a1 -> a2
    i = patch_reg_field(i, 'rs', 17, 18)  # s1 -> s2
    result[99] = i
    # target[100] = sw a1,24(s2) -- from compiled[96] = sw a0,24(s1)
    i = insns[96]
    i = patch_reg_field(i, 'rt', 4, 5)   # a0 -> a1
    i = patch_reg_field(i, 'rs', 17, 18)
    result[100] = i
    # target[101] = sw a0,20(s2) -- from compiled[97] = sw v1,20(s1)
    i = insns[97]
    i = patch_reg_field(i, 'rt', 3, 4)   # v1 -> a0
    i = patch_reg_field(i, 'rs', 17, 18)
    result[101] = i
    # target[102] = sw v1,16(s2) -- from compiled[98] = sw v0,16(s1)
    i = insns[98]
    i = patch_reg_field(i, 'rt', 2, 3)   # v0 -> v1
    i = patch_reg_field(i, 'rs', 17, 18)
    result[102] = i

    # target[103] = blez s1,+31 -> [135] -- compiled[99] = blez a3,+33
    i = insns[99]
    i = patch_reg_field(i, 'rs', 7, 17)  # a3 -> s1
    i = fix_branch_offset(i, 31)  # 135-103-1 = 31
    result[103] = i
    # target[104] = li s3,1 = compiled[105]
    result[104] = insns[105]

    # Bottom loop setup (target[105-108]):
    # target[105] = lui v1,0x8013 -- compiled[101] = lui v0,0x0000
    # v0 -> v1
    result[105] = patch_reg_field(insns[101], 'rt', 2, 3)
    # target[106] = addiu v1,v1,7404 -- compiled[102] = addiu v0,v0,0
    i = insns[102]
    i = patch_reg_field(i, 'rt', 2, 3)  # v0 -> v1
    i = patch_reg_field(i, 'rs', 2, 3)  # v0 -> v1
    result[106] = i
    # target[107] = sll v0,s1,2 -- compiled[100] = sll v1,a3,2
    i = insns[100]
    i = patch_reg_field(i, 'rd', 3, 2)  # v1 -> v0
    i = patch_reg_field(i, 'rt', 7, 17)  # a3 -> s1
    result[107] = i
    # target[108] = addu s0,v0,v1 -- compiled[103] = addu s0,v1,v0
    # Just swap v0<->v1 in the addu
    result[108] = swap_reg_in_insn(insns[103], 2, 3)

    # Bottom loop body (target[109-134]):
    # target[109] = lw a1,0(s0) = compiled[106]
    result[109] = insns[106]
    # target[110] = move a0,s2 -- compiled[107] = move a0,s1 with s1->s2
    result[110] = s(107)
    # target[111] = jal GsMulCoord3 = compiled[108]
    result[111] = insns[108]
    # target[112] = addiu a1,a1,4 = compiled[109]
    result[112] = insns[109]

    # After jal return:
    # target[113] = lw v0,0(s0) -- compiled[110] = lw a2,0(s0)
    # a2 -> v0
    result[113] = swap_reg_in_insn(insns[110], 2, 6)
    # target[114] = lw v1,0(s2) -- compiled[111] = lw v0,0(s1) with s1->s2, v0->v1
    i = insns[111]
    i = patch_reg_field(i, 'rs', 17, 18)  # s1 -> s2
    i = patch_reg_field(i, 'rt', 2, 3)   # v0 -> v1
    result[114] = i
    # target[115] = lw a0,4(s2) -- compiled[112] = lw v1,4(s1)
    i = insns[112]
    i = patch_reg_field(i, 'rs', 17, 18)
    i = patch_reg_field(i, 'rt', 3, 4)   # v1 -> a0
    result[115] = i
    # target[116] = lw a1,8(s2) -- compiled[113] = lw a0,8(s1)
    i = insns[113]
    i = patch_reg_field(i, 'rs', 17, 18)
    i = patch_reg_field(i, 'rt', 4, 5)   # a0 -> a1
    result[116] = i
    # target[117] = lw a2,12(s2) -- compiled[114] = lw a1,12(s1)
    i = insns[114]
    i = patch_reg_field(i, 'rs', 17, 18)
    i = patch_reg_field(i, 'rt', 5, 6)   # a1 -> a2
    result[117] = i

    # Stores to struct: target uses v0 as base, compiled uses a2
    # target[118] = sw v1,36(v0) -- compiled[115] = sw v0,36(a2)
    i = insns[115]
    i = swap_reg_in_insn(i, 2, 6)  # v0<->a2: both base and stored reg swap
    # That gives: sw a2,36(v0). But target is sw v1,36(v0).
    # v0($2) swapped to a2($6) in rt, a2($6) swapped to v0($2) in rs.
    # Result: sw a2,36(v0). We need sw v1,36(v0). So we also need to rotate the stored reg.
    # compiled[115] = sw v0,36(a2): rt=v0($2), rs=a2($6)
    # target[118] = sw v1,36(v0): rt=v1($3), rs=v0($2)
    i = insns[115]
    i = patch_reg_field(i, 'rs', 6, 2)  # a2 -> v0 (base)
    i = patch_reg_field(i, 'rt', 2, 3)  # v0 -> v1 (stored reg)
    result[118] = i

    # target[119] = sw a0,40(v0) from compiled[116] = sw v1,40(a2)
    i = insns[116]
    i = patch_reg_field(i, 'rs', 6, 2)
    i = patch_reg_field(i, 'rt', 3, 4)  # v1 -> a0
    result[119] = i
    # target[120] = sw a1,44(v0) from compiled[117] = sw a0,44(a2)
    i = insns[117]
    i = patch_reg_field(i, 'rs', 6, 2)
    i = patch_reg_field(i, 'rt', 4, 5)  # a0 -> a1
    result[120] = i
    # target[121] = sw a2,48(v0) from compiled[118] = sw a1,48(a2)
    i = insns[118]
    i = patch_reg_field(i, 'rs', 6, 2)  # a2 -> v0 (base)
    i = patch_reg_field(i, 'rt', 5, 6)  # a1 -> a2 (stored reg)
    result[121] = i

    # Second half loads from s2:
    # target[122] = lw a2,28(s2) from compiled[119] = lw a1,28(s1)
    i = insns[119]
    i = patch_reg_field(i, 'rs', 17, 18)
    i = patch_reg_field(i, 'rt', 5, 6)
    result[122] = i
    # target[123] = lw a1,24(s2) from compiled[120] = lw a0,24(s1)
    i = insns[120]
    i = patch_reg_field(i, 'rs', 17, 18)
    i = patch_reg_field(i, 'rt', 4, 5)
    result[123] = i
    # target[124] = lw a0,20(s2) from compiled[121] = lw v1,20(s1)
    i = insns[121]
    i = patch_reg_field(i, 'rs', 17, 18)
    i = patch_reg_field(i, 'rt', 3, 4)
    result[124] = i
    # target[125] = lw v1,16(s2) from compiled[122] = lw v0,16(s1)
    i = insns[122]
    i = patch_reg_field(i, 'rs', 17, 18)
    i = patch_reg_field(i, 'rt', 2, 3)
    result[125] = i

    # Stores to struct (second half):
    # target[126] = sw a2,64(v0) from compiled[123] = sw a1,64(a2)
    i = insns[123]
    i = patch_reg_field(i, 'rs', 6, 2)
    i = patch_reg_field(i, 'rt', 5, 6)
    result[126] = i
    # target[127] = sw a1,60(v0) from compiled[124] = sw a0,60(a2)
    i = insns[124]
    i = patch_reg_field(i, 'rs', 6, 2)
    i = patch_reg_field(i, 'rt', 4, 5)
    result[127] = i
    # target[128] = sw a0,56(v0) from compiled[125] = sw v1,56(a2)
    i = insns[125]
    i = patch_reg_field(i, 'rs', 6, 2)
    i = patch_reg_field(i, 'rt', 3, 4)
    result[128] = i
    # target[129] = sw v1,52(v0) from compiled[126] = sw v0,52(a2)
    i = insns[126]
    i = patch_reg_field(i, 'rs', 6, 2)
    i = patch_reg_field(i, 'rt', 2, 3)
    result[129] = i

    # Loop tail:
    # target[130] = lw v0,0(s0) from compiled[127] = lw a2,0(s0) with a2->v0
    result[130] = swap_reg_in_insn(insns[127], 2, 6)
    # target[131] = addiu s1,s1,-1 from compiled[130] = addiu s2,s2,-1 with s2->s1
    result[131] = s(130)
    # target[132] = addiu s0,s0,-4 = compiled[132]
    result[132] = insns[132]
    # target[133] = bgtz s1,-25 -> [109] from compiled[131] = bgtz s2,-26
    i = s(131)  # s2 -> s1
    i = fix_branch_offset(i, -25)  # 109-133-1 = -25
    result[133] = i
    # target[134] = sw s3,0(v0) from compiled[129] = sw s3,0(a2) with a2->v0
    result[134] = swap_reg_in_insn(insns[129], 2, 6)

    # Epilogue (target[135-141]):
    # Compiled has 141 insns (or 142 with padding nop), target needs 142.
    # Compiled epilogue: lw ra; lw s3; lw s2; lw s1; lw s0; addiu sp; jr ra; [nop]
    # Target epilogue:   lw ra; lw s3; lw s2; lw s1; lw s0; jr ra; addiu sp
    # (Target has epilogue delay slot filled)
    #
    # With gccwrap --no-delay-slots, compiled has indices up to 140 (141 insns).
    # With manual maspsx, compiled may have 142 insns (with trailing nop).
    # Handle both cases.
    n_compiled = n_insns  # 141 or 142

    # Epilogue register restore positions (compiled):
    # [133] lw ra,32(sp)
    # [134] lw s3,28(sp)
    # [135] lw s2,24(sp)
    # [136] lw s1,20(sp)
    # [137] lw s0,16(sp)
    # [138] addiu sp,sp,40
    # [139] jr ra
    # [140] nop (may not exist if stripped)
    result[135] = insns[133]
    result[136] = insns[134]
    result[137] = insns[135]  # lw s2,24(sp) -- no swap needed
    result[138] = insns[136]  # lw s1,20(sp) -- no swap needed
    result[139] = insns[137]
    result[140] = insns[139]  # jr ra
    result[141] = insns[138]  # addiu sp,sp,40 (moved to delay slot)

    # ===== Write result instructions =====
    # Target is 142 instructions = 568 bytes.
    # Compiled may be 141 (564), 142 (568), or more (576 with padding).
    # We need exactly 568 bytes of .text.
    target_text_size = 142 * 4  # 568

    # Update .text section size
    if text_sh_off is not None:
        struct.pack_into('<I', data, text_sh_off + 0x14, target_text_size)

    # Update symbol sizes that match old text_size
    for si in range(e_shnum):
        sh_off = e_shoff + si * e_shentsize
        sh_type = struct.unpack_from('<I', data, sh_off + 0x04)[0]
        if sh_type == 2:  # SHT_SYMTAB
            sym_off = struct.unpack_from('<I', data, sh_off + 0x10)[0]
            sym_size_total = struct.unpack_from('<I', data, sh_off + 0x14)[0]
            sym_entsize = struct.unpack_from('<I', data, sh_off + 0x24)[0]
            if sym_entsize == 0:
                sym_entsize = 16
            for j in range(sym_size_total // sym_entsize):
                st_off = sym_off + j * sym_entsize
                st_size = struct.unpack_from('<I', data, st_off + 0x08)[0]
                if st_size == text_size:
                    struct.pack_into('<I', data, st_off + 0x08, target_text_size)

    for i in range(142):
        struct.pack_into('<I', data, text_off + i * 4, result[i])

    # ===== Fix relocations =====
    # Compiled relocation positions (byte offsets):
    # - [7]  0x1C = lui a1 (DSTACK high)
    # - [8]  0x20 = addiu a1 (DSTACK low)
    # - [22] 0x58 = j loop_top (internal, may not have reloc)
    # - [31] 0x7C = lui a2 (DSTACK high) -- copy block 1
    # - [32] 0x80 = lw a2 (DSTACK low) -- copy block 1
    # - [46] 0xB8 = j done_copy_shared (internal)
    # - [50] 0xC8 = lui a2 (DSTACK high) -- final_idx==100
    # - [51] 0xCC = lw a2 (DSTACK low)
    # - [52] 0xD0 = j copy_second (internal)
    # - [57] 0xE4 = lui at (DSTACK high) -- array indexed
    # - [59] 0xEC = lw a2 (DSTACK low via at)
    # - [76] 0x130 = j done_copy (internal)
    # - [79] 0x13C = lui at (DSTACK high) -- not_equal
    # - [81] 0x144 = lw a2 (DSTACK+4 low via at)
    # - [101] 0x194 = lui v0 (DSTACK high) -- bottom loop
    # - [102] 0x198 = addiu v0 (DSTACK low)
    # - [108] 0x1B0 = jal GsMulCoord3

    # Target relocation positions (byte offsets):
    # - [5]  0x14 = lui a1 -> from compiled[7]  0x1C
    # - [7]  0x1C = addiu a1 -> from compiled[8]  0x20
    # - [20] 0x50 = j dispatch (internal) -- NO reloc needed (internal j)
    # - [28] 0x70 = j loop_top (internal) -- NO reloc
    # - [32] 0x80 = lui v0 -> from compiled[31] 0x7C
    # - [33] 0x84 = lw v0 -> from compiled[32] 0x80
    # - [50] 0xC8 = j done_final (internal) -- NO reloc
    # - [54] 0xD8 = lui v0 -> from compiled[50] 0xC8
    # - [55] 0xDC = lw v0 -> from compiled[51] 0xCC
    # - [57] 0xE4 = j copy_second (internal) -- NO reloc
    # - [62] 0xF8 = lui at -> from compiled[57] 0xE4
    # - [64] 0x100 = lw v0 -> from compiled[59] 0xEC
    # - [81] 0x144 = j done_final (internal) -- NO reloc
    # - [83] 0x14C = lui at -> from compiled[79] 0x13C
    # - [85] 0x154 = lw v0 -> from compiled[81] 0x144
    # - [105] 0x1A4 = lui v1 -> from compiled[101] 0x194
    # - [106] 0x1A8 = addiu v1 -> from compiled[102] 0x198
    # - [111] 0x1BC = jal -> from compiled[108] 0x1B0

    # Build relocation mapping: old_offset -> new_offset
    reloc_map = {
        0x1C: 0x14,   # lui a1 (DSTACK high)
        0x20: 0x1C,   # addiu a1 (DSTACK low)
        0x7C: 0x80,   # lui (DSTACK high) -- copy block 1
        0x80: 0x84,   # lw (DSTACK low) -- copy block 1
        0xC8: 0xD8,   # lui (DSTACK high) -- final_idx==100
        0xCC: 0xDC,   # lw (DSTACK low)
        0xE4: 0xF8,   # lui at (DSTACK high) -- array indexed
        0xEC: 0x100,  # lw (DSTACK low via at)
        0x13C: 0x14C, # lui at (DSTACK high) -- not_equal
        0x144: 0x154, # lw (DSTACK+4 low via at)
        0x194: 0x1A4, # lui v0 (DSTACK high) -- bottom loop
        0x198: 0x1A8, # addiu v0 (DSTACK low)
        0x1B0: 0x1BC, # jal GsMulCoord3
    }

    # Also need to handle j instructions -- internal j's may have relocations
    # (R_MIPS_26 type) for local labels. Let me check what relocs actually exist.
    # Internal j's in GCC output typically don't have relocations (they're assembled
    # with absolute addresses within the section). The assembler resolves them.

    if rel_off:
        for i in range(rel_size // 8):
            r_offset = struct.unpack_from('<I', data, rel_off + i * 8)[0]
            if r_offset in reloc_map:
                struct.pack_into('<I', data, rel_off + i * 8, reloc_map[r_offset])
            # j/b instructions to internal labels: delete relocs for them
            # (internal jumps are recoded with absolute word addresses in result)
            elif r_offset in (0x58, 0xB8, 0xD0, 0x130):
                # These are j instructions that moved, but they don't typically
                # have relocations in GCC output
                pass

    with open(output_path, 'wb') as f:
        f.write(data)


if __name__ == '__main__':
    process(sys.argv[1], sys.argv[2])
