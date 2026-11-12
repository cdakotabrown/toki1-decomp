#!/usr/bin/env python3
"""Function proposal queue, separate from the authoritative symbol registry.

Inventory reads only tracked configuration/source paths; it does not build,
rename, regenerate authoritative reports, or infer semantic names.
Shards contain at most 64 compact entry lines. Refresh preserves proposals by
scope/address, including absorbed-head prologue IDs and other-overlay collisions.
"""
import argparse
from collections import Counter, defaultdict
import hashlib
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[2]
STATES = {'unreviewed', 'proposed', 'approved', 'claimed', 'committed',
          'landed', 'deferred', 'collision', 'gate_failed', 'stale'}
FUNCTION_KINDS = {'matched_c', 'asm_only', 'head_fragment', 'covered'}
SHARD_SIZE = 64


def load_json(path):
    return json.loads(path.read_text(encoding='utf-8'))


def address(value):
    return f'0x{int(value, 16):08X}'


def portable_source(root, source):
    if not source:
        return None
    path = Path(source)
    if not path.is_absolute():
        return path.as_posix()
    try:
        return path.relative_to(root).as_posix()
    except ValueError:
        for directory in ('src', 'asm'):
            marker = f'/{directory}/'
            if marker in source:
                return directory + '/' + source.split(marker, 1)[1]
        return None


def registry_views(root):
    by_id, by_name = {}, {}
    for key, record in load_json(root / 'config/symbol_names.json')['symbols'].items():
        if record.get('kind') != 'func':
            continue
        scope, raw = key.split(':', 1) if ':' in key else ('MAIN', key)
        identity = f'{scope}:{address(raw)}'
        by_id[identity] = record
        by_name[(scope, record['name'])] = identity
    return by_id, by_name


