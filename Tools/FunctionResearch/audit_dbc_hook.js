// Read-only check of the supplied call-site proposal against the custom PE.
const fs=require('fs');
const b=fs.readFileSync(process.argv[2]);
const pe=b.readUInt32LE(0x3c), count=b.readUInt16LE(pe+6), opt=pe+24;
const base=b.readUInt32LE(opt+28), sect=opt+b.readUInt16LE(pe+20);
function offset(va) {
 const rva=va-base;
 for(let i=0;i<count;i++) {
  const s=sect+i*40, start=b.readUInt32LE(s+12), size=b.readUInt32LE(s+16);
  if(rva>=start&&rva<start+size)return b.readUInt32LE(s+20)+rva-start;
 }
 throw Error('Address outside raw sections');
}
const call=0x574e29, at=offset(call);
const target=b[at]===0xe8?call+5+b.readInt32LE(at+1):null;
console.log(JSON.stringify({callSite:'0x574e29',bytes:b.subarray(at,at+5).toString('hex'),
 decodedTarget:target&&'0x'+target.toString(16),matchesProposal:target===0x573c90,
 caveat:'CALL target only; does not verify full calling convention, timing, or stock-table integration'},null,2));
for(const va of [0x573c90,0x72dff0])console.log('0x'+va.toString(16),b.subarray(offset(va),offset(va)+16).toString('hex'));
