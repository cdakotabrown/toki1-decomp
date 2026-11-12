#!/usr/bin/env python3
"""Scan all semi-matched overlay functions and update status.

For each function currently marked as 'semi_matched':
- Run test_overlay_match.py --fix-relocs
- Update to 'byte_perfect' if test passes
- Update to 'reloc_matched' if 100% masked match (reloc-only diffs)
- Record high-pct functions for manual analysis

Usage:
    python3 tools/overlay/scan_and_update_status.py [--overlay OVERLAY] [--dry-run]
"""
import json, subprocess, re, sys, time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
STATUS_FILE = ROOT / 'config/overlay_match_status.json'

def scan_overlay(overlay, funcs, dry_run=False):
    """Test all funcs in an overlay and return results."""
    bp_funcs = []
    reloc_funcs = []
    high_funcs = []
    
    for func in funcs:
        try:
            r = subprocess.run(
                ['python3', 'tools/overlay/test_overlay_match.py',
                 overlay, func, '--fix-relocs'],
                capture_output=True, text=True, timeout=15,
                cwd=str(ROOT)
            )
            if 'BYTE-PERFECT' in r.stdout:
                bp_funcs.append(func)
            elif 'RELOC-ONLY' in r.stdout:
                m_exact = re.search(r'(\d+\.\d+)%\s+exact', r.stdout)
                exact_pct = float(m_exact.group(1)) if m_exact else 0
                reloc_funcs.append((func, exact_pct))
            else:
                m_exact = re.search(r'(\d+\.\d+)%\s+exact', r.stdout)
                m_masked = re.search(r'Masked:\s+\d+/\d+\s+\((\d+\.\d+)%\)', r.stdout)
                
                exact_pct = float(m_exact.group(1)) if m_exact else 0
                masked_pct = float(m_masked.group(1)) if m_masked else exact_pct
                
                if masked_pct == 100.0 and exact_pct < 100.0:
                    reloc_funcs.append((func, exact_pct))
                elif exact_pct >= 90:
                    high_funcs.append((func, exact_pct))
        except Exception as e:
            pass
    
    return bp_funcs, reloc_funcs, high_funcs


def main():
    dry_run = '--dry-run' in sys.argv
    target_overlay = None
    for i, arg in enumerate(sys.argv):
        if arg == '--overlay' and i + 1 < len(sys.argv):
            target_overlay = sys.argv[i + 1]
    
    with open(STATUS_FILE) as f:
        status = json.load(f)
    
    # Collect semi-matched functions
    work = []
    for overlay in sorted(status.keys()):
        if target_overlay and overlay != target_overlay:
            continue
        for func, info in status[overlay].items():
            if info.get('status') == 'semi_matched':
                work.append((overlay, func))
    
    # Group by overlay
    by_overlay = {}
    for overlay, func in work:
        by_overlay.setdefault(overlay, []).append(func)
    
    print(f"Scanning {len(work)} semi-matched functions across {len(by_overlay)} overlays")
    if dry_run:
        print("DRY RUN — no changes will be made")
    print()
    
    total_bp = 0
    total_reloc = 0
    total_high = 0
    
    for overlay in sorted(by_overlay.keys()):
        funcs = by_overlay[overlay]
        bp, reloc, high = scan_overlay(overlay, funcs, dry_run)
        
        if bp or reloc or high:
            print(f"{overlay}: {len(funcs)} tested → {len(bp)} BP, {len(reloc)} reloc, {len(high)} high-pct")
        
        # Update status
        for func in bp:
            if not dry_run and overlay in status and func in status[overlay]:
                status[overlay][func]['status'] = 'byte_perfect'
                status[overlay][func]['exact_pct'] = 100.0
                status[overlay][func]['reloc_pct'] = 100.0
            total_bp += 1
        
        for func, pct in reloc:
            if not dry_run and overlay in status and func in status[overlay]:
                status[overlay][func]['status'] = 'reloc_matched'
                status[overlay][func]['exact_pct'] = pct
                status[overlay][func]['reloc_pct'] = 100.0
            total_reloc += 1
        
        for func, pct in high:
            total_high += 1
    
    if not dry_run:
        with open(STATUS_FILE, 'w') as f:
            json.dump(status, f, indent=2)
    
    print(f"\n{'='*60}")
    print(f"Updated: {total_bp} → byte_perfect, {total_reloc} → reloc_matched")
    print(f"High-pct for manual analysis: {total_high}")
    print(f"{'='*60}")


if __name__ == '__main__':
    main()
