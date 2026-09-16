"""Generate separate proposed-label data and bounded raw WDBC samples. Never execute IDC."""
import argparse, json, struct, hashlib
from pathlib import Path

def quote(value):
    return json.dumps(str(value), ensure_ascii=True)

def dbc_record(path, limit=5000):
    b=path.read_bytes()
    if len(b)<20 or b[:4]!=b'WDBC':
        raise ValueError(f'{path}: not WDBC')
    count,fields,size,strings=struct.unpack_from('<4I',b,4)
    if not 0<fields<=256 or size!=fields*4 or 20+count*size+strings!=len(b):
        raise ValueError(f'{path}: unsupported or inconsistent WDBC layout')
    rows=[struct.unpack_from('<'+'I'*fields,b,20+i*size) for i in range(min(count,limit))]
    return {'name':path.name,'source':str(path),'count':count,'fields':fields,'rows':rows,'sha':hashlib.sha256(b).hexdigest()}

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--addon',type=Path,required=True);ap.add_argument('--audit',type=Path,required=True);ap.add_argument('--dbc',type=Path,action='append',default=[]);args=ap.parse_args()
    labels={};extras=[]
    for r in json.loads(args.audit.read_text(encoding='utf-8'))['labels']:
        labels.setdefault(int(r['address'],16),[]).append(r['proposed_name'])
        if not r['exact_catalog_start']:
            extras.append('{a='+str(int(r['address'],16))+',n="",g="Not a catalog function start",c='+quote(r['section'] or 'Unknown')+',s="CANDIDATE",q="LOW"},')
    data=args.addon/'Data'
    (data/'ProposedLabels.lua').write_text('MWDT_PROPOSED_LABELS={\n'+''.join(f'[{a}]={quote(" / ".join(v))},\n' for a,v in sorted(labels.items()))+'}\nMWDT_PROPOSED_NONFUNCTIONS={\n'+''.join(extras)+'}\n',encoding='ascii')
    records=[dbc_record(p) for p in args.dbc]
    out=['MWDT_DBC_SAMPLES={']
    for r in records:
        out.append('{name='+quote(r['name'])+',source='+quote(r['source'])+',sha='+quote(r['sha'])+',count='+str(r['count'])+',fields='+str(r['fields'])+',rows={')
        out.extend('{'+','.join(map(str,row))+'},' for row in r['rows']);out.append('}},')
    out.append('}')
    (data/'DBCSamples.lua').write_text('\n'.join(out)+'\n',encoding='ascii')
    print('Generated',len(labels),'proposed address entries and',len(records),'raw DBC samples')

if __name__=='__main__':main()
