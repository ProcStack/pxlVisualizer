#version 150

float psin(float rate, float mult, float off, int ittr){
	float ret=0.;
	for(int x=0; x<ittr; ++x){
		ret=sin(rate*mult*sin(float(ittr)*23452.3523)+off+ret+cos(float(ittr)+off + sin(float(x)*1424.5235)))*.5+.5;
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


uniform float cW;
uniform float cH;
uniform int mode;
uniform float runner;

out vec4 outputColor;
// gl_FragCoord.x
// gl_FragCoord.y


void main(){
	vec4 cd=vec4(1.0);
	float PI=3.14159265358979;
	vec2 vUv=gl_FragCoord.xy/vec2(cW,cH);
	float fr=runner;
	float fx=vUv.x;//* ( cW / cH );
	float fy=vUv.y;//* ( cH / cW );
	float r,g,b,ang;
	float scaler=1.;
	if(mode==4){
		fr*=10.;
		ang=torad(fr/4.24);
		fx=abs(fx-.5);
		fy=abs(fy-.5);

		ang*=(fx-.5)==0.?.1:(fx-.5)/abs(fx-.5);
		ang*=(fy-.5)==0.?.1:(fy-.5)/abs(fy-.5);
		vec2 xy=kalido(vec2(fx,fy), ang+sin(fx/231.53+fy/142.3+fr/440.0)*2.3 , 1.);
					
		scaler=20.;
		fx=float(xy.x)*scaler;
		fy=float(xy.y)*scaler;
	}else if(mode==5){
		fr*=5.;
		ang=torad(fr/20.24+cos(fx/7.4)*10.4);
		fx=abs(fx-.5);
		fy=abs(fy-.5);
		ang*=(fx-.5)==0.?.1:(fx-.5)/abs(fx-.5)*-1;
		ang*=(fy-.5)==0.?.1:(fy-.5)/abs(fy-.5)*-1;
		scaler=3.;
		fx=fx*scaler;
		fy=fy*scaler;
		vec2 xy=kalido(vec2(fx,fy), ang/2 , 1.);
					
		fx=float(xy.x)*scaler;
		fy=float(xy.y)*scaler;
	}else if(mode==6){
		fr=(fr*.002)+.02;
		scaler=.1;
		ang=torad(fr*2.24+cos(fx*2.4-fr-PI*2)*60.+cos(fy*1.4+123.2)*60.);
		fx=fx*scaler+.2;
		fy=fy*scaler+.2;
		float mather=sin(fx/20.0+fr/5.3);
		mather=mather==0?1.: sin(fx/20.0+fr/5.3)/abs(mather);
		fx=abs((fx+sin(fx*5.0+fr*15.)*3.3+fr)/scaler*mather);
		fx+= psin(fr*1.213+fy*.2,sin(fr*23.+fx*.2)*10.+20.,(fx-fy/2.24),4);
		vec2 xy=kalido(vec2(fx,fy), ang , 1.);
		fx=float(fx+xy.x)*scaler;
		fy=float(fy+xy.y)*scaler;
	}

	float tempr=(sin(fr/30.0+234.23+fx/2.342+cos(cos(fr/15.415+fy/5.5246+234.2) ))*.5+.5);
	float tempg=(cos(fr/25.0+63.526+fy/3.32+cos(fx*2.0+fr/12.0)*PI+sin(fx/5.356+24.3)*1.3 )*.5+.5);
	float tempb=(cos(fr/20.0+43.23+fy/2.0+cos(sin(fr/19.525+fx/1.356+52.3)*1.3  ))*.5+.5);

	
	float blender=clamp(sin(fr/31.3-fx/24.3+sin(fx/132.5+fr/542.3)*PI)*.7+.5, 0.,1.);
	tempr=tempr*blender+tempg*(1.-blender);
	blender=clamp(cos(fr/61.3+34.23+fy/54.3+cos(fy/132.5+fr/342.3-234.23)*PI)*.7+.5, 0.,1.);
	tempb=tempg*blender+tempb*(1.-blender);

	if(mode==5){
		vec3 rgb=toHSV(vec3(tempr,tempg,tempb));
		rgb.x=(rgb.x-sin(fr/24.0+fr/43.3)*98.0+cos(fy/12.0+fr/30.24)*98.+fr/2.0);
		rgb.x=rgb.x/360.;
		rgb.x=(rgb.x-float(int(rgb.x)))*360.;
		rgb=toRGB(rgb);
		tempr=rgb.x;
		tempg=rgb.y;
		tempb=rgb.z;
	}else if(mode==6){
		fr=runner*-.005;
		vec3 rgb=toHSV(vec3(tempr,tempg,tempb));
		rgb.x=(rgb.x+vUv.x*40.+fr*2.+vUv.y*20.-sin(fr*12.0+fr*5.3+vUv.x*2.6282+toGrey(vec3(tempr,tempg,tempb))*PI)*98.0+cos(fy*.14+fr*5.24)*78.+fr*5.0+  psin(fr*1.213+fy*.2,sin(fr/3.+fx*.2)*10.+20.,(fx*2.-fy*2.24),4)*90. );
		rgb.x=rgb.x/360.;
		rgb.x=(rgb.x-float(int(rgb.x)))*360.;
		rgb=toRGB(rgb);
		tempr=rgb.x;
		tempg=rgb.y;
		tempb=rgb.z;
	}

	r=tempr;
	g=tempg;
	b=tempb;
	cd=vec4(r,g,b,1.);
	outputColor=cd;
}

