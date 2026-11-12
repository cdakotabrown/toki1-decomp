#!/usr/bin/env python3
"""
Extract and analyze ALL functions from Tokimeki Memorial executable.

This script works around the splat/spimdisasm bug by using objdump directly.
It identifies all functions, classifies them by complexity, and generates
comprehensive metadata for decompilation planning.

Enhanced with undeclared data reference detection (based on Silent Hill func_rater.py).

Usage:
    python3 tools/extract_all_functions.py [--output-dir DIR] [--verbose]
    python3 tools/extract_all_functions.py --include-dir include --verbose
"""

import argparse
import json
import re
import struct
import subprocess
import sys
from pathlib import Path
from typing import List, Dict, Tuple, Optional, Set
from dataclasses import dataclass, asdict, field
from collections import defaultdict

@dataclass
class Function:
    """Metadata for a single function."""
    name: str
    vram_start: int
    vram_end: int
    file_offset: int
    size: int
    num_instructions: int
    has_prologue: bool
    has_epilogue: bool
    num_jal_calls: int
    num_branches: int
    num_loads: int
    num_stores: int
    complexity: str  # 'leaf', 'simple', 'medium', 'complex'
    category: str    # 'wrapper', 'logic', 'unknown'
    disassembly: List[str]
    # New fields for enhanced difficulty scoring
    data_refs: Set[str] = field(default_factory=set)
    undeclared_data_refs: Set[str] = field(default_factory=set)
    num_loops: int = 0
    difficulty_score: float = 0.0

    def to_dict(self):
        """Convert to dictionary for JSON serialization."""
        d = asdict(self)
        d['vram_start'] = f"0x{self.vram_start:08X}"
        d['vram_end'] = f"0x{self.vram_end:08X}"
        d['file_offset'] = f"0x{self.file_offset:08X}"
        # Convert sets to lists for JSON
        d['data_refs'] = sorted(list(self.data_refs))
        d['undeclared_data_refs'] = sorted(list(self.undeclared_data_refs))
        return d


def extract_symbols_from_headers(include_dir: str) -> Set[str]:
    """
    Extract declared symbols from header files using regex.

    Scans all .h files in include_dir for variable/function declarations
    and #define macros.
    """
    symbols = set()
    include_path = Path(include_dir)

    if not include_path.exists():
        print(f"Warning: Include directory '{include_dir}' not found")
        return symbols

    header_files = list(include_path.rglob('*.h'))
    print(f"Scanning {len(header_files)} header files for declared symbols...")

    for header in header_files:
        try:
            with open(header, 'r', encoding='utf-8', errors='ignore') as f:
                content = f.read()

                # Remove single-line comments
                content = re.sub(r'//.*?$', '', content, flags=re.MULTILINE)
                # Remove multi-line comments
                content = re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)

                # Pattern to match variable/function declarations
                # Matches: type name; or type name[]; or type name(...);
                # Handles: extern, static, const, volatile modifiers
                decl_pattern = r'\b(?:extern\s+)?(?:static\s+)?(?:const\s+)?(?:volatile\s+)?'
                decl_pattern += r'(?:unsigned\s+|signed\s+)?'
                decl_pattern += r'(?:struct\s+\w+|union\s+\w+|enum\s+\w+|\w+)'
                decl_pattern += r'\s+\**\s*(\w+)\s*(?:[;\[\(])'

                matches = re.findall(decl_pattern, content)
                symbols.update(matches)

                # Also catch #define macros (data references might be macros)
                define_pattern = r'#define\s+(\w+)'
                define_matches = re.findall(define_pattern, content)
                symbols.update(define_matches)

        except Exception as e:
            pass

    return symbols


