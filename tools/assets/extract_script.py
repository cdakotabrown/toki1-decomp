#!/usr/bin/env python3
"""
Tokimeki Memorial Script Extractor
Extracts all Japanese text/dialogue from the game disc and executable

Revision 2 (2026-01-19): Major filtering improvements to reduce garbage extraction
- Stricter Shift-JIS validation (common kanji/kana ranges only)
- Higher japanese_ratio threshold (50% instead of 30%)
- Character diversity check to filter repetitive binary patterns
- Minimum unique character count requirement
- Filter out strings with too many control/symbol characters
- Better detection of binary data masquerading as text
"""

import struct
import sys
from pathlib import Path
from typing import List, Tuple, Set
import json
import re
from collections import Counter

# Character names to look for (for organizing by character)
CHARACTER_NAMES = [
    "詩織",      # Shiori
    "虹野",      # Nijino
    "片桐",      # Katagiri
    "伊集院",    # Ijuin
    "古式",      # Koshiki
    "如月",      # Kisaragi
    "鏡",        # Kagami
    "早乙女",    # Saotome
    "朝日奈",    # Asahina
    "美樹原",    # Mikihara
    "館林",      # Tatebayashi
    "紐緒",      # Himoo
    "由美",      # Yumi
]

# Common Japanese punctuation that indicates real text
JAPANESE_PUNCTUATION = set('。、！？「」『』（）・〜ー…')

# Half-width katakana that are common in dialogue vs. binary patterns
# Restrict to more common ones, not the full 0xA1-0xDF range
COMMON_HALF_WIDTH_KATAKANA = set([
    0xA1,  # ｡
    0xA2,  # ｢
    0xA3,  # ｣
    0xA4,  # ､
    0xA5,  # ･
    0xA6,  # ｦ
    0xB0,  # ー
    0xDE,  # ゛
    0xDF,  # ゜
    # Common katakana: ｱ-ﾝ (0xB1-0xDD) but we'll be more selective
])
# Actually include common katakana syllables
for b in range(0xB1, 0xDD):  # ｱ through ﾝ
    COMMON_HALF_WIDTH_KATAKANA.add(b)

def is_shift_jis_char(byte1: int, byte2: int) -> bool:
    """
    Check if two bytes form a valid Shift-JIS character.

    This is a stricter check that focuses on ranges commonly used in dialogue:
    - Hiragana (0x82 range)
    - Katakana (0x83 range)
    - Common kanji (0x88-0x9F, 0xE0-0xEA)
    - Punctuation and symbols (0x81 range)

    We exclude some ranges that are valid Shift-JIS but rarely appear in dialogue
    and often appear in binary data that happens to look like text.
    """
    # First byte ranges - be more restrictive
    if byte1 == 0x81:
        # JIS symbols/punctuation - allow most but not all
        # Second byte 0x40-0xAC covers common punctuation
        if 0x40 <= byte2 <= 0xAC and byte2 != 0x7F:
            return True
        return False
    elif byte1 == 0x82:
        # Hiragana (0x9F-0xF1) and numbers/letters
        if 0x40 <= byte2 <= 0xFC and byte2 != 0x7F:
            return True
        return False
    elif byte1 == 0x83:
        # Katakana (0x40-0x96) and Greek letters
        if 0x40 <= byte2 <= 0xFC and byte2 != 0x7F:
            return True
        return False
    elif 0x84 <= byte1 <= 0x87:
        # Less common ranges - be stricter
        # 0x84: Box drawing, 0x85-0x87: rarely used in dialogue
        return False
    elif 0x88 <= byte1 <= 0x9F:
        # Level 1 Kanji - common in dialogue
        if 0x40 <= byte2 <= 0xFC and byte2 != 0x7F:
            return True
        return False
    elif 0xE0 <= byte1 <= 0xEA:
        # Level 2 Kanji - also common
        if 0x40 <= byte2 <= 0xFC and byte2 != 0x7F:
            return True
        return False
    elif 0xEB <= byte1 <= 0xEF:
        # Rare kanji/user-defined area - usually not dialogue
        return False

    return False

def is_half_width_katakana(byte: int) -> bool:
    """
    Check if byte is half-width katakana.
    More restrictive than the full 0xA1-0xDF range to avoid binary data.
    """
    return byte in COMMON_HALF_WIDTH_KATAKANA

