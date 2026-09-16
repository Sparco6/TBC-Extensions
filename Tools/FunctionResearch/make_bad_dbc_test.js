const fs=require('fs'),path=require('path');
const target=process.argv[2];
const b=Buffer.from(fs.readFileSync(path.join(target,'TBCExt_Demo.dbc')));
b[0]=88;fs.writeFileSync(path.join(target,'TBCExt_Bad.dbc'),b);
