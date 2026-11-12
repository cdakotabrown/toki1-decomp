#!/usr/bin/env python3
"""
Categorize main binary functions for PC port parity effort.
Categories:
  B - Pure Logic (no GPU/SPU/CD calls)
  C - Rendering (GPU/GTE/display functions)
  D - Audio (SPU/sound functions)
"""

import os
import re
import json
import sys

# Rendering patterns (Category C)
RENDERING_PATTERNS = [
    # PsyQ GPU primitives
    r'\bSetSprt\b', r'\bSetTile\b', r'\bSetPoly[FG][34T]?\b', r'\bAddPrim\b',
    r'\bDrawOTag\b', r'\bDR_MODE\b', r'\bDR_TPAGE\b',
    # VRAM operations
    r'\bClearImage\b', r'\bLoadImage\b', r'\bStoreImage\b', r'\bMoveImage\b',
    r'\bsetRECT\b',
    # Primitive types
    r'\bPOLY_[FG][34T]\b', r'\bSPRT\b', r'\bTILE[_0-9]*\b', r'\bLINE_[FG][234]?\b',
    # GTE functions
    r'\bGTE_\w+', r'\bgte_\w+',
    # Gs library (graphics)
    r'\bGs[A-Z]\w+',
    # GPU-related
    r'\bgpu_\w+', r'\bGPU_\w+',
    # OT (ordering table)
    r'\bOT_\w+', r'\bot_\w+',
    # Display/sprite related
    r'\bsprite_set_\w+', r'\b_sprite_set_\w+',
    # TMD model rendering
    r'\bGsTMD\w+',
    # Display buffer
    r'\bdisp_buf\w*', r'\bDISPENV\b', r'\bDRAWENV\b',
    r'\bSetDefDispEnv\b', r'\bSetDefDrawEnv\b',
    r'\bPutDispEnv\b', r'\bPutDrawEnv\b',
]

# Audio patterns (Category D)
AUDIO_PATTERNS = [
    # SPU functions
    r'\bSpu[A-Z]\w+', r'\b_spu_\w+',
    # Sound sequences
    r'\bSsSeq\w+', r'\bSsVab\w+', r'\bSsStart\b', r'\bSsEnd\b',
    # Sound VM
    r'\b_svm_\w+', r'\b_snd_\w+',
    # Sound driver
    r'\bSD_\w+',
    # Voice functions
    r'\bVoice\w+', r'\bNoteOn\b', r'\bNoteOff\b',
    # Portamento/Modulation
    r'\bContPortamento\b', r'\bContModulation\b',
]

# CD/IO patterns (could be separate category, for now include in B as they're logic)
CD_PATTERNS = [
    r'\bCd[A-Z]\w+', r'\bCD_\w+', r'\b_cd_\w+',
    r'\b_bu_\w+', r'\bMemCard\w*', r'\bmemcard\w*',
]

def compile_patterns(pattern_list):
    """Compile a list of regex patterns into a single compiled pattern."""
    combined = '|'.join(f'({p})' for p in pattern_list)
    return re.compile(combined)

def categorize_file(filepath, rendering_re, audio_re):
    """
    Categorize a single file.
    Returns tuple: (has_gpu, has_spu, category)
    """
    try:
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
    except:
        return (False, False, 'B')

    has_gpu = bool(rendering_re.search(content))
    has_spu = bool(audio_re.search(content))

    if has_gpu and has_spu:
        # Both - prioritize GPU
        category = 'C'
    elif has_gpu:
        category = 'C'
    elif has_spu:
        category = 'D'
    else:
        category = 'B'

    return (has_gpu, has_spu, category)

def get_function_name(filepath):
    """Extract function name from filepath."""
    basename = os.path.basename(filepath)
    return os.path.splitext(basename)[0]

