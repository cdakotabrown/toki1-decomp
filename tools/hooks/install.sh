#!/bin/sh
# Opt-in installer: symlink the reaudit hooks into .git/hooks/ so they are NOT
# forced on anyone. Re-run any time; symlinks are refreshed idempotently.
set -e
d=$(cd "$(dirname "$0")" && pwd)

# --git-path output is relative to the cwd git runs in, so run it from the
# repo toplevel and resolve against that.
top=$(git -C "$d" rev-parse --show-toplevel)
hooks=$(git -C "$top" rev-parse --git-path hooks)
case "$hooks" in /*) ;; *) hooks="$top/$hooks" ;; esac
mkdir -p "$hooks"

for h in post-merge pre-push; do
    ln -sf "$d/$h" "$hooks/$h" && echo "linked $h -> $hooks/$h"
done

# Heads-up: `make install-hooks` sets core.hooksPath=.githooks, which would make
# git ignore these .git/hooks symlinks entirely.
hp=$(git -C "$d" config --get core.hooksPath || true)
[ -n "$hp" ] && echo "WARNING: core.hooksPath=$hp is set; git will ignore .git/hooks/ symlinks."

echo "Done. Uninstall with: rm -f $hooks/post-merge $hooks/pre-push"
