"""Assembly transform for ContRpn1.

IDO generates 'sh $11, 18($3)' (halfword store preserving andi mask),
but target has 'sb $6, 19($3)' (byte store, same mask, different reg).

Transform:
1. 'and $11, $6, 255' -> 'andi $6, $6, 0xff' (keep mask, target dest reg)
2. 'sh $11, 18($3)' -> 'sb $6, 19($3)' (halfword -> byte, fix offset)
3. 'move' -> 'addu $rd, $rs, $0' (library region encoding)
4. 'j $31' -> 'jr $31' (return instruction)
"""
import re


def transform(lines):
    result = []
    for line in lines:
        s = line

        # Fix: and $11, $6, 255 -> andi $6, $6, 0xff
        if re.search(r'and\s+\$11,\s*\$6,\s*255', s):
            s = re.sub(r'and\s+\$11,\s*\$6,\s*255', 'andi\t$6, $6, 0xff', s)
            result.append(s)
            continue

        # Fix: sh $11, 18($3) -> sb $6, 19($3)
        if re.search(r'sh\s+\$11,\s*18\(\$3\)', s):
            s = re.sub(r'sh\s+\$11,\s*18\(\$3\)', 'sb\t$6, 19($3)', s)
            result.append(s)
            continue

        # Convert move to addu (library region: addu encoding, not or)
        m = re.match(r'(\s+)move\s+(\$\d+),\s*(\$\d+)(.*)', s)
        if m:
            indent, rd, rs, rest = m.groups()
            s = f'{indent}addu\t{rd}, {rs}, $0{rest}\n'
            result.append(s)
            continue

        # Fix j $31 -> jr $31
        if re.search(r'\bj\s+\$31\b', s):
            s = re.sub(r'\bj\s+\$31\b', 'jr\t$31', s)
            result.append(s)
            continue

        result.append(s)

    return result
