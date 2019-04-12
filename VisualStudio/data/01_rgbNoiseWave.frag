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
vec2 kalido_noRot(vec2 xy, float div){
	float x=abs(xy.x-cW/2.)/div;
	float y=abs(xy.y-cH/2.)/div;
	return vec2(x,y);
}



void main(){
	vec4 cd=vec4(1.0);
	vec2 vUv=gl_FragCoord.xy/vec2(cW,cH);
	float fr=runner;
	float fx=vUv.x*2.+.01;
	float fy=vUv.y*1.5+.01;
	float r;
	float g;
	float b;
	float tempr=(sin(fr/30.+fx+cos(cos(fr/15.415+fy/52.5246)*3.9 ))*.5+.7);
	float tempg=(cos(fr/20.+(fy+.01)/tempr+cos(sin(fr/19.525+fx/51.356)/1.3 + psin(fr/31.213+fy,sin(fr/23.+fx+1.)*7.+20.,(fx-fy/2.24+1.),3) ))*.5+.7);
	float tempb=(cos(fr/20.+(fy+.01)/tempr+cos(sin(fr/29.525+fx/51.356)/1.3 + psin(fr/31.213+fy,sin(fr/23.+fx+1.)*7.+20.,(fx-fy/2.24+1.),3) ))*.5+.7);
		
	r=min(1.,max(0.,tempr*(sin(fr/21.+fx-fy/2.+sin(fr/2.-fx/10.+1.) )*.4+.5)+psin(fr/21.213,sin(fr/23.)+1.5, (fr/10.23+1323.), 2) ));
	g=min(1.,max(0.,tempg*(sin(-fr/19.+233.+fy/2.+fx*1.5+1.-sin(fr/2.+fx+1.)*3.)*.4+.5)+psin(fr/31.213,sin(fr/23.)+1.5, (fr/14.23+23.1), 2) ));
	b=min(1.,max(0.,tempb*(sin(-fr/29.+133.+fy/2.+fx*1.5+1.-sin(fr/5.+fx+1.)*3.)*.4+.5)+psin(fr/41.213,sin(fr/13.)+1.5, (fr/24.23), 2) ));
	cd=vec4(r,g,b,1.);
	outputColor=cd;
}
