#version 150


uniform float cW;
uniform float cH;
uniform float runner;

out vec4 outputColor;
// gl_FragCoord.x
// gl_FragCoord.y

float psin(float rate, float mult, float off, int ittr){
	float ret=0.;
	for(int x=0; x<ittr; ++x){
		ret=sin(rate*mult*float(ittr)+off+ret+cos(float(ittr)+off + float(x)))*.5+.5;
	}
	return ret;
}




vec3 toHSV(vec3 RGB) {
	float H,S,V,r,g,b,d,minv,maxv;
	r = RGB.x;
	g = RGB.y;
	b = RGB.z;
	minv = min(r, min(g, b));
	maxv = max(r, max(g, b));
	V = maxv;
	d = max(1., maxv - minv);

	if(maxv != 0.){
		S = d / maxv;
	}else { 
		S = 0.;
		H = -1.;
		return vec3(H,S,V);
	}
	if(r == maxv){
		H=(g-b)/d;
	}else if( g == maxv){
		H=2.+(b-r)/d;
	}else{
		H=4.+(r-g)/d;
	}
	H *= 60.;
	if(H < 0.){
		H += 360.;
	}
	return vec3(H,S,V);
}

vec3 toRGB(vec3 HSV){
	float H,S,V, R,G,B, i,f,p,q,t;
	H=HSV.x;
	S=HSV.y;
	V=HSV.z;
	if(S == 0. ){
		return vec3(V,V,V);
	}
	H/=60.;
	i=floor(H);
	f=H-i;
	p=V*(1.-S);
	q=V*(1.-S*f);
	t=V*(1.-S*(1-f));
	if(i == 0.){
		R=V;
		G=t;
		B=p;
	}else if( i == 1.){
		R=q;
		G=V;
		B=p;
	}else if( i == 2.){
		R=p;
		G=V;
		B=t;
	}else if( i == 3.){
		R=q;
		G=p;
		B=V;
	}else if( i == 4.){
		R=t;
		G=p;
		B=V;
	}else{
		R=V;
		G=p;
		B=q;
	}
	return vec3(R,G,B);
}

float toGreyPerfect(vec3 RGB){ //Mathematically correct greyscale -- I will probably never use this
	return (RGB.x+RGB.y+RGB.z)/3.;
}
void main(){
	vec2 vUv=gl_FragCoord.xy / vec2(cW,cH) *.03;
	vUv=vec2(vUv.y,vUv.x);
	vec4 cd=vec4(1.0);
	float fr=runner*.001;
	float fx=vUv.x*(vUv.y+.7)+sin(vUv.y+runner*.1)*.001;
	float fy=vUv.y*(vUv.x+.3);
	cd.r=(( psin(fr*91.213+fx/40.,4.,(fx*631.-sin(fr*30.+fy)*5.),4) )*.5+.5);
	cd.g=(( psin(-fr*91.213+fy/540.,3.,(fx*631.-sin(fr*30.+fy)*5.),5) )*.5+.5);
	cd.b=( sin(sin(fx/122.)*.2+.5-fr/113.)* sin(fr/31.-fy/22.*sin(fx/3.+fy/2.+fr/33.)) );
	cd.r=min(1., max(.001, cd.r*(sin(fr/213.+fy/24.)*.7+.5) ));
	cd.g=min(1., max(.001, cd.g*(sin(fr/213.+fx/43.)*.7+.5)*(sin(fr/260.)+.5) ));
	cd.b=((psin(-fr/51.213+fy/240.,3.,(fx/231.-sin(fr/30.+fy)*5.),3))*.5+.5);

	
	fr=runner*.003;
	vec3 rgb=toHSV(cd.rgb);
	rgb.x=(rgb.x+fx*60.+fr*2.+fy*120.-sin(fr*12.0+fr*5.3+fx*22.6282)*98.0+cos(fy*3.14+fr*5.24)*78.+fr*5.0 + toGreyPerfect(cd.rgb)*120. +  psin(fr*2.213+fy*.2,sin(fr/3.+fx*4.2)*10.+20.,(fx*32.-fy*2.24),5)*90. );
	rgb.x=rgb.x/360.;
	rgb.x=(rgb.x-float(int(rgb.x)))*360.;
	rgb=toRGB(rgb);
	cd.rgb=rgb;

	outputColor=cd;


}
