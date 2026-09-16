#!/usr/bin/env python3
import argparse, hashlib, json, struct
from pathlib import Path

def pe(data):
    off=struct.unpack_from('<I',data,0x3c)[0]
    machine,sections,timestamp=struct.unpack_from('<HHI',data,off+4)[:3]
    optional=off+24
    return {'machine':f'0x{machine:04X}','x86':machine==0x14c,'timestamp':timestamp,
            'image_size':struct.unpack_from('<I',data,optional+56)[0],'sections':sections}

def main():
    ap=argparse.ArgumentParser(); ap.add_argument('custom',type=Path); ap.add_argument('reference',type=Path); ap.add_argument('--output',type=Path,required=True); a=ap.parse_args()
    x=a.custom.read_bytes(); y=a.reference.read_bytes(); n=max(len(x),len(y)); ranges=[]; start=None
    for i in range(n):
        different=i>=len(x) or i>=len(y) or x[i]!=y[i]
        if different and start is None:start=i
        if not different and start is not None:ranges.append((start,i));start=None
    if start is not None:ranges.append((start,n))
    def meta(path,data): return {'path':str(path.resolve()),'size':len(data),'sha256':hashlib.sha256(data).hexdigest().upper(),'pe':pe(data)}
    report={'custom':meta(a.custom,x),'reference':meta(a.reference,y),'different_bytes':sum(e-s for s,e in ranges),'different_ranges':len(ranges),
            'ranges':[{'file_offset':f'0x{s:08X}','length':e-s,'custom':x[s:min(e,s+32)].hex(' ').upper(),'reference':y[s:min(e,s+32)].hex(' ').upper()} for s,e in ranges]}
    a.output.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8'); print(json.dumps({k:report[k] for k in ('different_bytes','different_ranges')},indent=2))
if __name__=='__main__':main()