def is_likely_dialogue(decoded: str) -> bool:
    """
    Additional heuristics to determine if a decoded string is likely real dialogue
    vs. garbage data that happens to decode to something.

    Returns True if the string appears to be real dialogue.
    """
    if len(decoded) < 4:
        return False

    # Count character types
    hiragana_count = 0
    katakana_count = 0
    kanji_count = 0
    punctuation_count = 0
    ascii_letter_count = 0
    ascii_digit_count = 0
    ascii_symbol_count = 0
    fullwidth_count = 0
    halfwidth_kana_count = 0
    other_count = 0

    for c in decoded:
        cp = ord(c)
        if 0x3040 <= cp <= 0x309F:  # Hiragana
            hiragana_count += 1
        elif 0x30A0 <= cp <= 0x30FF:  # Katakana
            katakana_count += 1
        elif 0x4E00 <= cp <= 0x9FFF:  # CJK Unified Ideographs (Kanji)
            kanji_count += 1
        elif c in JAPANESE_PUNCTUATION:
            punctuation_count += 1
        elif c.isalpha() and 0x20 <= cp <= 0x7E:  # ASCII letters
            ascii_letter_count += 1
        elif c.isdigit() and 0x20 <= cp <= 0x7E:  # ASCII digits
            ascii_digit_count += 1
        elif 0x20 <= cp <= 0x7E:  # Other ASCII (symbols, space)
            ascii_symbol_count += 1
        elif 0xFF00 <= cp <= 0xFF5E:  # Full-width ASCII
            fullwidth_count += 1
        elif 0xFF61 <= cp <= 0xFF9F:  # Half-width katakana
            halfwidth_kana_count += 1
        else:
            other_count += 1

    total = len(decoded)

    # Core Japanese characters (hiragana, katakana, kanji, punctuation)
    core_japanese = hiragana_count + katakana_count + kanji_count + punctuation_count
    # Include full-width and half-width kana as Japanese
    all_japanese = core_japanese + fullwidth_count + halfwidth_kana_count

    # Calculate ratio of Japanese characters
    japanese_ratio = all_japanese / total if total > 0 else 0

    # Require at least 60% Japanese characters for short strings
    # Real dialogue rarely has many random ASCII characters mixed in
    if total <= 10:
        if japanese_ratio < 0.6:
            return False
    else:
        # For longer strings, 50% is acceptable
        if japanese_ratio < 0.5:
            return False

    # Check for suspicious patterns: single random ASCII characters interspersed
    # Real dialogue doesn't have patterns like "<行c芯" with isolated ASCII chars
    # Exception: strings that start with characters like numbers or brackets
    ascii_count = ascii_letter_count + ascii_symbol_count
    if ascii_count > 0 and core_japanese > 0:
        # If we have a mix of ASCII and Japanese, check if it makes sense
        # Single isolated ASCII letters in the middle of Japanese text are suspicious
        if ascii_letter_count > 0 and ascii_letter_count <= 2 and total > 5:
            # Check if ASCII letters are isolated (not part of a word)
            # This catches patterns like "<行c芯" where 'c' is isolated
            for i, c in enumerate(decoded):
                if c.isalpha() and ord(c) < 0x80:  # ASCII letter
                    # Check if surrounded by non-ASCII
                    prev_is_japanese = i > 0 and ord(decoded[i-1]) > 0x7F
                    next_is_japanese = i < len(decoded)-1 and ord(decoded[i+1]) > 0x7F
                    if prev_is_japanese and next_is_japanese:
                        return False  # Isolated ASCII letter in Japanese text

    # Check for character diversity - real dialogue has varied characters
    unique_chars = len(set(decoded))
    diversity_ratio = unique_chars / total if total > 0 else 0

    # If string is longer than 6 chars but has very low diversity, it's probably garbage
    if total > 6 and diversity_ratio < 0.3:
        return False

    # Require at least some hiragana OR full-width chars OR punctuation for longer strings
    # Pure kanji strings with no hiragana are suspicious for Japanese dialogue
    if total > 8 and hiragana_count == 0 and punctuation_count == 0 and fullwidth_count == 0:
        # Allow if it's mostly katakana (foreign words, names, etc.)
        if katakana_count + halfwidth_kana_count < total * 0.4:
            return False

    # Filter out strings with too many "other" characters (garbage Unicode)
    if other_count > total * 0.15:
        return False

    # Strings that are all the same character repeated are garbage
    if unique_chars <= 2 and total > 4:
        return False

    # Filter strings that are too short after removing ASCII
    japanese_only_len = hiragana_count + katakana_count + kanji_count + fullwidth_count + halfwidth_kana_count
    if japanese_only_len < 3:
        return False

    # Additional check: filter strings that start with certain suspicious patterns
    # Binary data often produces strings starting with symbols like < > { } etc.
    if decoded and decoded[0] in '<>{|}[]\\^~':
        # If it starts with these symbols and is short, likely garbage
        if total < 8:
            return False
        # If it starts with these and has isolated kanji, likely garbage
        if kanji_count > 0 and hiragana_count == 0 and katakana_count == 0:
            return False

    # Filter strings that rely heavily on half-width katakana
    # Real Japanese dialogue uses full-width katakana; half-width is rare and
    # often appears in binary data that happens to decode to something
    if halfwidth_kana_count > 0:
        # If half-width kana is the majority of the string, it's suspicious
        if halfwidth_kana_count >= total * 0.4:
            # Unless it's very long and has other Japanese chars
            if hiragana_count == 0 and katakana_count == 0 and total < 15:
                return False

        # If we have half-width kana mixed with random ASCII symbols, it's garbage
        if ascii_symbol_count > 0 and halfwidth_kana_count > hiragana_count + katakana_count:
            return False

    # Filter strings that have multiple different types of ASCII characters mixed in
    # Real dialogue might have "ABC" or "123" but not random "{>c" mixed with Japanese
    if ascii_symbol_count > 1 and ascii_letter_count > 0:
        # Multiple different ASCII things mixed together is suspicious
        if hiragana_count < 2:
            return False

    # Filter strings with isolated kanji surrounded by half-width punctuation
    # This catches patterns like "夫｡ﾙ" or "c怱､"
    if kanji_count <= 2 and halfwidth_kana_count > kanji_count:
        if hiragana_count == 0 and katakana_count == 0:
            return False

    return True


