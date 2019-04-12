#version 150

uniform float cW;
uniform float cH;
uniform int mode;
uniform float runner;

out vec4 outputColor;
// gl_FragCoord.x
// gl_FragCoord.y

float psin(float rate, float mult, float off, int ittr){
	float ret=0.;
	for(int x=0; x<ittr; ++x){
		ret=sin(rate*mult*sin(float(ittr)*23452.3523)+off+ret+cos(float(ittr)+off + sin(float(x)*1424.5235)))*.5+.5;
	}
	return ret;
}


vec3 toHSV_skewed(vec3 RGB) {
	float H,S,V,r,g,b,d,minv,maxv;
	r = RGB.x;
	g = RGB.y;
	b = RGB.z;
	minv = min(r, min(g, b));
	maxv = max(r, max(g, b));
	V = maxv;
	d = maxv - minv;

	if(d<.00001){
		S = 0.;
		H = -1.;
		return vec3(H,S,V);
	}

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

vec3 toRGB_skewed(vec3 HSV){
	float H,S,V, R,G,B, i,f,p,q,t;
	int ii;
	H=HSV.x;
	S=HSV.y;
	V=HSV.z;
	if(S == 0. ){
		return vec3(V,V,V);
	}
	
	H=H/360.+5.;
	H=(H-float(int(H)))*360.;
	H/=60.;
	i=floor(H);
	ii=int(i);
	f=H-i;
	p=V*(1.-S);
	q=V*(1.-(S*f));
	t=V*(1.-(S*(1.-f)));
	if(ii == 0){
		R=V;
		G=t;
		B=p;
	}else if( ii == 1){
		R=q;
		G=V;
		B=p;
	}else if( ii == 2){
		R=p;
		G=V;
		B=t;
	}else if( ii == 3){
		R=q;
		G=p;
		B=V;
	}else if( ii == 4){
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

float toGrey(vec3 RGB){ // Visually correct Greyscale
	return RGB.x * 0.2989 + RGB.y * 0.5870 + RGB.z * .1140;
}
float toGray(vec3 RGB){
	return toGrey(RGB);
}
float toGreyPerfect(vec3 RGB){ //Mathematically correct greyscale -- I will probably never use this
	return (RGB.x+RGB.y+RGB.z)/3.;
}
	
float todeg(float rad){
float PI=3.14159265358979;
	return float(rad*(180/PI));
}

float torad(float deg){
float PI=3.14159265358979;
	return float(deg*(PI/180));
}
vec2 kalido(vec2 xy, float angle, float mult){
	float x,y,s, c, editx,edity;
	s=sin(angle)*mult;
	c=cos(angle)*mult;
	editx=xy.x-1.;
	edity=xy.y-1.;
	x=(editx*c-edity*s)+1.;
	y=(-editx*s+edity*c)+1.;

	return vec2(x,y);
}


void main(){
	vec4 cd=vec4(1.0);
	float PI=3.14159265358979;
	vec2 vUv=gl_FragCoord.xy/vec2(cW,cH);
	float fr=runner;
	float fx=vUv.x;//* ( cW / cH );
	float fy=vUv.y;//* ( cH / cW );
	float r, g, b,tempr,tempg,tempb;

	float ang=sin(fr/130.0+sin(fx+fr/20.)*PI-cos(fy)*PI)*180.;
	ang=torad(ang);
	vec2 xy;
	
	if(mode==2){
		ang=sin(fr/130.0+sin(fx+fr/20.)*PI-cos(fy)*PI)*180.;
		ang=torad(ang);
		xy=kalido(vec2(fx,fy), ang, 1.);
	}else if(mode==3){
		ang=sin(fr/130.0+sin(fy+fr/20.)*PI-cos(fx)*PI)*180.;
		ang=torad(ang);
		xy=kalido(vec2(fx,fy), ang, 1.);
	}
	float scaler=5.;
	fx=(xy.x)*scaler;
	fy=(xy.y)*scaler;
		
	tempr=(sin(fr/19.0+fx/3.3+sin(fr/15.415+fy/2.5246)*5.9 )*.5+.5);
	tempg=(cos(fr/30.0+fy/4.3*sin(fx*2.0+fr/23.3 ))*.5+.5);
	tempb=(cos(fr/40.0+fy/2.0+cos(sin(fr/9.525+fx/1.356)/1.3  ))*.5+.5);
		
	float blender=clamp(sin(fr/31.3+fx/2.3+cos(fx/2.5+fr/342.3)*PI)*.7+.3, 0.,1.);
	tempr=tempr*blender+tempg*(1.-blender);

	if(mode==3){
		vec3 rgb=toHSV_skewed(vec3(tempr,tempg,tempb));
		if(rgb.x>-1){
			rgb.x=rgb.x+(fy*.4+sin(fx/2.0+fr/3.)*PI*2.)*.01;
			rgb.x=rgb.x/360.+5.;
			rgb.x=(rgb.x-float(int(rgb.x)))*360.;
			rgb.x=(sin(rgb.x*.005)*.5+.5)*360.;
			rgb=toRGB_skewed(rgb);
			tempr=rgb.x;
			tempg=rgb.y;
			tempb=rgb.z;
		}
	}
	r=tempr;
	g=tempg;
	b=tempb;
	cd=vec4(r,g,b,1.);
	outputColor=cd;
}


