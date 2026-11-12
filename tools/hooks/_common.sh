#!/bin/sh
# Shared logic for the opt-in reaudit git hooks (post-merge, pre-push).
# This file is SOURCED, not executed. It must never fail the calling hook:
# every entry point below is designed to return 0 no matter what.
#
# Why this exists: an `asm_only` verdict is a *judgment* made with the tooling
# of the day. When the compiler/hook/harness layer changes, some of those
# judgments can go stale (the retained C now matches byte-perfect = a free win).
# tools/analysis/reaudit_asm_only.py detects that. These hooks run it, but only
# when the relevant layer actually changed, so the common case stays fast.

# Paths that, when touched, can make asm_only verdicts go stale.
REAUDIT_WATCH='tools/ido/ tools/build/ tools/overlay/overlay_postproc.py'

# Repo root, resolved from git so the hook works from any subdir/worktree.
REAUDIT_ROOT=$(git rev-parse --show-toplevel 2>/dev/null) || REAUDIT_ROOT=.

# is_zero_oid OID -> true if OID is empty or all-zeros (a deleted/new ref).
is_zero_oid() {
    [ -z "$(printf '%s' "$1" | tr -d 0)" ]
}

# reaudit_paths_changed A B -> true if any watched path differs between the two
# revisions A and B. Any git error (missing rev, etc.) yields "no change" so the
# hook stays silent rather than noisy.
reaudit_paths_changed() {
    _changed=$(git -C "$REAUDIT_ROOT" diff --name-only "$1" "$2" \
        -- $REAUDIT_WATCH 2>/dev/null)
    [ -n "$_changed" ]
}

# reaudit_run_and_banner -> run `make reaudit-asm`, echo its output, and if it
# reports any GENUINELY STALE verdicts, print a big loud banner. Always 0.
reaudit_run_and_banner() {
    echo "[reaudit-asm hook] compiler/hook layer changed -- re-testing asm_only retained C..."
    _out=$(cd "$REAUDIT_ROOT" && make reaudit-asm 2>&1)
    printf '%s\n' "$_out"
    # Primary signal: the tool's "N GENUINELY STALE" summary line. (The JSON at
    # config/asm_only_reaudit.json carries the machine-readable "stale" array.)
    _stale=$(printf '%s\n' "$_out" | grep 'GENUINELY STALE')
    if [ -n "$_stale" ]; then
        cat <<'BANNER'

################################################################
##                                                            ##
##   ⭐  STALE asm_only VERDICT(S) DETECTED  ⭐                ##
##   Retained C now matches byte-perfect -- PROMOTE THEM!     ##
##   Details: config/asm_only_reaudit.json ("stale" array)   ##
##                                                            ##
################################################################
BANNER
        printf '%s\n' "$_stale"
    fi
    return 0
}