def extract_data_refs_from_disasm(disasm: List[str]) -> Set[str]:
    """
    Extract data symbol references from disassembly lines.

    Looks for:
    - lui/addiu pairs loading addresses (0x800xxxxx patterns)
    - Direct symbol references if using splat-style disassembly
    """
    data_refs = set()

    for line in disasm:
        # Look for lui loading high part of address (0x800x = VRAM)
        # Pattern: lui $reg, 0x800x
        lui_match = re.search(r'lui\s+\$?\w+,\s*0x(800[0-9a-fA-F])', line, re.IGNORECASE)
        if lui_match:
            high = lui_match.group(1)
            # This is a potential data reference to address 0x800x0000
            data_refs.add(f"D_{high.upper()}0000")

        # Look for %hi(symbol) and %lo(symbol) if present
        hi_lo_pattern = r'%(?:hi|lo)\(([A-Za-z_][A-Za-z0-9_]*)\)'
        hi_lo_matches = re.findall(hi_lo_pattern, line)
        data_refs.update(hi_lo_matches)

        # Look for D_80xxxxxx style symbols
        d_pattern = r'\b(D_80[0-9A-Fa-f]{6})\b'
        d_matches = re.findall(d_pattern, line)
        data_refs.update(d_matches)

        # Look for g_Xxxx style global symbols
        g_pattern = r'\b(g_[A-Za-z][A-Za-z0-9_]*)\b'
        g_matches = re.findall(g_pattern, line)
        data_refs.update(g_matches)

    return data_refs


def count_loops_in_disasm(disasm: List[str]) -> int:
    """
    Count backward branches (loops) in disassembly.

    A backward branch is a branch to an earlier address, indicating a loop.
    """
    loop_count = 0

    # Track addresses and find backward branches
    addresses = []
    for line in disasm:
        # Extract address from line (format: "offset: bytes instruction")
        match = re.match(r'\s*([0-9a-fA-F]+):', line)
        if match:
            addr = int(match.group(1), 16)
            addresses.append(addr)

    for i, line in enumerate(disasm):
        # Look for branch instructions
        if re.search(r'\b(beq|bne|blez|bgtz|bltz|bgez|beqz|bnez|bnel|beql)\b', line, re.IGNORECASE):
            # Extract target from the end of the line
            # objdump format: "addr: bytes  bxx reg,reg,target"
            parts = line.split()
            if len(parts) >= 1:
                # Target is usually last part, might be hex
                target_str = parts[-1]
                try:
                    # Could be 0xXXXX or just XXXX
                    target_str = target_str.replace('0x', '')
                    target = int(target_str, 16)
                    # Current address
                    if i < len(addresses):
                        current = addresses[i]
                        if target < current:
                            loop_count += 1
                except ValueError:
                    pass

    return loop_count


def calculate_difficulty_score(func: 'Function', declared_symbols: Set[str]) -> float:
    """
    Calculate difficulty score for a function.

    Lower score = easier to decompile.
    Based on Silent Hill func_rater.py scoring system.
    """
    difficulty = 0.0

    # Size factor: gentle curve, larger functions are harder
    size_lines = func.num_instructions
    if size_lines < 50:
        difficulty += size_lines * 0.1
    elif size_lines < 100:
        difficulty += 5 + (size_lines - 50) * 0.2
    elif size_lines < 300:
        difficulty += 15 + (size_lines - 100) * 0.25
    else:
        difficulty += 55 + (size_lines - 300) * 0.30

    # Branch complexity (loops are harder)
    if func.num_loops > 0:
        difficulty += 8.0 + (func.num_loops * 6.0)

    # Regular branches add some difficulty
    difficulty += func.num_branches * 2.0

    # JAL calls add complexity
    difficulty += func.num_jal_calls * 3.0

    # Undeclared data references: highly variable difficulty
    # Could be simple u32 or complex struct
    difficulty += len(func.undeclared_data_refs) * 8.0

    # Incomplete functions (missing prologue/epilogue) are harder
    if not func.has_prologue:
        difficulty += 50.0
    if not func.has_epilogue:
        difficulty += 50.0

    # Leaf functions are generally easier
    if func.num_jal_calls == 0:
        difficulty -= 20.0

    return max(0.0, difficulty)


