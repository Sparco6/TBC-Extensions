// Read-only PE evidence collector. String hits/xrefs are leads, not semantic proof.
const fs=require('fs');const exe=fs.readFileSync(process.argv[2]),csv=fs.readFileSync(process.argv[3],'utf8');
const pe=exe.readUInt32LE(0x3c),base=exe.readUInt32LE(pe+24+28),sections=pe+24+exe.readUInt16LE(pe+20),count=exe.readUInt16LE(pe+6);
const ss=[];for(let i=0;i<count;i++){const p=sections+i*40;ss.push({name:exe.subarray(p,p+8).toString().replace(/\0.*/,''),va:base+exe.readUInt32LE(p+12),raw:exe.readUInt32LE(p+20),size:exe.readUInt32LE(p+16)});}
const funcs=[];for(const line of csv.split(/\r?\n/)){const m=line.match(/^"([^"]+)","([0-9A-Fa-f]+)"/);if(m)funcs.push({a:parseInt(m[2],16),n:m[1]});}funcs.sort((a,b)=>a.a-b.a);
function owner(a){let lo=0,hi=funcs.length;while(lo<hi){let m=(lo+hi)>>1;if(funcs[m].a<=a)lo=m+1;else hi=m;}return lo?funcs[lo-1]:null;}
function va(off){for(const s of ss)if(off>=s.raw&&off<s.raw+s.size)return s.va+off-s.raw;return null;}
function refs(addr){const q=Buffer.alloc(4);q.writeUInt32LE(addr);let out=[];for(const s of ss.filter(x=>x.name==='.text'))for(let p=s.raw;(p=exe.indexOf(q,p))>=0&&p<s.raw+s.size;p++)out.push(va(p));return out;}
for(const word of ['BLP1','BLP2','DXT1','DXT3','DXT5','GxTex_Dxt1','GxTex_Dxt3','GxTex_Dxt5']){let pos=0,h=[];while((pos=exe.indexOf(Buffer.from(word+'\0'),pos))>=0){const a=va(pos),x=a?refs(a):[];h.push({va:a&&'0x'+a.toString(16),xrefs:x.map(v=>({at:'0x'+v.toString(16),owner:owner(v)}))});pos+=word.length;}console.log(word,JSON.stringify(h));}
for(const word of ['BLP1','BLP2']){let pos=0,h=[];while((pos=exe.indexOf(Buffer.from(word),pos))>=0){const a=va(pos),s=ss.find(x=>pos>=x.raw&&pos<x.raw+x.size);h.push({va:a&&'0x'+a.toString(16),section:s&&s.name,owner:a&&owner(a)});pos+=4;}console.log(word+'_raw',JSON.stringify(h));}
