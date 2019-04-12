#version 150

uniform float cW;
uniform float cH;
uniform int mode;
uniform float runner;

out vec4 outputColor;
// gl_FragCoord.x
// gl_FragCoord.y

float torad(float deg){
float PI=3.14159265358979;
	return float(deg*(PI/180));
}

float psin(float rate, float mult, float off, int ittr){
	float ret=0.;
	for(int x=0; x<ittr; ++x){
		ret=sin(rate*mult*sin(float(ittr)*23452.3523)+off+ret+cos(float(ittr)+off + sin(float(x)*1424.5235)))*.5+.5;
	}
	return ret;
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
	float r;
	float g;
	float b;
	
	fr*=.04;
	float ang=sin(cos(fr*.7)*PI*2.+sin(fx*.2+fr)*PI-cos(fy*.2)*PI)*360.;
	ang=torad(ang);
	vec2 xy=kalido(vec2( fx+psin(fr*1.213+fy*.2,sin(fr*1.+fx*1.2)*15.+20.,(fx-fy/2.24),4)*5.*sin(fr*.2+cos(fx*PI*2.3+PI*.5)*PI + sin(fy*PI*1.4+PI*.5)*PI +PI*.5)*.35+1.,
		fy+50.0+psin(fr*1.213+fy*1.2,sin(fr*1.+fy*1.2)*15.+20.,(fr-fy/2.24),4)*5.*sin(fr*.2+cos(fx*PI*2.4+PI*.5)*PI + sin(fy*PI*1.2+PI*.5)*PI +PI*.5)*.35+1. ),
		ang, sin(fr+cos(fx*PI)*PI + sin(fy*PI)*PI )*.3+1. );
					
	float scaler=.1;
	fx=(xy.x)*scaler;
	fy=(xy.y)*scaler;

	fr=runner*.1;

	float tempr=(sin(fr/30.+fx/23.3+cos(sin(fr/15.415+fy/52.5246)*5.9 ))*.5+.5);
	float tempg=(cos(fr/20.+fy*(sin(fx/20.+fr/2.0)+cos(sin(fx/51.356)*6.3+7.) ))*.5+.5);
	float tempb=(cos(fr/20.+353.2+fy/322.+cos(sin(fr*9.525+fx/31.356)/1.3  ))*.5+.5);

	r=tempr;
	g=tempg;
	b=tempb;
	cd=vec4(r,g,b,1.);
	outputColor=cd;

	outputColor=cd;
}