def extract_strings_from_data(data: bytes, min_length: int = 8) -> List[Tuple[int, str]]:
    """
    Extract null-terminated Shift-JIS strings from binary data
    Returns list of (offset, decoded_string) tuples

    This version includes stricter filtering to avoid extracting binary data
    that happens to look like valid Shift-JIS sequences.
    """
    strings = []
    i = 0

    while i < len(data) - min_length:
        # Look for start of potential string
        if data[i] == 0:
            i += 1
            continue

        # Try to extract a string
        start_offset = i
        string_bytes = bytearray()
        has_japanese = False
        sjis_char_count = 0
        half_width_count = 0

        while i < len(data) and data[i] != 0:
            byte = data[i]

            # Check for Shift-JIS double-byte character
            if i + 1 < len(data) and is_shift_jis_char(byte, data[i + 1]):
                string_bytes.append(byte)
                string_bytes.append(data[i + 1])
                has_japanese = True
                sjis_char_count += 1
                i += 2
            # Half-width katakana
            elif is_half_width_katakana(byte):
                string_bytes.append(byte)
                has_japanese = True
                half_width_count += 1
                i += 1
            # ASCII printable or common punctuation
            elif 0x20 <= byte <= 0x7E or byte in [0x0A, 0x0D]:
                string_bytes.append(byte)
                i += 1
            else:
                # Invalid character, not a string
                break

        # Check if we found a valid string
        if i < len(data) and data[i] == 0 and len(string_bytes) >= min_length and has_japanese:
            try:
                decoded = string_bytes.decode('shift-jis', errors='ignore')

                # Apply enhanced dialogue detection heuristics
                if is_likely_dialogue(decoded):
                    strings.append((start_offset, decoded))

            except:
                pass

        i += 1

    return strings

def extract_from_executable(exe_path: Path) -> List[Tuple[int, str]]:
    """Extract strings from the main executable (SLPM_86.053)"""
    print(f"Extracting strings from executable: {exe_path}")

    with open(exe_path, 'rb') as f:
        data = f.read()

    # Data section starts at 0x6F340 (455,488 bytes from start)
    data_section_offset = 0x6F340
    data_section = data[data_section_offset:]

    strings = extract_strings_from_data(data_section, min_length=4)

    # Adjust offsets to be relative to file start
    strings = [(offset + data_section_offset, text) for offset, text in strings]

    print(f"  Found {len(strings)} strings in executable")
    return strings