def document_window(root, row, canonical=None):
    """Document reported windows and source context, not compiler-match verdicts."""
    source = root / row['source'] if row['source'] else None
    text = source.read_text(encoding='utf-8') if source and source.is_file() else ''
    start = re.search(r'@SymbolStart\s*:\s*(0x[0-9A-Fa-f]+)', text)
    size = re.search(r'@SymbolSize\s*:\s*(\d+)', text)
    row['source_sha256'] = hashlib.sha256(source.read_bytes()).hexdigest() if text else None
    row['window_origin'] = 'overlay-sym' if row['scope'] != 'MAIN' else 'main-audit'
    if row['scope'] == 'MAIN' and row['kind'] != 'covered':
        if canonical:
            row['canonical_id'] = 'MAIN:' + address(canonical['address'])
            row['window_address'] = address(canonical['address'])
            row['size'] = canonical.get('size', row['size'])
            row['window_origin'] = 'level3-source-map'
        if start:
            row['window_address'] = address(start.group(1))
            row['window_origin'] = 'source-directive'
        if size:
            row['size'] = int(size.group(1))
    row['target_window_address'] = address(start.group(1)) if start else row['window_address']
    row['target_size'] = int(size.group(1)) if size else row['size']
    row['window_end'] = (f"0x{int(row['window_address'], 16) + row['size']:08X}"
                         if row['size'] is not None else None)
    row['words'] = row['size'] // 4 if row['size'] is not None and row['size'] % 4 == 0 else None
    # Tokens are best-effort source context, not a proven static/dynamic call graph.
    cleaned = re.sub(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"', '', text, flags=re.S)
    definition = re.search(r'\b' + re.escape(row['current_name']) + r'\s*\([^;{}]*\)\s*\{', cleaned)
    body = ''
    if definition:
        depth = 1
        position = definition.end()
        begin = position
        while position < len(cleaned) and depth:
            depth += (cleaned[position] == '{') - (cleaned[position] == '}')
            position += 1
        body = cleaned[begin:position - 1]
    tokens = set(re.findall(r'\b([A-Za-z_][A-Za-z_0-9]*)\s*\(', body))
    tokens -= {'if', 'while', 'for', 'switch', 'sizeof', row['current_name']}
    if source and source.suffix == '.s':
        tokens.update(re.findall(r'\bjal\s+([A-Za-z_][A-Za-z_0-9]*|0x[0-9A-Fa-f]+)', cleaned))
    row['call_tokens_best_effort'] = sorted(tokens)
    row['indirect_call_syntax'] = bool(re.search(r'\]\s*\(|\(\s*\*', body))


def proposal(record, name, kind):
    audit = (record or {}).get('audit') or {}
    landed = bool(record and record.get('landed') and record['name'] == name)
    state = 'landed' if landed and audit.get('verdict') != 'refuted' else 'unreviewed'
    if kind == 'covered':
        state = 'deferred'
    return {
        'state': state, 'candidates': [], 'evidence': [], 'uncertainty': [],
        'proposed_by': None, 'reviewed_by': None,
    }


def inventory(root):
    by_id, by_name = registry_views(root)
    level3 = {address(row['address']): row for row in
              load_json(root / 'config/level3_functions.json')['functions']}
    by_source = {row['source']: row for row in level3.values() if row.get('source')}
    rows = {}
    audit = load_json(root / 'config/main_function_audit_by_addr.json')['addresses']
    for raw, item in audit.items():
        kind = item['kind']
        if kind not in FUNCTION_KINDS:
            continue
        addr = address(raw)
        identity = f'MAIN:{addr}'
        live = level3.get(addr, {})
        source = portable_source(root, live.get('source') or item.get('source'))
        name = live.get('name') or (Path(source).stem if source else item['aliases'][0])
        record = by_id.get(identity)
        rows[identity] = {
            'id': identity, 'scope': 'MAIN', 'address': addr,
            'window_address': addr, 'size': live.get('size', item.get('size')),
            'current_name': name, 'source': source, 'kind': kind,
            'enclosed_by': item.get('enclosed_by'),
            'registry_audit': (record or {}).get('audit'),
            'proposal': proposal(record, name, kind),
        }
        document_window(root, rows[identity], live or by_source.get(source))
        if kind == 'covered':
            rows[identity]['proposal']['uncertainty'] = [
                'Secondary/covered label: establish an independent function window before renaming.'
            ]
    pattern = re.compile(r'^(0x[0-9a-fA-F]+):(\d+):([A-Za-z_][A-Za-z_0-9]*)\s*$')
    for symbols in sorted((root / 'config/overlay').glob('*.sym')):
        scope = symbols.stem
        for line in symbols.read_text(encoding='utf-8').splitlines():
            match = pattern.match(line)
            if not match:
                continue
            start, size, name = match.groups()
            suffix = re.fullmatch(r'func_([0-9a-fA-F]{8})', name)
            identity = by_name.get((scope, name))
            if identity is None:
                identity = f'{scope}:{address(suffix.group(1) if suffix else start)}'
            if identity in rows:
                raise ValueError(f'duplicate identity {identity}')
            source = f'src/overlay/{scope}/{name}.c'
            if not (root / source).is_file():
                source = f'asm/overlay/{scope}/{name}.s'
            record = by_id.get(identity)
            rows[identity] = {
                'id': identity, 'scope': scope, 'address': identity.split(':')[1],
                'window_address': address(start), 'size': int(size),
                'current_name': name, 'source': source, 'kind': 'overlay_function',
                'enclosed_by': None, 'registry_audit': (record or {}).get('audit'),
                'proposal': proposal(record, name, 'overlay_function'),
            }
            document_window(root, rows[identity])
    return rows


def read_queue(directory):
    index = directory / 'index.json'
    if not index.exists():
        return {}
    rows = {}
    for shard in load_json(index)['shards']:
        for entry in load_json(directory / shard)['entries']:
            if entry['id'] in rows:
                raise ValueError(f"duplicate queue ID {entry['id']}")
            rows[entry['id']] = entry
    return rows


def refresh(root, directory):
    previous = read_queue(directory)
    rows = inventory(root)
    records, _ = registry_views(root)
    for identity, entry in rows.items():
        old = previous.get(identity)
        if old is None:
            continue
        live_state = entry['proposal']['state']
        entry['proposal'] = old['proposal']
        changed = any(old.get(field) != entry.get(field)
                      for field in ('current_name', 'window_address', 'size'))
        if not entry['proposal']['candidates'] and live_state == 'landed':
            entry['proposal']['state'] = 'landed'
        elif entry['proposal']['state'] == 'landed':
            if old['current_name'] != entry['current_name']:
                entry['proposal']['state'] = 'stale'
        elif entry['current_name'] in entry['proposal']['candidates']:
            record = records.get(identity)
            if record and record.get('landed') and record['name'] == entry['current_name']:
                entry['proposal']['state'] = 'landed'
        elif changed and entry['proposal']['state'] not in {'unreviewed', 'deferred'}:
            entry['proposal']['state'] = 'stale'
    for identity in previous.keys() - rows.keys():
        entry = previous[identity]
        entry['proposal']['state'] = 'stale'
        entry['proposal']['uncertainty'] = sorted(set(
            entry['proposal']['uncertainty'] + ['No longer in current authoritative inventory.']))
        rows[identity] = entry
    write_queue(directory, rows)
    return rows


def write_if_changed(path, content):
    if path.exists() and path.read_text(encoding='utf-8') == content:
        return
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_suffix(path.suffix + '.tmp')
    temporary.write_text(content, encoding='utf-8')
    temporary.replace(path)


def write_queue(directory, rows):
    grouped = defaultdict(list)
    for identity, entry in sorted(rows.items()):
        if entry['proposal']['state'] not in STATES:
            raise ValueError(f'unknown state for {identity}')
        grouped[entry['scope']].append(entry)
    shards = []
    for scope, entries in sorted(grouped.items()):
        for offset in range(0, len(entries), SHARD_SIZE):
            shard = f'{scope}/{offset // SHARD_SIZE:03d}.json'
            subset = entries[offset:offset + SHARD_SIZE]
            text = '{\n  "schema": 1,\n  "scope": ' + json.dumps(scope) + ',\n  "entries": [\n'
            text += ',\n'.join('    ' + json.dumps(entry, ensure_ascii=False, separators=(',', ':'))
                              for entry in subset)
            text += '\n  ]\n}\n'
            write_if_changed(directory / shard, text)
            shards.append(shard)
    summary = {
        'schema': 1, 'description': 'Proposals only; config/symbol_names.json remains authoritative.',
        'counts': {'total': len(rows), 'states': dict(sorted(Counter(
            row['proposal']['state'] for row in rows.values()).items())),
            'scopes': {scope: len(entries) for scope, entries in sorted(grouped.items())},
            'kinds': dict(sorted(Counter(row['kind'] for row in rows.values()).items()))},
        'shards': shards,
        'window_documentation': {
            'function_records': sum(row['kind'] != 'covered' for row in rows.values()),
            'unique_reported_windows': len({(row['scope'], row['window_address'], row['size'])
                                           for row in rows.values() if row['kind'] != 'covered'}),
            'missing_function_size': sum(row['size'] is None for row in rows.values()
                                         if row['kind'] != 'covered'),
        },
    }
    canonical = json.dumps(rows, sort_keys=True, ensure_ascii=False).encode('utf-8')
    summary['inventory_sha256'] = hashlib.sha256(canonical).hexdigest()
    write_if_changed(directory / 'index.json', json.dumps(summary, indent=2) + '\n')


def import_proposals(root, rows, batch_path, result_path, proposed_by):
    """Import unapproved suggestions only, refusing stale source snapshots."""
    batch = {entry['id']: entry for entry in load_json(batch_path)['entries']}
    incoming = load_json(result_path)['proposals']
    seen = set()
    for item in incoming:
        identity = item['id']
        if identity in seen or identity not in batch or identity not in rows:
            raise ValueError(f'unknown or duplicate proposal ID {identity}')
        seen.add(identity)
        original = batch[identity]
        source = root / original['source']
        if hashlib.sha256(source.read_bytes()).hexdigest() != original['source_sha256']:
            raise ValueError(f'stale source snapshot for {identity}')
        if rows[identity]['current_name'] != original['current_name']:
            raise ValueError(f'stale name snapshot for {identity}')
        state = item['state']
        candidates = item['candidates']
        if state not in {'proposed', 'deferred'} or not isinstance(candidates, list):
            raise ValueError(f'invalid unapproved proposal for {identity}')
        if (len(candidates) > 2 or len(set(candidates)) != len(candidates)
                or any(not isinstance(name, str) or re.fullmatch(
                    r'[A-Za-z_][A-Za-z_0-9]*', name) is None for name in candidates)):
            raise ValueError(f'invalid candidates for {identity}')
        if state == 'proposed' and not candidates:
            raise ValueError(f'proposed entry has no candidate: {identity}')
        for field in ('evidence', 'uncertainty'):
            if not isinstance(item[field], list) or any(
                    not isinstance(value, str) for value in item[field]):
                raise ValueError(f'invalid {field} for {identity}')
        if not item['evidence']:
            raise ValueError(f'no evidence for {identity}')
        if rows[identity]['proposal']['state'] == 'landed':
            raise ValueError(f'cannot replace landed record {identity}')
    for item in incoming:
        rows[item['id']]['proposal'] = {
            'state': item['state'], 'candidates': item['candidates'],
            'evidence': item['evidence'], 'uncertainty': item['uncertainty'],
            'proposed_by': proposed_by, 'reviewed_by': None,
        }
    return sorted(batch.keys() - seen)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, default=ROOT)
    parser.add_argument('--queue', type=Path, default=ROOT / 'config/symbol_proposals')
    subparsers = parser.add_subparsers(dest='command', required=True)
    subparsers.add_parser('inventory')
    subparsers.add_parser('summary')
    propose = subparsers.add_parser('propose')
    propose.add_argument('identity')
    propose.add_argument('candidates', nargs='+')
    propose.add_argument('--evidence', action='append', required=True)
    propose.add_argument('--uncertainty', action='append', default=[])
    propose.add_argument('--proposed-by', required=True)
    imported = subparsers.add_parser('import')
    imported.add_argument('batch', type=Path)
    imported.add_argument('results', type=Path)
    imported.add_argument('--proposed-by', required=True)
    args = parser.parse_args()
    if args.command != 'inventory' and not (args.queue / 'index.json').exists():
        print(f'SKIP: no proposal queue at {args.queue} (the queue is not '
              'published with the repository); build one with '
              '`symbol_proposals.py inventory`')
        return
    if args.command == 'inventory':
        rows = refresh(args.root, args.queue)
    else:
        rows = read_queue(args.queue)
        if args.command == 'import':
            missing = import_proposals(args.root, rows, args.batch, args.results, args.proposed_by)
            write_queue(args.queue, rows)
            if missing:
                print(json.dumps({'still_unreviewed': missing}))
        elif args.command == 'propose':
            if len(args.candidates) > 2:
                parser.error('propose one or two candidates, not an unreviewed name dump')
            if len(set(args.candidates)) != len(args.candidates) or any(
                    re.fullmatch(r'[A-Za-z_][A-Za-z_0-9]*', name) is None
                    for name in args.candidates):
                parser.error('candidates must be distinct C identifiers')
            if args.identity not in rows:
                parser.error('unknown inventory ID')
            if rows[args.identity]['proposal']['state'] == 'landed':
                parser.error('landed record needs explicit naming adjudication, not overwrite')
            rows[args.identity]['proposal'] = {
                'state': 'proposed', 'candidates': args.candidates,
                'evidence': args.evidence, 'uncertainty': args.uncertainty,
                'proposed_by': args.proposed_by, 'reviewed_by': None,
            }
            write_queue(args.queue, rows)
    print(json.dumps(dict(sorted(Counter(row['proposal']['state']
                                        for row in rows.values()).items())), indent=2))


if __name__ == '__main__':
    main()
