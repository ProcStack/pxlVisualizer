#version 150

uniform float cW;
uniform float cH;
uniform float runner;
uniform int transitionActive;
uniform float transitionTimer;

//uniform sampler2D tex0;
uniform sampler2DRect curPass;
uniform sampler2DRect nextPass;

in vec2 texcoord;

out vec4 outputColor;
// gl_FragCoord.x
// gl_FragCoord.y

void main(){
	float PI=3.14159265358979;
	vec2 vUv=gl_FragCoord.xy;
	float fr=runner*.5;
	float fx=vUv.x/cW*10.;
	float fy=vUv.y/cH*.7;
	vec4 cd=vec4(1.0);
	vec4 ct=texture2DRect(curPass, vUv);
	if(transitionActive==1){
		vec4 nt=texture2DRect(nextPass, vUv);
		float blend=transitionTimer;
		blend = min(1., max(0., cos( max(0., min(PI, (blend*1.5-.25)*PI+(sin(fx*(cos(fy+fr*2.)*.5+1.)+sin(fr+3.)*PI*2.+cos(fy+fr)*PI)*.5)+(sin(fy*2.+fr+sin(fx))*.5) ) ) )*.5+.5 ) );
		blend=cos(blend*PI*.5);
		cd=ct*(1.-blend) + nt*(blend);
	}else{
		cd=ct;
	}
	cd=vec4(cd.rgb, 1);
	outputColor=cd;
}