def main():
    base_dir = '.'
    matched_dir = os.path.join(base_dir, 'src/main/matched')
    semi_dir = os.path.join(base_dir, 'src/main/semi-matched')
    output_file = os.path.join(base_dir, 'config/pc_port_function_status.json')

    # Compile patterns
    rendering_re = compile_patterns(RENDERING_PATTERNS)
    audio_re = compile_patterns(AUDIO_PATTERNS)

    results = {
        'summary': {
            'category_b_pure_logic': 0,
            'category_c_rendering': 0,
            'category_d_audio': 0,
            'total_matched': 0,
            'total_semi_matched': 0,
            'duplicates': [],  # Functions in both matched and semi-matched
        },
        'functions': {}
    }

    # Track seen function names for duplicate detection
    seen_funcs = {}

    # Process matched files
    for filename in sorted(os.listdir(matched_dir)):
        if not filename.endswith('.c'):
            continue

        filepath = os.path.join(matched_dir, filename)
        func_name = get_function_name(filepath)
        has_gpu, has_spu, category = categorize_file(filepath, rendering_re, audio_re)

        seen_funcs[func_name] = 'matched'
        results['functions'][func_name] = {
            'category': category,
            'ps1_source': f'src/main/matched/{filename}',
            'source_type': 'matched',
            'calls_gpu': has_gpu,
            'calls_spu': has_spu,
        }

        results['summary']['total_matched'] += 1
        if category == 'B':
            results['summary']['category_b_pure_logic'] += 1
        elif category == 'C':
            results['summary']['category_c_rendering'] += 1
        elif category == 'D':
            results['summary']['category_d_audio'] += 1

    # Process semi-matched files
    for filename in sorted(os.listdir(semi_dir)):
        if not filename.endswith('.c'):
            continue

        filepath = os.path.join(semi_dir, filename)
        func_name = get_function_name(filepath)
        has_gpu, has_spu, category = categorize_file(filepath, rendering_re, audio_re)

        # Check for duplicate
        if func_name in seen_funcs:
            results['summary']['duplicates'].append({
                'name': func_name,
                'matched_source': f'src/main/matched/{func_name}.c',
                'semi_matched_source': f'src/main/semi-matched/{filename}',
            })
            # Skip - keep matched version
            results['summary']['total_semi_matched'] += 1
            continue

        seen_funcs[func_name] = 'semi_matched'
        results['functions'][func_name] = {
            'category': category,
            'ps1_source': f'src/main/semi-matched/{filename}',
            'source_type': 'semi_matched',
            'calls_gpu': has_gpu,
            'calls_spu': has_spu,
        }

        results['summary']['total_semi_matched'] += 1
        if category == 'B':
            results['summary']['category_b_pure_logic'] += 1
        elif category == 'C':
            results['summary']['category_c_rendering'] += 1
        elif category == 'D':
            results['summary']['category_d_audio'] += 1

    # Write output
    with open(output_file, 'w') as f:
        json.dump(results, f, indent=2, sort_keys=False)

    # Recalculate category counts from functions dict
    results['summary']['category_b_pure_logic'] = sum(1 for v in results['functions'].values() if v['category'] == 'B')
    results['summary']['category_c_rendering'] = sum(1 for v in results['functions'].values() if v['category'] == 'C')
    results['summary']['category_d_audio'] = sum(1 for v in results['functions'].values() if v['category'] == 'D')

    # Print summary
    print(f"Function Categorization Complete")
    print(f"================================")
    print(f"Category B (Pure Logic):  {results['summary']['category_b_pure_logic']}")
    print(f"Category C (Rendering):   {results['summary']['category_c_rendering']}")
    print(f"Category D (Audio):       {results['summary']['category_d_audio']}")
    print(f"--------------------------------")
    print(f"Total Matched Files:      {results['summary']['total_matched']}")
    print(f"Total Semi-Matched Files: {results['summary']['total_semi_matched']}")
    print(f"Unique Functions:         {len(results['functions'])}")
    if results['summary']['duplicates']:
        print(f"Duplicates (kept matched): {len(results['summary']['duplicates'])}")
        for dup in results['summary']['duplicates']:
            print(f"  - {dup['name']}")
    print(f"\nOutput written to: {output_file}")

if __name__ == '__main__':
    main()
