#version 150

float psin(float rate, float mult, float off, int ittr){
	float ret=0.;
	for(int x=0; x<ittr; ++x){
		ret=sin(rate*mult*float(ittr)+off+ret+cos(float(ittr)+off + float(x)))*.5+.5;
	}
	return ret;
}

/*
void kalido(float (*arr)[2], float div){
	float x=*arr[0];
	float y=*arr[1];
	x=abs(x-cW/2)/div;
	y=abs(y-cH/2)/div;
	*arr[0]=x;
	*arr[1]=y;
}*/

uniform float cW;
uniform float cH;
uniform float runner;

out vec4 outputColor;
// gl_FragCoord.x
// gl_FragCoord.y

void main(){
	vec4 cd=vec4(1.0);
	vec2 vUv=gl_FragCoord.xy/vec2(cW,cH);
	float fr=runner;
	float fx=vUv.x*2.+.01;
	float fy=vUv.y*1.5+.01;
	float r;
	float g;
	float b;
	float tempr=(sin(fr/30.+fx+cos(cos(fr/15.415+fy/52.5246)*3.9 ))*.5+.5);
	float tempg=(cos(fr/20.+(fy+.01)/tempr+cos(sin(fr/19.525+fx/51.356)/1.3 + psin(fr/31.213+fy,sin(fr/23.+fx)*10.+20.,(fx-fy/2.24),3) ))*.5+.5);
	float tempb=(cos(fr/20.+(fy+.01)/tempr+cos(sin(fr/29.525+fx/51.356)/1.3 + psin(fr/31.213+fy,sin(fr/23.+fx)*10.+20.,(fx-fy/2.24),3) ))*.5+.5);
		
	r=min(1.,max(0.,tempr*(sin(fr/21.+fx-fy/2.+sin(fr/2.-fx/10.) )*.5+.5)+psin(fr/21.213,sin(fr/23.), (fr/10.23+1323.), 2) ));
	g=min(1.,max(0.,tempg*(sin(-fr/19.+233.+fy/2.+fx*1.5-sin(fr/2.+fx)*3.)*.5+.5)+psin(fr/31.213,sin(fr/23.), (fr/14.23+23.1), 2) ));
	b=min(1.,max(0.,tempb*(sin(-fr/29.+133.+fy/2.+fx*1.5-sin(fr/5.+fx)*3.)*.5+.5)+psin(fr/41.213,sin(fr/13.), (fr/24.23), 2) ));
	cd=vec4(r,g,b,1.);
	outputColor=cd;
}


/*

					xy=kalido((double(x),double(y)), 4, 0)
					fx=xy[0]
					fy=xy[1]
					rgb=rgbNoiseWave(rgb,(fx,fy,fr), 1)
					modeVal[runMode]=rgb
					hit=1
					*/