def extract_from_disc_sectors(disc_path: Path, start_sector: int = 17000, end_sector: int = 60000) -> dict:
    """
    Extract strings from raw disc sectors
    Returns dict mapping sector ranges to strings
    """
    print(f"Extracting strings from disc sectors {start_sector}-{end_sector}: {disc_path}")

    SECTOR_SIZE = 2352  # MODE2/2352 format
    SECTOR_DATA_OFFSET = 24  # Skip sync + header
    SECTOR_DATA_SIZE = 2048  # Actual data per sector

    with open(disc_path, 'rb') as f:
        # Seek to start sector
        f.seek(start_sector * SECTOR_SIZE)

        # Process sectors in chunks
        regions = {}
        current_region_start = start_sector
        current_region_strings = []
        empty_sectors = 0

        for sector_num in range(start_sector, end_sector):
            # Read sector
            sector_data = f.read(SECTOR_SIZE)
            if len(sector_data) < SECTOR_SIZE:
                break

            # Extract data portion (skip sync/header)
            data = sector_data[SECTOR_DATA_OFFSET:SECTOR_DATA_OFFSET + SECTOR_DATA_SIZE]

            # Extract strings from this sector
            strings = extract_strings_from_data(data, min_length=6)

            if strings:
                # Add sector number to each string
                for offset, text in strings:
                    current_region_strings.append((sector_num, offset, text))
                empty_sectors = 0
            else:
                empty_sectors += 1

                # If we hit 50 empty sectors, consider it a region boundary
                if empty_sectors >= 50 and current_region_strings:
                    region_key = f"sectors_{current_region_start:06d}-{sector_num - 50:06d}"
                    regions[region_key] = current_region_strings
                    current_region_strings = []
                    current_region_start = sector_num + 1

        # Save final region
        if current_region_strings:
            region_key = f"sectors_{current_region_start:06d}-{end_sector:06d}"
            regions[region_key] = current_region_strings

    total_strings = sum(len(strings) for strings in regions.values())
    print(f"  Found {total_strings} strings across {len(regions)} regions")
    return regions

def categorize_by_character(strings: List[Tuple[int, int, str]]) -> dict:
    """Categorize dialogue strings by character name"""
    categorized = {name: [] for name in CHARACTER_NAMES}
    categorized["unknown"] = []

    for entry in strings:
        sector, offset, text = entry

        # Check if any character name appears in the text
        found_character = False
        for char_name in CHARACTER_NAMES:
            if char_name in text:
                categorized[char_name].append(entry)
                found_character = True
                break

        if not found_character:
            categorized["unknown"].append(entry)

    return categorized