class FunctionExtractor:
    """Extract and analyze functions from PS1 executable."""

    def __init__(self, exe_path: str, include_dir: str = 'include', verbose: bool = False):
        self.exe_path = Path(exe_path)
        self.include_dir = include_dir
        self.verbose = verbose
        self.functions: List[Function] = []
        self.declared_symbols: Set[str] = set()

        # Read the entire executable
        with open(self.exe_path, 'rb') as f:
            self.data = f.read()

        # Parse PS-X EXE header
        self._parse_header()

        # Load declared symbols from headers
        if include_dir:
            self.declared_symbols = extract_symbols_from_headers(include_dir)
            print(f"Found {len(self.declared_symbols)} declared symbols")

    def _parse_header(self):
        """Parse PS-X EXE header."""
        def read_u32(offset):
            return struct.unpack('<I', self.data[offset:offset+4])[0]

        self.text_file_start = read_u32(0x08)
        self.text_size = read_u32(0x0C)
        self.pc_entry = read_u32(0x10)
        self.gp = read_u32(0x14)
        self.text_vram_start = read_u32(0x18)

        self.text_file_end = self.text_file_start + self.text_size
        self.text_vram_end = self.text_vram_start + self.text_size

        if self.verbose:
            print(f"Text section: file 0x{self.text_file_start:X} - 0x{self.text_file_end:X}")
            print(f"Text section: VRAM 0x{self.text_vram_start:08X} - 0x{self.text_vram_end:08X}")
            print(f"Entry point: 0x{self.pc_entry:08X}")

    def _file_offset_to_vram(self, offset: int) -> int:
        """Convert file offset to VRAM address."""
        return self.text_vram_start + (offset - self.text_file_start)

    def _vram_to_file_offset(self, vram: int) -> int:
        """Convert VRAM address to file offset."""
        return self.text_file_start + (vram - self.text_vram_start)

    def find_function_boundaries(self) -> List[Tuple[int, int]]:
        """
        Find all function boundaries by scanning for prologues and epilogues.

        Returns list of (start_offset, end_offset) tuples.
        """
        boundaries = []
        offset = self.text_file_start

        while offset < self.text_file_end - 4:
            word = struct.unpack('<I', self.data[offset:offset+4])[0]

            # Check for function prologue: addiu $sp, $sp, -X
            if (word & 0xFFFF0000) == 0x27BD0000:
                imm = word & 0xFFFF
                if imm >= 0xFF00:  # Negative immediate (stack allocation)
                    # Found potential function start
                    func_start = offset

                    # Look for jr $ra within next 1000 bytes
                    for i in range(4, min(1000, self.text_file_end - offset), 4):
                        check_offset = offset + i
                        check_word = struct.unpack('<I', self.data[check_offset:check_offset+4])[0]

                        # jr $ra
                        if check_word == 0x03E00008:
                            # Found function end (include delay slot)
                            func_end = check_offset + 8
                            boundaries.append((func_start, func_end))
                            break

            offset += 4

        # Sort and remove duplicates
        boundaries = sorted(set(boundaries))

        # Filter out overlapping functions (keep larger one)
        filtered = []
        for start, end in boundaries:
            overlaps = False
            for i, (f_start, f_end) in enumerate(filtered):
                if not (end <= f_start or start >= f_end):
                    # Overlaps - keep larger function
                    if (end - start) > (f_end - f_start):
                        filtered[i] = (start, end)
                    overlaps = True
                    break
            if not overlaps:
                filtered.append((start, end))

        return sorted(filtered)

    def disassemble_function(self, file_offset: int, size: int) -> List[str]:
        """
        Disassemble a function using objdump.

        Returns list of disassembled instruction lines.
        """
        # Extract function bytes
        func_data = self.data[file_offset:file_offset+size]

        # Write to temp file
        temp_file = Path('/tmp/func_temp.bin')
        with open(temp_file, 'wb') as f:
            f.write(func_data)

        # Disassemble with objdump
        try:
            result = subprocess.run(
                ['mipsel-linux-gnu-objdump', '-D', '-b', 'binary',
                 '-m', 'mips:3000', str(temp_file)],
                capture_output=True,
                text=True,
                check=True
            )

            # Parse output
            lines = result.stdout.split('\n')
            disasm = []
            in_section = False

            for line in lines:
                if '<.data>:' in line:
                    in_section = True
                    continue
                if in_section and line.strip():
                    # Extract instruction part (skip address and bytes)
                    parts = line.split('\t')
                    if len(parts) >= 2:
                        disasm.append(line.strip())

            return disasm

        except subprocess.CalledProcessError as e:
            print(f"Error disassembling at offset 0x{file_offset:X}: {e}", file=sys.stderr)
            return []

    def analyze_function(self, file_offset: int, size: int, disasm: List[str]) -> Dict:
        """
        Analyze function complexity and characteristics.

        Returns dict with analysis results.
        """
        analysis = {
            'has_prologue': False,
            'has_epilogue': False,
            'num_jal_calls': 0,
            'num_branches': 0,
            'num_loads': 0,
            'num_stores': 0,
        }

        for line in disasm:
            # Check for prologue
            if 'addiu' in line and 'sp,sp,' in line and '-' in line:
                analysis['has_prologue'] = True

            # Check for epilogue
            if 'jr' in line and 'ra' in line:
                analysis['has_epilogue'] = True

            # Count JAL calls
            if '\tjal\t' in line:
                analysis['num_jal_calls'] += 1

            # Count branches
            if any(x in line for x in ['\tb', '\tbeq', '\tbne', '\tblez', '\tbgtz',
                                       '\tbltz', '\tbgez', '\tbnez', '\tbeqz']):
                analysis['num_branches'] += 1

            # Count loads
            if any(x in line for x in ['\tlw', '\tlh', '\tlb', '\tlhu', '\tlbu']):
                analysis['num_loads'] += 1

            # Count stores
            if any(x in line for x in ['\tsw', '\tsh', '\tsb']):
                analysis['num_stores'] += 1

        return analysis

    def classify_function(self, analysis: Dict, num_instructions: int) -> Tuple[str, str]:
        """
        Classify function by complexity and category.

        Returns (complexity, category) tuple.
        """
        # Determine complexity
        if analysis['num_jal_calls'] == 0:
            complexity = 'leaf'
        elif num_instructions <= 10:
            complexity = 'simple'
        elif num_instructions <= 30:
            complexity = 'medium'
        else:
            complexity = 'complex'

        # Determine category
        if analysis['num_jal_calls'] == 1 and num_instructions <= 10:
            category = 'wrapper'
        elif analysis['num_jal_calls'] > 0 or analysis['num_branches'] > 0:
            category = 'logic'
        else:
            category = 'unknown'

        return complexity, category

    def extract_all_functions(self) -> List[Function]:
        """
        Extract and analyze all functions in the executable.

        Returns list of Function objects.
        """
        print("Finding function boundaries...")
        boundaries = self.find_function_boundaries()
        print(f"Found {len(boundaries)} potential functions")

        functions = []

        for i, (start_offset, end_offset) in enumerate(boundaries):
            if self.verbose and i % 100 == 0:
                print(f"Processing function {i+1}/{len(boundaries)}...")

            size = end_offset - start_offset
            vram_start = self._file_offset_to_vram(start_offset)
            vram_end = self._file_offset_to_vram(end_offset)

            # Disassemble
            disasm = self.disassemble_function(start_offset, size)
            num_instructions = len([l for l in disasm if '\t' in l])

            # Analyze
            analysis = self.analyze_function(start_offset, size, disasm)
            complexity, category = self.classify_function(analysis, num_instructions)

            # Extract data references
            data_refs = extract_data_refs_from_disasm(disasm)
            undeclared = data_refs - self.declared_symbols

            # Count loops
            num_loops = count_loops_in_disasm(disasm)

            # Create function object
            func = Function(
                name=f"func_{vram_start:08X}",
                vram_start=vram_start,
                vram_end=vram_end,
                file_offset=start_offset,
                size=size,
                num_instructions=num_instructions,
                has_prologue=analysis['has_prologue'],
                has_epilogue=analysis['has_epilogue'],
                num_jal_calls=analysis['num_jal_calls'],
                num_branches=analysis['num_branches'],
                num_loads=analysis['num_loads'],
                num_stores=analysis['num_stores'],
                complexity=complexity,
                category=category,
                disassembly=disasm,
                data_refs=data_refs,
                undeclared_data_refs=undeclared,
                num_loops=num_loops,
            )

            # Calculate difficulty score
            func.difficulty_score = calculate_difficulty_score(func, self.declared_symbols)

            functions.append(func)

        self.functions = functions
        return functions

    def generate_statistics(self) -> Dict:
        """Generate statistics about functions."""
        stats = {
            'total_functions': len(self.functions),
            'by_complexity': defaultdict(int),
            'by_category': defaultdict(int),
            'by_size': {
                'tiny (<=32B)': 0,
                'small (33-64B)': 0,
                'medium (65-128B)': 0,
                'large (129-256B)': 0,
                'huge (>256B)': 0,
            },
            'leaf_functions': 0,
            'avg_size': 0,
            'avg_instructions': 0,
        }

        total_size = 0
        total_instructions = 0

        for func in self.functions:
            stats['by_complexity'][func.complexity] += 1
            stats['by_category'][func.category] += 1

            if func.size <= 32:
                stats['by_size']['tiny (<=32B)'] += 1
            elif func.size <= 64:
                stats['by_size']['small (33-64B)'] += 1
            elif func.size <= 128:
                stats['by_size']['medium (65-128B)'] += 1
            elif func.size <= 256:
                stats['by_size']['large (129-256B)'] += 1
            else:
                stats['by_size']['huge (>256B)'] += 1

            if func.complexity == 'leaf':
                stats['leaf_functions'] += 1

            total_size += func.size
            total_instructions += func.num_instructions

        if self.functions:
            stats['avg_size'] = total_size / len(self.functions)
            stats['avg_instructions'] = total_instructions / len(self.functions)

        # Convert defaultdicts to regular dicts
        stats['by_complexity'] = dict(stats['by_complexity'])
        stats['by_category'] = dict(stats['by_category'])

        return stats

    def get_top_candidates(self, n: int = 50) -> List[Function]:
        """
        Get top N decompilation candidates.

        Uses the enhanced difficulty_score (lower is easier).
        """
        # Sort by difficulty score (lower = easier to decompile)
        sorted_funcs = sorted(self.functions, key=lambda f: f.difficulty_score)
        return sorted_funcs[:n]


