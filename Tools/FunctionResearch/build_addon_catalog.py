#!/usr/bin/env python3
import argparse,csv
from pathlib import Path
def q(s):return '"'+str(s or '').replace('\\','\\\\').replace('"','\\"').replace('\n',' ')+'"'
ap=argparse.ArgumentParser();ap.add_argument('csv',type=Path);ap.add_argument('output',type=Path);a=ap.parse_args()
rows=list(csv.DictReader(a.csv.open(encoding='utf-8-sig')))
with a.output.open('w',encoding='utf-8',newline='\n') as f:
 f.write('MWDT_NATIVE_FUNCTIONS={\n')
 for r in rows:f.write('{a='+str(int(r['address'],16))+',g='+q(r['ghidra_name'])+',n='+q(r['friendly_name'])+',c='+q(r['category'])+',s='+q(r['status'])+',q='+q(r['confidence'])+',z='+q(r['size'])+'},\n')
 f.write('}\nMWDT_NATIVE_BY_ADDRESS={}\nfor i=1,#MWDT_NATIVE_FUNCTIONS do MWDT_NATIVE_BY_ADDRESS[MWDT_NATIVE_FUNCTIONS[i].a]=MWDT_NATIVE_FUNCTIONS[i] end\n')
print(len(rows))
