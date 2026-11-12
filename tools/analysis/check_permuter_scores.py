#!/usr/bin/env python3
"""
List permuter scores below a threshold and optionally write a stop-list.

Reuses get_permuter_score from get_permuter_scores.py (DRY).
"""

from __future__ import annotations

import argparse
import importlib.util
import re
from pathlib import Path
from typing import List, Optional, Tuple

ROOT_DIR = Path(__file__).resolve().parents[2]
GET_SCORES_SCRIPT = ROOT_DIR / "tools" / "analysis" / "get_permuter_scores.py"
PERMUTER_DIR = Path("src/permuter_best_results")


def load_get_permuter_score():
    spec = importlib.util.spec_from_file_location("get_permuter_scores", GET_SCORES_SCRIPT)
    if spec is None or spec.loader is None:
        raise RuntimeError(f"Failed to load {GET_SCORES_SCRIPT}")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module.get_permuter_score


def collect_scores() -> List[Tuple[str, Optional[int]]]:
    results = []
    get_permuter_score = load_get_permuter_score()

    for func_dir in sorted(PERMUTER_DIR.iterdir()):
        if not func_dir.is_dir():
            continue

        match = re.search(r"func_([0-9A-Fa-f]{8})", func_dir.name)
        if not match:
            continue

        address = match.group(1).upper()
        score = get_permuter_score(func_dir)
        results.append((f"func_{address}", score))

    return results


def main() -> None:
    parser = argparse.ArgumentParser(description="Report permuter scores below threshold")
    parser.add_argument("--threshold", type=int, default=200)
    parser.add_argument("--output", type=Path, help="Write stop-list file")

    args = parser.parse_args()

    results = collect_scores()
    below = [(func, score) for func, score in results if score is not None and score <= args.threshold]

    below.sort(key=lambda item: item[1])

    print(f"# Permuter Scores <= {args.threshold}\n")
    print("| Function | Score |")
    print("|----------|-------|")
    for func, score in below:
        print(f"| {func} | {score} |")

    print(f"\nTotal <= threshold: {len(below)}")

    if args.output:
        args.output.write_text("\n".join(func for func, _ in below) + "\n")
        print(f"Stop-list written to {args.output}")


if __name__ == "__main__":
    main()