def main():
    parser = argparse.ArgumentParser(description='Extract all functions from executable')
    parser.add_argument('--exe', default='disks/SLPS_000.65',
                      help='Path to executable')
    parser.add_argument('--output-dir', default='build/function_analysis',
                      help='Output directory for results')
    parser.add_argument('--include-dir', default='include',
                      help='Directory containing header files for symbol extraction')
    parser.add_argument('--verbose', '-v', action='store_true',
                      help='Verbose output')
    parser.add_argument('--top-n', type=int, default=50,
                      help='Number of top candidates to identify')

    args = parser.parse_args()

    # Create output directory
    output_dir = Path(args.output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    print(f"Extracting functions from {args.exe}")
    print(f"Include directory: {args.include_dir}")
    print()

    # Extract functions
    extractor = FunctionExtractor(args.exe, include_dir=args.include_dir, verbose=args.verbose)
    functions = extractor.extract_all_functions()

    print()
    print(f"Extracted {len(functions)} functions")
    print()

    # Generate statistics
    stats = extractor.generate_statistics()

    print("=== Statistics ===")
    print(f"Total functions: {stats['total_functions']}")
    print()
    print("By complexity:")
    for complexity, count in sorted(stats['by_complexity'].items()):
        print(f"  {complexity:10s}: {count:4d}")
    print()
    print("By category:")
    for category, count in sorted(stats['by_category'].items()):
        print(f"  {category:10s}: {count:4d}")
    print()
    print("By size:")
    for size_range, count in stats['by_size'].items():
        print(f"  {size_range:20s}: {count:4d}")
    print()
    print(f"Leaf functions: {stats['leaf_functions']}")
    print(f"Average size: {stats['avg_size']:.1f} bytes")
    print(f"Average instructions: {stats['avg_instructions']:.1f}")
    print()

    # Get top candidates
    print(f"Identifying top {args.top_n} decompilation candidates...")
    top_candidates = extractor.get_top_candidates(args.top_n)

    # Save full function map
    function_map = {
        'metadata': {
            'executable': str(args.exe),
            'total_functions': len(functions),
            'text_section': {
                'vram_start': f"0x{extractor.text_vram_start:08X}",
                'vram_end': f"0x{extractor.text_vram_end:08X}",
                'size': extractor.text_size,
            }
        },
        'statistics': stats,
        'functions': [f.to_dict() for f in functions]
    }

    map_file = output_dir / 'function_map.json'
    with open(map_file, 'w') as f:
        json.dump(function_map, f, indent=2)
    print(f"Saved function map to {map_file}")

    # Save top candidates
    candidates_data = {
        'top_n': args.top_n,
        'candidates': [f.to_dict() for f in top_candidates]
    }

    candidates_file = output_dir / 'top_candidates.json'
    with open(candidates_file, 'w') as f:
        json.dump(candidates_data, f, indent=2)
    print(f"Saved top candidates to {candidates_file}")

    # Save human-readable candidate list
    candidates_md = output_dir / 'TOP_50_CANDIDATES.md'
    with open(candidates_md, 'w') as f:
        f.write("# Top 50 Decompilation Candidates\n\n")
        f.write("Functions ranked by difficulty score (lower = easier).\n\n")
        f.write("**Scoring factors:**\n")
        f.write("- Size: Larger functions have higher scores\n")
        f.write("- Loops: +8 base, +6 per loop\n")
        f.write("- Branches: +2 each\n")
        f.write("- JAL calls: +3 each\n")
        f.write("- Undeclared data refs: +8 each\n")
        f.write("- Leaf functions: -20 bonus\n\n")
        f.write(f"{'Rank':<5} {'Function':<18} {'Score':<8} {'Size':<7} {'Loops':<6} {'JAL':<5} {'Branch':<7} {'Undecl':<7} {'Type':<10}\n")
        f.write("=" * 100 + "\n")

        for i, func in enumerate(top_candidates, 1):
            f.write(f"{i:<5} {func.name:<18} {func.difficulty_score:6.1f}  "
                   f"{func.size:4d}B   {func.num_loops:4d}  {func.num_jal_calls:3d}  "
                   f"{func.num_branches:5d}   {len(func.undeclared_data_refs):4d}   "
                   f"{func.complexity:<10}\n")

    print(f"Saved candidate list to {candidates_md}")

    # Print summary of undeclared data
    total_undeclared = sum(len(f.undeclared_data_refs) for f in functions)
    total_data_refs = sum(len(f.data_refs) for f in functions)
    print()
    print(f"Data references: {total_data_refs} total, {total_undeclared} undeclared")
    print()
    print("Done!")


if __name__ == '__main__':
    main()
