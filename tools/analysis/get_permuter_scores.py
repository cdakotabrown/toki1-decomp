#!/usr/bin/env python3
"""
Extract permuter scores from both README.md and score_*.c filenames
"""
import re
from pathlib import Path

def get_permuter_score(func_dir):
    """Get best permuter score from directory"""
    # Try README.md first
    readme = func_dir / 'README.md'
    if readme.exists():
        content = readme.read_text()
        match = re.search(r'Score:\s*(\d+)', content)
        if match:
            return int(match.group(1))

    # Try score_*.c filenames
    scores = []
    for f in func_dir.glob('score_*.c'):
        match = re.search(r'score_(\d+)', f.name)
        if match:
            scores.append(int(match.group(1)))

    if scores:
        return min(scores)  # Best score is lowest

    return None

def main():
    permuter_dir = Path('src/permuter_best_results')
    results = []

    for func_dir in sorted(permuter_dir.iterdir()):
        if not func_dir.is_dir():
            continue

        match = re.search(r'func_([0-9A-Fa-f]{8})', func_dir.name)
        if not match:
            continue

        address = match.group(1).upper()
        score = get_permuter_score(func_dir)

        if score is not None:
            results.append((address, score))

    # Output by score
    results.sort(key=lambda x: x[1])

    print("# Permuter Scores for All Tested Functions\n")
    print("| Function | Score |")
    print("|----------|-------|")

    for addr, score in results:
        print(f"| func_{addr} | {score} |")

    print(f"\nTotal tested: {len(results)}")

if __name__ == '__main__':
    main()
