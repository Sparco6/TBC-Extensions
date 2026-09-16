const fs=require('fs'),path=require('path');const out=process.argv[2];if(!out)throw Error('output folder required');fs.mkdirSync(out,{recursive:true});
const w=64,h=64,pixels=w*h;function header(enc,depth,alpha,start,size){const b=Buffer.alloc(start+size);b.write('BLP2');b.writeUInt32LE(1,4);b[8]=enc;b[9]=depth;b[10]=alpha;b[11]=0;b.writeUInt32LE(w,12);b.writeUInt32LE(h,16);b.writeUInt32LE(start,20);b.writeUInt32LE(size,84);return b;}
const bgra=header(3,8,0,148,pixels*4);for(let y=0;y<h;y++)for(let x=0;x<w;x++){const i=148+(y*w+x)*4,edge=Math.min(x,y,w-1-x,h-1-y);bgra[i]=x<32?40:220;bgra[i+1]=y<32?60:210;bgra[i+2]=x<32?230:50;bgra[i+3]=Math.min(255,edge*32);}fs.writeFileSync(path.join(out,'TBCExt_BLP2Test.blp'),bgra);
const pal=header(1,8,8,1172,pixels*2);for(let i=0;i<256;i++){pal[148+i*4]=i;pal[149+i*4]=255-i;pal[150+i*4]=(i*3)&255;pal[151+i*4]=255;}for(let y=0;y<h;y++)for(let x=0;x<w;x++){const p=y*w+x;pal[1172+p]=((x>>3)+(y>>3)*8)&63;pal[1172+pixels+p]=Math.min(255,Math.min(x,y,w-1-x,h-1-y)*32);}fs.writeFileSync(path.join(out,'TBCExt_BLP2Palette.blp'),pal);
function c565(r,g,b){return ((r>>3)<<11)|((g>>2)<<5)|(b>>3)}
function dxt(kind){const blockBytes=kind===1?8:16,b=header(2,kind===1?0:kind===3?4:8,kind===1?0:kind===3?1:7,148,(w/4)*(h/4)*blockBytes);let q=148;for(let by=0;by<h/4;by++)for(let bx=0;bx<w/4;bx++){const even=(bx+by)%2===0,r=even?235:35,g=kind===1?70:kind===3?210:80,bl=even?40:230;if(kind===3){for(let i=0;i<8;i++)b[q+i]=(bx===0||by===0)?0x88:0xff;q+=8}else if(kind===5){b[q]=255;b[q+1]=48;for(let i=2;i<8;i++)b[q+i]=0;q+=8}const c0=c565(r,g,bl),c1=c565(Math.max(0,r-40),Math.max(0,g-40),Math.max(0,bl-40));b.writeUInt16LE(c0,q);b.writeUInt16LE(c1,q+2);b.writeUInt32LE(0,q+4);q+=8;}return b}
fs.writeFileSync(path.join(out,'TBCExt_DXT1.blp'),dxt(1));fs.writeFileSync(path.join(out,'TBCExt_DXT3.blp'),dxt(3));fs.writeFileSync(path.join(out,'TBCExt_DXT5.blp'),dxt(5));
function rawMipChain(name,width,height){
 const levels=[];let mw=width,mh=height,total=148;
 for(let level=0;level<16;level++){
  const bytes=mw*mh*4;if(!Number.isSafeInteger(bytes)||bytes<=0||total+bytes>0xffffffff)throw Error('unsafe mip arithmetic');
  levels.push({width:mw,height:mh,offset:total,bytes});total+=bytes;
  if(mw===1&&mh===1)break;mw=Math.max(1,Math.floor(mw/2));mh=Math.max(1,Math.floor(mh/2));
 }
 const b=Buffer.alloc(total);b.write('BLP2');b.writeUInt32LE(1,4);b[8]=3;b[9]=8;b[10]=0;b[11]=levels.length>1?1:0;
 b.writeUInt32LE(width,12);b.writeUInt32LE(height,16);
 const colors=[[0,0,255,255],[255,0,0,255],[0,255,255,255],[255,255,0,255],[255,0,255,255],[0,255,0,255],[0,128,255,255],[255,128,0,255],[255,255,255,255]];
 levels.forEach((m,level)=>{b.writeUInt32LE(m.offset,20+level*4);b.writeUInt32LE(m.bytes,84+level*4);const c=colors[level%colors.length];
  for(let y=0;y<m.height;y++)for(let x=0;x<m.width;x++){const i=m.offset+(y*m.width+x)*4;const border=x===0||y===0||x+1===m.width||y+1===m.height;const stripe=((x>>Math.max(0,5-level))+(y>>Math.max(0,5-level)))%2;
   b[i]=border?255:c[0];b[i+1]=stripe?c[1]:Math.floor(c[1]/2);b[i+2]=border?255:c[2];b[i+3]=255;}}
 );
 fs.writeFileSync(path.join(out,name),b);return levels;
}
const square=rawMipChain('TBCExt_BGRA8_256_Mips.blp',256,256);
const nonsquare=rawMipChain('TBCExt_BGRA8_128x64_Mips.blp',128,64);
console.log('Created isolated controls plus deterministic raw BGRA8 chains: '+square.length+' square mips, '+nonsquare.length+' non-square mips');
