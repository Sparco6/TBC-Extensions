// Read-only input audit: never executes IDC and never alters the native catalog.
const fs = require('fs');
const path = require('path');
const root = path.resolve(__dirname, '../../..');
const source = path.join(root, 'export+search/friendfoundadresses.txt');
const script = fs.readFileSync(source, 'utf8');
const catalog = fs.readFileSync(path.join(root, 'MasterWoWDevTools/Data/NativeFunctions.lua'), 'utf8');
const starts = new Set([...catalog.matchAll(/\{a=(\d+),/g)].map(m => Number(m[1])));
const pe = fs.readFileSync(path.join(root, 'Wow.exe'));
const nt = pe.readUInt32LE(60), base = pe.readUInt32LE(nt + 52);
const sections = [];
const sectionTable = nt + 24 + pe.readUInt16LE(nt + 20);
for (let i=0;i<pe.readUInt16LE(nt+6);i++) {
 const o=sectionTable+40*i;
 sections.push({name:pe.toString('ascii',o,o+8).replace(/\0/g,''), start:base+pe.readUInt32LE(o+12), size:Math.max(pe.readUInt32LE(o+8),pe.readUInt32LE(o+16)), executable:!!(pe.readUInt32LE(o+36)&0x20000000)});
}
const labels = [...script.matchAll(/\b(NameAddr|Data)\(\s*(0x[\dA-Fa-f]+)\s*,\s*"([^"]+)"(?:\s*,\s*"([^"]+)")?/g)].map(m=>{
 const address=Number(m[2]), section=sections.find(s=>address>=s.start && address<s.start+s.size);
 return {address:'0x'+address.toString(16).toUpperCase().padStart(8,'0'), proposed_name:m[3], declaration:m[1], data_type:m[4]||null, section:section?.name||null, executable_section:section?.executable||false, exact_catalog_start:starts.has(address), status:'CANDIDATE', confidence:'LOW', danger_level:'DO_NOT_CALL', source:path.relative(root,source)};
});
const apiText=fs.readFileSync(path.join(root,'export+search/API.txt'),'utf8');
const apiNames=[...apiText.matchAll(/^(.+?)\s+\[function\]\s*$/gm)].map(m=>m[1].trim());
const report={notice:'Third-party labels only. Section membership/function start does not verify semantics. No IDC executed; no catalog or binary modified.', total_labels:labels.length, exact_catalog_starts:labels.filter(r=>r.exact_catalog_start).length, data_declarations:labels.filter(r=>r.declaration==='Data').length, runtime_function_names:apiNames.length, runtime_unique_function_names:new Set(apiNames).size, labels};
const output=path.join(root,'TBC-Extensions/Docs/FRIEND_LABELS_AUDIT.json');
fs.writeFileSync(output,JSON.stringify(report,null,2)+'\n');
console.log(JSON.stringify({...report,labels:undefined,output},null,2));
