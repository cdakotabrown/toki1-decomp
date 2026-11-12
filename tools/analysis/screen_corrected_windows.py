#!/usr/bin/env python3
"""Re-measure OVERRUN functions against their audit-corrected window.

TODO-NEXT item 1 claimed some of the OVERRUN bucket "may need no new C at all"
-- that the C is already right and only the truncated window makes it read as
compiling too long.  This screens that claim: for each candidate it backs the
file up, substitutes @SymbolStart/@SymbolSize from the boundary audit, re-runs
test_single_match, and restores the file whatever happens.

RESULT (2026-08-28, 19 candidates): the strong claim is REFUTED -- not one
becomes a match on the window alone.  The weak claim holds for seven: their
WINDOW OVERRUN verdict disappears, so it was a measurement artefact and not a
codegen problem.

Rerun after regenerating the worklists; the candidate list is the OVERRUN rows
that carry a START HERE column.
"""
import re, subprocess, os, sys
ROOT='.'
CAND=[('0x80059E50','0x80059E48','536','printpix'),('0x80068A14','0x800689FC','684','cal_font_load'),
('0x80075D40','0x80075D38','796','func_80075D40'),('0x8007D9F8','0x8007D9F0','2788','_wipe_main'),
('0x8004555C','0x80045534','252','cdq_command_enqueue'),('0x80079104','0x800790FC','1204','scene_entry_set'),
('0x80053374','0x80053364','1164','grp_table_calc'),('0x800957BC','0x800957AC','560','_spu_init'),
('0x80098FDC','0x80098F98','168','GsInitGraph2'),('0x8009E62C','0x8009E5F4','828','func_8009E62C'),
('0x800994A4','0x80099480','200','GsDefDispBuff'),('0x8009CAC8','0x8009CAB0','176','func_8009CAC8'),
('0x80099374','0x80099370','304','GsSetDrawBuffOffset'),('0x800938BC','0x80093890','264','ContNrpn2'),
('0x8008BE14','0x8008BE0C','176','set_tarao_rect'),('0x8008B2B8','0x8008B290','128','SsSetMVol'),
('0x80092F48','0x80092F18','164','SetControlChange'),('0x8008C810','0x8008C800','424','SpuVmAlloc'),
('0x8009FBBC','0x8009FBAC','264','func_8009FBBC')]
def find(name):
    for d in ('matched','semi-matched'):
        p=os.path.join(ROOT,'src/main',d,name+'.c')
        if os.path.exists(p): return p
    return None
def test(name):
    r=subprocess.run(['python3','tools/analysis/test_single_match.py',name],cwd=ROOT,
                     capture_output=True,text=True,timeout=600)
    e=re.search(r'Exact:\s+(\d+)/(\d+)',r.stdout); f=re.search(r'Fixup:\s+(\d+)',r.stdout)
    ov='OVERRUN' in r.stdout
    return (e.group(1)+'/'+e.group(2) if e else 'ERR', int(f.group(1)) if f else 0, ov)
for addr,start,size,name in CAND:
    p=find(name)
    if not p: print('%-20s NO FILE'%name); continue
    orig=open(p).read()
    before=test(name)
    s=re.sub(r'/\* @Symbol(Start|Size):[^\n]*\n','',orig)
    s=re.sub(r'\n \* @Symbol(Start|Size):[^\n]*','',s)
    s=s.rstrip('\n')+'\n/* @SymbolStart: %s */\n/* @SymbolSize: %s */\n'%(start,size)
    open(p,'w').write(s)
    try: after=test(name)
    finally: open(p,'w').write(orig)
    flag=' <== IMPROVED' if after[0]!=before[0] and after[1]==0 else ''
    print('%-20s before=%-9s fx=%d ov=%d | after=%-9s fx=%d ov=%d%s'%(
        name,before[0],before[1],before[2],after[0],after[1],after[2],flag),flush=True)
