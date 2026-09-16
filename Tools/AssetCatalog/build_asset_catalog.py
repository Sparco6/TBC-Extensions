#!/usr/bin/env python3
import argparse
from pathlib import Path
def q(s):return '"'+str(s).replace('\\','\\\\').replace('"','\\"')+'"'
ap=argparse.ArgumentParser();ap.add_argument('roots',nargs='+',type=Path);ap.add_argument('--textures',type=Path,required=True);ap.add_argument('--models',type=Path,required=True);a=ap.parse_args()
tex=[];mod=[]
for root in a.roots:
 for p in root.rglob('*'):
  if not p.is_file():continue
  e=p.suffix.lower(); rel=str(p.relative_to(root)).replace('/','\\'); src=root.name
  if e in ('.blp','.tga','.dds','.png'):
   low=rel.lower(); cats=[('interface\\icons','Icons'),('buttons','Buttons'),('dialogframe','DialogFrame'),('tooltips','Tooltips'),('targetingframe','TargetingFrame'),('paperdoll','PaperDoll'),('spellbook','Spellbook'),('worldmap','WorldMap'),('minimap','Minimap'),('glues','Glues')];cat=next((c for k,c in cats if k in low),'Custom' if 'custom' in low else 'Other');tex.append((rel.rsplit('.',1)[0],p.stem,cat,e[1:],src))
  elif e in ('.m2','.mdx','.mdl'):
   low=rel.lower();cat=next((c for k,c in [('character','Character'),('creature','Creature'),('item','Item'),('world','World'),('doodad','Doodad'),('spell','Spell'),('interface','Interface')] if k in low),'Other');mod.append((rel,p.stem,cat,src))
for path,name,rows,fields in [(a.textures,'MWDT_TEXTURE_CATALOG',tex,5),(a.models,'MWDT_MODEL_CATALOG',mod,4)]:
 path.parent.mkdir(parents=True,exist_ok=True)
 with path.open('w',encoding='utf-8',newline='\n') as f:
  f.write(name+'={\n');[f.write('{'+','.join(q(x) for x in r)+'},\n') for r in sorted(rows)];f.write('}\n')
print('textures',len(tex),'models',len(mod))
