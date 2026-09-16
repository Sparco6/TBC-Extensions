// Builds an actual binary WDBC for the native loader, not a Lua sample.
const fs=require('fs'),path=require('path');
const target=process.argv[2];
if(!target)throw Error('Usage: node make_dbc_demo.js output-directory [message]');
const label=process.argv[3]||'Native DBC loaded from disk';
if(label.includes('\0')||Buffer.byteLength(label)>1000)throw Error('Invalid message');
const strings=Buffer.from('\0'+label+'\0Character\\Human\\Male\\HumanMale.m2\0','utf8');
const b=Buffer.alloc(20+24+strings.length);b.write('WDBC');
[2,3,12,strings.length].forEach((x,i)=>b.writeUInt32LE(x,4+i*4));
b.writeUInt32LE(1,20);b.writeUInt32LE(1,24);b.writeFloatLE(1.75,28);
b.writeUInt32LE(2,32);b.writeUInt32LE(Buffer.byteLength(label)+2,36);b.writeFloatLE(2.5,40);
strings.copy(b,44);fs.mkdirSync(target,{recursive:true});
fs.writeFileSync(path.join(target,'TBCExt_Demo.dbc'),b);
console.log('Wrote TBCExt_Demo.dbc: 2 rows; ID, string offset, float. Row 1 message: '+label);