def save_script_files(exe_strings: List[Tuple[int, str]],
                      disc_regions: dict,
                      output_dir: Path):
    """Save extracted script to organized files"""
    output_dir.mkdir(parents=True, exist_ok=True)

    # Save executive strings (UI/system text)
    print(f"\nSaving executable strings to: {output_dir / 'system_ui_text.txt'}")
    with open(output_dir / 'system_ui_text.txt', 'w', encoding='utf-8') as f:
        f.write("=" * 80 + "\n")
        f.write("TOKIMEKI MEMORIAL - SYSTEM/UI TEXT (from SLPM_86.053)\n")
        f.write("=" * 80 + "\n\n")

        for offset, text in exe_strings:
            f.write(f"[Offset 0x{offset:08X}]\n")
            f.write(f"{text}\n\n")

        f.write(f"\nTotal: {len(exe_strings)} strings\n")

    # Save disc regions as separate files
    print(f"\nSaving dialogue regions:")
    region_summary = []

    for region_name, strings in disc_regions.items():
        filename = f"dialogue_{region_name}.txt"
        print(f"  {filename}: {len(strings)} strings")

        with open(output_dir / filename, 'w', encoding='utf-8') as f:
            f.write("=" * 80 + "\n")
            f.write(f"TOKIMEKI MEMORIAL - DIALOGUE REGION: {region_name}\n")
            f.write("=" * 80 + "\n\n")

            for sector, offset, text in strings:
                f.write(f"[Sector {sector:06d}, Offset 0x{offset:04X}]\n")
                f.write(f"{text}\n\n")

            f.write(f"\nTotal: {len(strings)} strings\n")

        region_summary.append({
            "file": filename,
            "region": region_name,
            "string_count": len(strings)
        })

    # Create character-based files
    print(f"\nOrganizing dialogue by character:")
    all_dialogue = []
    for strings in disc_regions.values():
        all_dialogue.extend(strings)

    by_character = categorize_by_character(all_dialogue)

    for char_name, strings in by_character.items():
        if not strings:
            continue

        filename = f"character_{char_name}.txt"
        print(f"  {filename}: {len(strings)} lines")

        with open(output_dir / filename, 'w', encoding='utf-8') as f:
            f.write("=" * 80 + "\n")
            f.write(f"TOKIMEKI MEMORIAL - CHARACTER: {char_name}\n")
            f.write("=" * 80 + "\n\n")

            for sector, offset, text in strings:
                f.write(f"[Sector {sector:06d}]\n")
                f.write(f"{text}\n\n")

            f.write(f"\nTotal: {len(strings)} lines\n")

    # Create summary file
    print(f"\nCreating summary: {output_dir / 'extraction_summary.txt'}")
    with open(output_dir / 'extraction_summary.txt', 'w', encoding='utf-8') as f:
        f.write("=" * 80 + "\n")
        f.write("TOKIMEKI MEMORIAL - SCRIPT EXTRACTION SUMMARY\n")
        f.write("=" * 80 + "\n\n")

        f.write("SYSTEM/UI TEXT:\n")
        f.write(f"  File: system_ui_text.txt\n")
        f.write(f"  Strings: {len(exe_strings)}\n")
        f.write(f"  Source: SLPM_86.053 executable data section\n\n")

        f.write("DIALOGUE REGIONS:\n")
        for item in region_summary:
            f.write(f"  {item['file']}: {item['string_count']} strings\n")

        f.write(f"\n  Total dialogue regions: {len(region_summary)}\n")
        f.write(f"  Total dialogue strings: {sum(len(s) for s in disc_regions.values())}\n\n")

        f.write("CHARACTER FILES:\n")
        for char_name, strings in by_character.items():
            if strings:
                f.write(f"  character_{char_name}.txt: {len(strings)} lines\n")

        total = len(exe_strings) + sum(len(s) for s in disc_regions.values())
        f.write(f"\n" + "=" * 80 + "\n")
        f.write(f"GRAND TOTAL: {total:,} strings extracted\n")
        f.write("=" * 80 + "\n")

    # Save metadata as JSON
    metadata = {
        "extraction_date": "2026-01-19",
        "executable": {
            "path": "disks/SLPM_86.053",
            "string_count": len(exe_strings)
        },
        "disc_regions": region_summary,
        "characters": {
            char_name: len(strings)
            for char_name, strings in by_character.items()
            if strings
        },
        "total_strings": total
    }

    with open(output_dir / 'extraction_metadata.json', 'w', encoding='utf-8') as f:
        json.dump(metadata, f, ensure_ascii=False, indent=2)

def main():
    # Paths
    project_root = Path(__file__).parent.parent
    exe_path = project_root / "disks" / "SLPM_86.053"
    disc_path = project_root / "disks" / "Tokimeki Memorial - Forever with You (Japan) (PlayStation the Best) (Track 1).bin"
    output_dir = project_root / "output" / "script"

    # Check files exist
    if not exe_path.exists():
        print(f"ERROR: Executable not found: {exe_path}")
        return 1

    if not disc_path.exists():
        print(f"ERROR: Disc image not found: {disc_path}")
        return 1

    print("=" * 80)
    print("TOKIMEKI MEMORIAL - SCRIPT EXTRACTOR")
    print("=" * 80)
    print()

    # Extract from executable
    exe_strings = extract_from_executable(exe_path)

    # Extract from disc
    disc_regions = extract_from_disc_sectors(disc_path)

    # Save organized files
    save_script_files(exe_strings, disc_regions, output_dir)

    print("\n" + "=" * 80)
    print("EXTRACTION COMPLETE!")
    print("=" * 80)
    print(f"\nOutput directory: {output_dir}")
    print(f"  - system_ui_text.txt: System/UI strings")
    print(f"  - dialogue_sectors_*.txt: Dialogue by disc region")
    print(f"  - character_*.txt: Dialogue by character")
    print(f"  - extraction_summary.txt: Complete summary")
    print(f"  - extraction_metadata.json: Machine-readable metadata")

    return 0

if __name__ == "__main__":
    sys.exit(main())
